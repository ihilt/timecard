#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "efunc.h"
#include "utils.h"

int main(int argc, char **argv)
{
	int fd = open(argv[3], O_RDONLY);

	const char *prgname;

	prgname = setprogname("timecard");

	if (fd < 0)
		eprintf("No such file: %s", argv[3]);

	printf("%.2f\n", get_hours_worked(argv[1], argv[2], argv[3]));

	free((void *)prgname);
	return 0;
}
