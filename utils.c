#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "efunc.h"
#include "utils.h"

int addname(struct nameval_t newname)
{
	struct nameval_t *nvp;

	if (nvtab.nameval == NULL) {
		nvtab.nameval =	(struct nameval_t *)
				emalloc(NVINIT * sizeof(struct nameval_t));
		nvtab.max = NVINIT;
		nvtab.nval = 0;
	} else if (nvtab.nval >= nvtab.max) {
		nvp = (struct nameval_t *)
			erealloc(nvtab.nameval,
				(NVGROW*nvtab.max) * sizeof(struct nameval_t));
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
	struct nameval_t t;
	char date[DATE_MAX_LEN];
	char start_time[TIME_MAX_LEN];
	char end_time[TIME_MAX_LEN];
	int i;
	int c;

	for (i = 0; (c = fgetc(fin)) != EOF && !isspace(c); i++)
		date[i] = c;
	date[i] = '\0';
	strcpy(t.date, date);

	i = 0;
	while (c != EOF && (c = fgetc(fin)) != EOF && i < TIME_MAX_LEN && c != '-')
		if (isdigit(c)) /* increment i only if c is a digit */
			start_time[i++] = c;
	start_time[i] = '\0';

	i = 0;
	while (c != EOF && (c = fgetc(fin)) != EOF && i < TIME_MAX_LEN && c != '\n')
		if (isdigit(c)) /* increment i only if c is a digit */
			end_time[i++] = c;
	end_time[i] = '\0';

	t.hours = time_to_hours(end_time) - time_to_hours(start_time);
	addname(t);

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

double get_hours_worked(char *begin_date, char *end_date, char *name)
{
	int max, i;
	int begin = 0;
	int end = 0;
	double hours_worked = 0.0;

	FILE *fin = fopen(name, "r");

	while (!build_timecard(fin))
		;
	fclose(fin);

	max = nvtab.nval;
	for (i = 0; i < max; i++) {
		if (!strcmp(nvtab.nameval[i].date, begin_date))
			begin = i;
		if (!strcmp(nvtab.nameval[i].date, end_date))
			end = i;
	}
	for (i = begin; i <= end; i++)
		hours_worked += nvtab.nameval[i].hours;
	free(nvtab.nameval);
	return hours_worked;
}
