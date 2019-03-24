#ifndef MYTIME_H_INCLUDED
#define MYTIME_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static int day_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

typedef struct {
    int year;
    int month;
    int day;
    int day_in_year;
}date;
int day_in_year(date sol);
bool is_leap_year(int year);



// the period start date "YYYY-MM-DD"
static date start_date ;

// the period end date "YYYY-MM-DD"
static date end_date;

// the period start time "hh" (0~24)
static int start_time = -1;  

// the period end time "hh" (0~24)
static int end_time= -1;

int get_day_duration();

int get_time_duration();

// the timeslot of a real time eg. "15:00" with start time "12:00" return 3;
int convert_to_timeslot(char *ptr);

typedef struct
{
    int days_since_base; //the day since the base day
    int time_slot;       //if not ,-1
} Date;


int getStartTime();

int getEndTime();

void get_start_date(char* buf);

void get_end_date(char* buf);

// the duration data of the period eg."4.1~4.3" return 3;
int getdurationDate();

// the duration time of the period eg."11~15" return 4;
int getdurationtime();

Date* newDate(int days_since_base, int time_slot);

bool delDate(Date* ptr);

//check valid date "YYYY-MM-DD"
bool is_valid_date_format(char *date);

// "YYYY-MM-DD" to store at global char *end_date
bool set_end_date(char* date);

// "YYYY-MM-DD" to store at global char *start_date
bool set_start_date(char* date);

//get date_to_base
int convert_to_base(char* date);

//get date_to_real date
char* convert_to_date(int num,char* target);

//check valid time "hh:00"
bool is_valid_time_format(char *time);

//set hh:00 to start time
bool set_start_time(char* time);

//set hh:00 to end time
bool set_end_time(char* time);

#endif
