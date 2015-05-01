/*    
 *  PearPC Virtual Machine Manager - configuration.h
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

#include <exception>

#include "tools/except.h"
#include "tools/snprintf.h"
#include "configparser.h"

typedef struct
{
	String resolution;
	int full_screen;
	int redraw;
	/* Key Codes */
	String compose_dialog;
	String change_cd;
	String mouse_grab;
	String fullscreen_k;
	/* Loader */
	String bootmethod;
	String loadfile;
	String bootpath;
	String bootargs;
	String machargs;
	String driver_graph;
	/* CPU */
	uint32 pvr;
	uint32 pagetable;
	/* Memory */
	uint32 memory;
	/* IDE */
	int ide0;
	String ide0_path;
	String ide0_type;
	int ide0_s;
	String ide0_s_path;
	String ide0_s_type;
	/* Network */
	int net_3c;
	String net_3c_mac;
	int net_rtl;
	String net_rtl_mac;
	/* USB */
	int usb;
	/* Serial Port */
	int serial;
	/* NVRAM */
	String nvram;
} CONF;

bool load_config ( CONF& config, String path );
bool save_config ( CONF& cnf, String path );
