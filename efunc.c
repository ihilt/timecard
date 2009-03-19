#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "efunc.h"

static const char *name = NULL;

/* progname: return stored name of program */
static const char *progname(void)
{
	return name;
}

/* eprintf: print error message and exit */
void eprintf(const char *fmt, ...)
{
	va_list args;

	fflush(stdout);
	if (progname() != NULL)
		fprintf(stderr, "%s: ", progname());

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':')
		fprintf(stderr, " %s", strerror(errno));
	fprintf(stderr, "\n");
	exit(2);
}

/* estrdup: duplicate a string, report if error */
const char *estrdup(const char *s)
{
	char *t;

	t = (char *) malloc(strlen(s)+1);
	if (t == NULL)
		eprintf("estrdup(\"%.20s\") failed:", s);
	strcpy(t, s);
	return t;
}

/* esetprogname: set stored name of program */
const char *esetprogname(const char *str)
{
	name = estrdup(str);
	return name;
}

/* emalloc: malloc and report if error */
void *emalloc(size_t n)
{
	void *p;

	p = malloc(n);
	if (p == NULL)
		eprintf("malloc of %u bytes failed:", n);
	return p;
}

/* erealloc: realloc and report if error */
void *erealloc(void *q, size_t n)
{
	void *p;

	p = realloc(q, n);
	if (p == NULL)
		eprintf("realloc of %u bytes failed:", n);
	return p;
}
