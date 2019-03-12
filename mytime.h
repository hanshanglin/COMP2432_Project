#ifndef MYTIME_H_INCLUDED
#define MYTIME_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// the period start date "YYYY-MM-DD"
static char *start_date = NULL; 

// the period end date "YYYY-MM-DD"
static char *end_date = NULL;  

// the period start time "hh" (0~24)
static int start_time = -1;  

// the period end time "hh" (0~24)
static int end_time= -1;

int get_day_duration();

int get_time_duration();

typedef struct
{
    int days_since_base; //the day since the base day
    int time_slot;       //if not ,-1
} Date;


int getStartTime();

int getEndTime();

// the duration data of the period eg."4.1~4.3" return 3;
int getdurationDate();

// the duration time of the period eg."11~15" return 4;
int getdurationtime();

Date* newDate(int days_since_base, int time_slot);

bool delDate(Date* ptr);

//check valid date "YYYY-MM-DD"
bool is_valid_date(char* date);

// "YYYY-MM-DD" to store at global char *end_date
bool set_end_date(char* date);

// "YYYY-MM-DD" to store at global char *start_date
bool set_start_date(char* date);

//get date_to_base
int convert_to_base(char* date);

//get date_to_real date
char* convert_to_date(int num);

//check valid time "hh:00"
bool is_valid_time(char* time);

//set hh:00 to start time
bool set_start_time(char* time);

//set hh:00 to end time
bool set_end_time(char* time);

#endif
