#ifndef MYTIME_H_INCLUDED
#define MYTIME_H_INCLUDED
#include <stdbool.h>

// the period start date "YYYY-MM-DD"
static char *start_date = NULL; 

// the period end date "YYYY-MM-DD"
static char *end_date = NULL;  

// the period start time "hh" (0~24)
static int start_time = -1;  

// the period end time "hh" (0~24)
static int end_time= -1;    

typedef struct
{
    int days_since_base; //the day since the base day
    int time_slot;       //if not ,-1
} Date;


Date initDate(int days_since_base, int time_slot){
    Date sol = (*Date)malloc(sizeof(Date));
    sol->days_since_base = days_since_base;
    sol->time_slot = time_slot;
    return Date;
};

bool delDate(Date* ptr){
    if(ptr==NULL)return false;
    free(ptr);
    ptr=NULL;
    return true;
}

//check valid date "YYYY-MM-DD"
bool is_valid_date(char* date);

// "YYYY-MM-DD" to store at globol char *end_date 
bool set_end_date(char* date);

// "YYYY-MM-DD" to store at globol char *start_date 
bool set_start_date(char* date);

//get date_to_base
int convert_to_base(char* date);

//get date_to_realdate
char* convert_to_date(int num);

//check valid time "hh:00"
bool is_valid_time(char* time);

//set hh:00 to start time
bool set_start_time(char* time);

//set hh:00 to end time
bool set_end_time(char* time);

#endif
