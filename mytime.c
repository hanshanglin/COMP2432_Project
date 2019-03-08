#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "mytime.h"

// "YYYY-MM-DD" to store at globol char *start_date
bool set_start_date(char *date)
{
    assert(is_valid_date(date));

    start_date = date;
    return true;
}

// "YYYY-MM-DD" to store at globol char *end_date
bool set_end_date(char *date)
{
    assert(is_valid_date(date));

    end_date = date;
    return true;
}

//get date_to_base
int convert_to_base(char *date)
{
    assert(is_valid_date(date) && start_date != NULL);

    //todomark
    return 0;
}

//get date_to_realdate
char *convert_to_date(int num)
{

    //todomark
    return NULL;
}

//check valid date "YYYY-MM-DD"
bool is_valid_date(char *date)
{
    if (strlen(date) < 10)
        return false;
    for (size_t i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7) // '-'
            continue;
        if ('9' < date[i] || '0' > date[i])
            return false;
    }
    return true;
}

//check valid time "hh:00"
bool is_valid_time(char *cur_time)
{
    assert(strlen(cur_time) > 2);

    int current_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return (current_time < 0 || current_time > 23);
}

//set hh:00 to start time
bool set_start_time(char *cur_time)
{
    if (assert(is_valid_time(cur_time)))
        return false;

    start_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return true;
}

//set hh:00 to end time
bool set_end_time(char *cur_time)
{
    if (assert(is_valid_time(cur_time)))
        return false;

    end_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return true;
}

Date* newDate(int days_since_base, int time_slot){
    Date* sol = (*Date)malloc(sizeof(Date));
    sol->days_since_base = days_since_base;
    sol->time_slot = time_slot;
    return sol;
};

bool delDate(Date* ptr){
    if(ptr==NULL)return false;
    free(ptr);
    //ptr=NULL;
    return true;
}
