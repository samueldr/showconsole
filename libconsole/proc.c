/*
 * proc.c
 *
 * Copyright 2015 Werner Fink, 2015 SuSE Linux GmbH.
 *
 * This source is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "libconsole.h"

/*
 * Get real path of the binary of a specifiy pid.
 */
char *proc2exe(const pid_t pid)
{
    char *tmp, *exe;
    int ret;

    ret = asprintf(&exe, "/proc/%lu/exe", (unsigned long)pid);
    if (ret < 0)
	error("can not allocate string for /proc/%lu/exe", (unsigned long)pid);

    tmp = exe;
    exe = realpath(exe, NULL);
    if (!exe) {
	if (errno != ENOENT && errno != ENOTDIR)
	    error("can not allocate string for /proc/%lu/exe", (unsigned long)pid);
    }
    free(tmp);

    return exe;
}

/*
 * For debugging: show open file descriptors
 */
void list_fd(const pid_t pid)
{
    struct dirent *dr;
    char *fds;
    DIR *dir;
    int ret;

    ret = asprintf(&fds, "/proc/%lu/fd", (unsigned long)pid);
    if (ret < 0)
	error("can not allocate string for /proc/%lu/fd", (unsigned long)pid);

    dir = opendir(fds);
    if (!dir) {
	warn("can not open %s", fds);
	return;
    }
    free(fds);

    while ((dr = readdir(dir))) {
	char tmp[LINE_MAX+1];
	ssize_t len;
	if (dr->d_name[0] == '.')
	    continue;
	len = readlinkat(dirfd(dir), dr->d_name, &tmp[0], LINE_MAX);
	tmp[len] = '\0';
	fprintf(stderr, "/proc/%d/fd/%s %s\n", (int)pid, dr->d_name, tmp);
    }
    closedir(dir);
}
