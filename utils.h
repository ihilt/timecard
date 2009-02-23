#define DATE_MAX_LEN (12)
#define TIME_MAX_LEN (5)

enum { NVINIT = 1, NVGROW = 2 };

struct nameval_t {
	char date[DATE_MAX_LEN];
	double hours;
};

extern int addname(struct nameval_t newname);
extern double get_hours_worked(char *begin_date, char *end_date, char *name);
