/*
 *  PearBox
 *  createimage.cpp
 *
 *  Copyright (C) 2015 Muhammad Mominul Huque
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* Based on Bochs(Bximage) source code */

/****************************************************************************
 * Portions of this file contain code released under the following license. *
 ****************************************************************************
 *
 * $Id: bximage_old.c 12364 2014-06-07 07:32:06Z vruppert $
 *
 *  Copyright (C) 2001-2013  The Bochs Project
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

/* TODO:
 * - Add Endianness Code to remove hack (BX_LITTLE_ENDIAN)
 * - Fix fatal in fileset()
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

//#include "../osdep.h"
//#include "bswap.h"
#include "tools/types.h"

#define BX_MAX_CYL_BITS 24 // 8 TB

const int bx_max_hd_megs = (int)(((1 << BX_MAX_CYL_BITS) - 1) * 16.0 * 63.0 / 2048.0);

typedef bool (*WRITE_IMAGE)(FILE*, uint64);

// Hack Hack Hack! TODO: Check by cmake to sure about endianness
#define BX_LITTLE_ENDIAN 1 // Host is Little Endian (x86) Hack ^^

/*               From hdimage.h(Bochs)               */
// SPARSE IMAGES HEADER
#define SPARSE_HEADER_MAGIC  (0x02468ace)
#define SPARSE_HEADER_VERSION  2
#define SPARSE_HEADER_V1       1
#define SPARSE_HEADER_SIZE        (256) // Plenty of room for later
#define SPARSE_PAGE_NOT_ALLOCATED (0xffffffff)

 typedef struct
 {
   uint32  magic;
   uint32  version;
   uint32  pagesize;
   uint32  numpages;
   uint64  disk;

   uint32  padding[58];
 } sparse_header_t;

// htod : convert host to disk (little) endianness
// dtoh : convert disk (little) to host endianness
#if defined (BX_LITTLE_ENDIAN)
#define htod16(val) (val)
#define dtoh16(val) (val)
#define htod32(val) (val)
#define dtoh32(val) (val)
#define htod64(val) (val)
#define dtoh64(val) (val)
#else
#define htod16(val) ((((val)&0xff00)>>8) | (((val)&0xff)<<8))
#define dtoh16(val) htod16(val)
#define htod32(val) bx_bswap32(val)
#define dtoh32(val) htod32(val)
#define htod64(val) bx_bswap64(val)
#define dtoh64(val) htod64(val)
#endif
/*               End               */

// fileset is like memset but for a file handle
void fileset(FILE * fp, int c, size_t n)
{
#define BLOCK_SIZE (1024)
 int block[BLOCK_SIZE];
 size_t left_to_write = n;

 memset(block, c, sizeof(block));

 while(left_to_write > 0)
 {
   size_t write = sizeof(block);
   if (write > left_to_write) write = left_to_write;

   if (1 != fwrite(block, write, 1, fp))
   {
     fclose(fp);
     //fatal("\nERROR: The disk image is not complete - could not write data block!");
   }

   left_to_write -= write;
 }
}

/* produce a flat image file */
bool make_flat_image(FILE *fp, uint64 sec)
{
  bool bRet = false;
 /*
  * seek to sec*512-1 and write a single character.
  * can't just do: fseek(fp, 512*sec-1, SEEK_SET)
  * because 512*sec may be too large for signed int.
  */
  while(sec > 0)
  {
    /* temp <-- min(sec, 4194303)
     * 4194303 is (int)(0x7FFFFFFF/512)
     */
    long temp = (long)((sec < 4194303) ? sec : 4194303);
    fseek(fp, 512*temp, SEEK_CUR);
    sec -= temp;
  }

  fseek(fp, -1, SEEK_CUR);
  if (fputc('\0', fp) == EOF)
  {
    fclose(fp);
    bRet = false;
    printf("\nERROR: The disk image is not complete! (image larger then free space?)");
    return bRet;
  }
  bRet = true; // File Created!

  return bRet;
}

/* produce a sparse image file */
bool make_sparse_image(FILE *fp, uint64 sec)
{
  uint64 numpages;
  sparse_header_t header;
  size_t  sizesofar;
  size_t  padtopagesize;
  bool bRet = false;

  memset(&header, 0, sizeof(header));
  header.magic = htod32(SPARSE_HEADER_MAGIC);
  header.version = htod32(SPARSE_HEADER_VERSION);

  header.pagesize = htod32((1 << 10) * 32); // Use 32 KB Pages - could be configurable
  numpages = (sec / (dtoh32(header.pagesize) / 512)) + 1;

  header.numpages = htod32((uint32)numpages);
  header.disk = htod64(sec * 512);

  if (numpages != dtoh32(header.numpages))
  {
    fclose(fp);
    printf("\nERROR: The disk image is too large for a sparse image!");
    bRet = false;
    return bRet;
    // Could increase page size here.
    // But note this only happens at 128 Terabytes!
  }

  if (fwrite(&header, sizeof(header), 1, fp) != 1)
  {
    fclose(fp);
    printf("\nERROR: The disk image is not complete - could not write header!");
    bRet = false;
    return bRet;
  }

  fileset(fp, 0xff, 4 * dtoh32(header.numpages));

  sizesofar = SPARSE_HEADER_SIZE + (4 * dtoh32(header.numpages));
  padtopagesize = dtoh32(header.pagesize) - (sizesofar & (dtoh32(header.pagesize) - 1));

  fileset(fp, 0, padtopagesize);
  bRet = true; // File Created!

  return bRet;
}

/* produce the image file */
bool make_image(uint64 sec, char *filename, WRITE_IMAGE write_image)
{
  FILE *fp;
  char buffer[1024];
  bool bRet = false;

  // check if it exists before trashing someone's disk image
  fp = fopen(filename, "r");
  if (fp) {
    // File Exists
    bRet = false;
    fclose(fp);
    return bRet;
  }

  // okay, now open it for writing
  fp = fopen(filename, "w");
  if (fp == NULL) {
    // attempt to print an error
    bRet = false;
    printf("\nERROR: Could not write disk image");
    return bRet;
  }

  if((*write_image)(fp, sec) != true) {
    bRet = false;
    printf("\nERROR: while writing disk image!");
    return bRet;
  }
  /*if (!sparse){
    if(make_flat_image(fp, sec) != true){
      bRet = false;
      printf("\nERROR: while writing disk image!");
      return bRet
    }else {
      goto image_ok;
    }
  }else{
    if(make_sparse_image(fp, sec) != true) {
      bRet = false;
      printf("\nERROR: while writing disk image!");
      return bRet
    }else{
      goto image_ok;
    }
  }*/

//image_ok:
  fclose(fp);
  bRet = true; // File Created!
  return bRet;
}

bool Create_HD_Image ( int hdsize, char *path, bool sparse )
{
  uint64 sectors = 0;
  uint64 cyl;
  int heads=16, spt=63;
  bool bRet = false;

  WRITE_IMAGE write_function=NULL;

  cyl = (uint64)(hdsize*1024.0*1024.0/16.0/63.0/512.0);
  assert(cyl < (1 << BX_MAX_CYL_BITS));
  sectors = cyl*heads*spt;

  if ( !sparse ){
    // We not want a sparse file
    write_function=make_flat_image;
  } else {
    // We want a sparse file
    write_function=make_sparse_image;
  }

  bRet = make_image(sectors, path, write_function);
  if ( !bRet ) {
    // File Not Created!
    printf("\n[Error] File not Created!");
    return bRet;
  }

  // File Created!
  printf("\n[CHDI] Disk image '%s' Created with size %d MB",path,hdsize);
  bRet = true;

  return bRet;
}
