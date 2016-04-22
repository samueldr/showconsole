/*
 * isserial.c
 *
 * Copyright 2000 Werner Fink, 2000 SuSE GmbH Nuernberg, Germany.
 *
 * This source is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <sys/ioctl.h>
#include <errno.h>

int main()
{
    int serial, ret = 0;

    if (ioctl (0, TIOCMGET, (char*)&serial) == 0)
	goto out;
    /* Expected error */
    serial = errno = 0;
    ret = 1;
out:
    return ret;
}
