/* efunc.h: error wrapper functions */
extern void		eprintf(const char *, ...);
extern const char	*esetprogname(const char *);
extern void		*emalloc(size_t);
extern void		*erealloc(void *, size_t);
extern const char	*estrdup(const char *);
