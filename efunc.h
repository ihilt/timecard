/* efunc.h: error wrapper functions */
extern void	eprintf(char *, ...);
extern char	*progname(void);
extern void	setprogname(char *);
extern void	*emalloc(size_t);
extern void	*erealloc(void *, size_t);
extern char	*estrdup(char *);
