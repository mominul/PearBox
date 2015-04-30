#ifndef __SYSFILE_H__
#define __SYSFILE_H__
int sys_pstat(pstat_t &s, const char *filename);
int sys_pstat_fd(pstat_t &s, int fd);
int sys_truncate(const char *filename, FileOfs ofs);
#endif
