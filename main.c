#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include "efunc.h"

#define DATE_MAX_LEN (12)
#define TIME_MAX_LEN (5)

typedef struct nameval_t nameval_t;
struct nameval_t {
	char date[DATE_MAX_LEN];
	double hours;
};

static struct nvtab_t {
	int nval;
	int max;
	nameval_t *nameval;
} nvtab;

enum { NVINIT = 1, NVGROW = 2 };

static int addname(nameval_t newname)
{
	nameval_t *nvp;

	if (nvtab.nameval == NULL) {
		nvtab.nameval = (nameval_t *) emalloc(NVINIT * sizeof(nameval_t));
		nvtab.max = NVINIT;
		nvtab.nval = 0;
	} else if (nvtab.nval >= nvtab.max) {
		nvp = (nameval_t *) erealloc(nvtab.nameval, (NVGROW*nvtab.max) * sizeof(nameval_t));
		nvtab.max *= NVGROW;
		nvtab.nameval = nvp;
	}
	nvtab.nameval[nvtab.nval] = newname;
	return nvtab.nval++;
}

static double time_to_hours(char *time)
{
	int len = strlen(time);
	double seconds = 0.0;
	char tmp[2];

	tmp[1] = '\0';
	switch(len) {
	case 4:
		tmp[0] = *time;
		seconds += strtod(tmp, NULL) * 36000;
		time++;
	case 3:
		tmp[0] = *time;
		seconds += strtod(tmp, NULL) * 3600;
		time++;
		tmp[0] = *time;
		seconds += strtod(tmp, NULL) * 600;
		time++;
		tmp[0] = *time;
		seconds += strtod(tmp, NULL) * 60;
	}
	return seconds/3600;
}

static int get_time(FILE *fin)
{
	nameval_t t;
	char date[DATE_MAX_LEN];
	char start_time[TIME_MAX_LEN];
	char end_time[TIME_MAX_LEN];
	int i;
	int c;

	for (i = 0; (c = fgetc(fin)) != EOF && !isspace(c); i++)
		date[i] = c;
	date[i] = '\0';
	strcpy(t.date, date);

	if (c == EOF)
		return -1;
	i = 0;
	while ((c = fgetc(fin)) != EOF && i < TIME_MAX_LEN && c != '-')
		if (isdigit(c))
			start_time[i++] = c;
	start_time[i] = '\0';

	if (c == EOF)
		return -1;

	i = 0;
	while ((c = fgetc(fin)) != EOF && i < TIME_MAX_LEN && c != '\n') {
		if (isdigit(c))
			end_time[i++] = c;
		else if (isdigit(c) && c != '0' && i == 0) {
			end_time[i++] = '0';
			end_time[i++] = c;
		}
	}
	end_time[i] = '\0';

	t.hours = time_to_hours(end_time) - time_to_hours(start_time);
	addname(t);

	if (c == EOF)
		return -1;

	while (c != '\n' && c != EOF)
		c = fgetc(fin);

	if (c == EOF)
		return -1;
	return 0;
}

static int build_timecard(FILE *fin)
{
	if (get_time(fin))
		return -1;
	return 0;
}

static void get_hours_worked(char *begin_date, char *end_date, char *name)
{
	FILE *fin = fopen(name, "r");

	while (!build_timecard(fin))
		;
	fclose(fin);
}

int main(int argc, char **argv)
{
	int i, max;
	int fd = open(argv[3], O_RDONLY);
	double hours_worked = 0.0;
	int begin = 0;
	int end = 0;

	if (fd < 0)
		eprintf("No such file: %s", argv[3]);
	get_hours_worked(argv[1], argv[2], argv[3]);

	max = nvtab.nval;
	for (i = 0; i < max; i++) {
		if (strcmp(nvtab.nameval[i].date, argv[1]) == 0)
			begin = i;
		if (strcmp(nvtab.nameval[i].date, argv[2]) == 0)
			end = i;
	}
	for (i = begin; i <= end; i++)
		hours_worked += nvtab.nameval[i].hours;
	printf("%.2f\n", hours_worked);
	free(nvtab.nameval);

	return 0;
}
