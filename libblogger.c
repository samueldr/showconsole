/*
 * libblogger.c
 *
 * Copyright 2000 Werner Fink, 2000 SuSE GmbH Nuernberg, Germany.
 *
 * This source is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef  _PATH_BLOG_FIFO
# define _PATH_BLOG_FIFO	"/dev/blog"
#endif
#include <errno.h>
#include <fcntl.h>
#include <paths.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "libblogger.h"
#include "libconsole.h"

/*
 * Use Esacape sequences which are handled by console
 * driver of linux kernel but not used.  We transport
 * with this our informations to the parser of blogd
 * whereas the linux console throw them away.
 * Problem: Does not work on serial console.
 */

#define ESNN	"notice"	/* Notice  */
#define ESND	"done"		/* Done    */
#define ESNF	"failed"	/* Failed  */
#define ESNS	"skipped"	/* Skipped */
#define ESNU	"unused"	/* Unused  */

static struct sigaction saved_sigpipe;
static volatile sig_atomic_t broken = 0;
static int fdfifo = -1;
static char * fifo_name = _PATH_BLOG_FIFO;

static void sigpipe(int sig __attribute__((__unused__)))
{
    broken++;
}

static int bootlog_init(const int lvl __attribute__((__unused__)))
{
    int ret = -1;
    struct stat st;

    if (stat(fifo_name, &st))
	goto out;

    if (!S_ISFIFO(st.st_mode))
	goto out;

    if ((fdfifo = open(fifo_name, O_WRONLY|O_NONBLOCK|O_NOCTTY|O_CLOEXEC)) < 0)
	goto out;

    set_signal(SIGPIPE, &saved_sigpipe, sigpipe);

    ret = 0;
out:
    return ret;
}

void closeblog()
{
    if (fdfifo < 0)
	goto out;

    reset_signal(SIGPIPE,  &saved_sigpipe);

    (void)close(fdfifo);
out:
    return;
}

int bootlog(const int lvl, const char *fmt, ...)
{
    va_list ap;
    int ret = -1;
    char * head = ESNN;
    char buf[4096];
    sigset_t blockpipe, oldpipe;

    if (fdfifo < 0 && bootlog_init(lvl) < 0)
	goto out;

    ret = 0;
    switch (lvl) {
	case -1:
	    head = NULL;
	    break;
	case B_NOTICE:
	    head = ESNN;
	    break;
	case B_DONE:
	    head = ESND;
	    break;
	case B_FAILED:
	    head = ESNF;
	    break;
	case B_SKIPPED:
	    head = ESNS;
	    break;
	case B_UNUSED:
	    head = ESNU;
	    break;
	default:
	    head = ESNN;
	    break;
    }

    sigprocmask(SIG_BLOCK, &blockpipe, &oldpipe);
    if (broken)
	goto pipe;
    if (head) {
	const struct tm *local;
	struct timeval now;
	char stamp[72], strnow[56];

	if ((gettimeofday(&now, (struct timezone*)0) == 0)	&&
	    ((local = localtime(&now.tv_sec)) != (struct tm*)0) &&
	    (strftime(strnow, sizeof(strnow), "%b %e %T", local) > 0))
	    snprintf(stamp, sizeof(stamp), "<%s -- %s.%lu> ", head, strnow, now.tv_usec*1000UL);
	else
	    snprintf(stamp, sizeof(stamp), "<%s> ", head);

	ret = write(fdfifo, stamp, strlen(stamp));
    }
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    ret = write(fdfifo, buf, strlen(buf));
pipe:
    sigprocmask(SIG_SETMASK, &oldpipe, NULL);
out:
    return ret;
}
