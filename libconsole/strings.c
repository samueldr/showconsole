/*
 * strings.c
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
#include "libconsole.h"

/*
 * Append on buffer `*buf' the string `*str' and append '\0'.
 * This generates a string list like:
 *  str1\0str1\0str3\0
 * The overall length is return in `size' and the final
 * buffer in `*buf'.
 */
void str0append(char **buf, size_t *size, const char *str)
{
    if (!buf || !size) {
	errno = EINVAL;
	error("can not allocate string buffer");
    }

    if (!*buf) {
	*size = strlen(str)+1;

	*buf = strdup(str);
	if (!*buf)
	    error("can not allocate string buffer");

    } else {
	off_t off = *size;

	*size += strlen(str)+1;

	*buf = realloc(*buf, *size);
	if (!*buf)
	    error("can not allocate string buffer");

	strcpy(&(*buf)[off], str);
    }
}
