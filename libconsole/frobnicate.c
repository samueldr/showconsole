/*
 * frobnicate.c
 *
 * Copyright 2015 Werner Fink, 2015 SuSE Linux GmbH.
 *
 * This source is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static unsigned char randnum;
static void initialseed(void) __attribute__((__constructor__));
static void initialseed(void)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    srand (tv.tv_sec ^ tv.tv_usec ^ getpid());
    randnum = (unsigned char)(rand() & 0xff);
    if (!randnum)
	randnum = 42;
}

void *frobnicate(void *in, const size_t len)
{
    unsigned char *ptr = (unsigned char *)in;
    ssize_t pos = len;

    while (pos-- > 0)
	*ptr++ ^= randnum;

    return in;
}
