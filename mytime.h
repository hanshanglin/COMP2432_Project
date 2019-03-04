#ifndef MYTIME_H_INCLUDED
#define MYTIME_H_INCLUDED

//priority Project > Assignment > Revision > Activity
enum task_type
{
    Project,
    Assignment,
    Revision,
    Activity
};

typedef struct
{
    int days_since_base; //the day since the base day
    int time_slot; //if not ,-1
} Date;

typedef struct
{
    task_type type; // enum type
    char *id; // subjest code
    Date day; // struct Date
    int duration; // number of time slot, need to range
} Record;

char *start_date; // the period start date "YYYY-MM-DD"
char *end_date; // the period end date "YYYY-MM-DD"
int start_time; // the period start time "hh" (0~24)
int end_time; // the period end time "hh" (0~24)

#endif