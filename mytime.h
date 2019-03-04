#ifndef MYTIME_H_INCLUDED
#define MYTIME_H_INCLUDED


typedef struct
{
    int days_since_base; //the day since the base day
    int time_slot;       //if not ,-1
} Date;


//check valid date "YYYY-MM-DD"
bool is_valid_date(char* date)

// "YYYY-MM-DD" to store at globol char *end_date 
bool set_end_date(char* date)

// "YYYY-MM-DD" to store at globol char *start_date 
bool set_start_date(char* date)

//get date_to_base
int convert_to_base(char* date)

//get date_to_realdate
char* convert_to_date(int num)

static char *start_date; // the period start date "YYYY-MM-DD"
static char *end_date;   // the period end date "YYYY-MM-DD"
static int start_time;   // the period start time "hh" (0~24)
static int end_time;     // the period end time "hh" (0~24)

#endif