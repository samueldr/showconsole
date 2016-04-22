/*
 * libblogger.h
 *
 * Copyright 2000 Werner Fink, 2000 SuSE GmbH Nuernberg, Germany.
 *
 * This source is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

extern int bootlog(const int lvl, const char *fmt, ...);
extern void closeblog();
#define B_NOTICE	((int)'n')	/* Notice  */
#define B_DONE		((int)'d')	/* Done    */
#define B_FAILED	((int)'f')	/* Failed  */
#define B_SKIPPED	((int)'s')	/* Skipped */
#define B_UNUSED	((int)'u')	/* Unused  */
