#include "mytime.h"

void get_start_date(char* buf){
    sprintf(buf,"%04d-%02d-%02d",start_date.year,start_date.month,start_date.day);
}

void get_end_date(char* buf){
    
    sprintf(buf,"%04d-%02d-%02d",end_date.year,end_date.month,end_date.day);
}

int getdurationtime(){
    return end_time-start_time;
}

int getdurationDate(){
    int sol = end_date.day_in_year-start_date.day_in_year;
    for (int i = start_date.year; i < end_date.year; ++i) {
        if(is_leap_year(i)){
            sol+=367;
        }
        else    {
            sol+=366;
        }
    }
    return sol;
};

int convert_to_timeslot(char *ptr){
    assert(is_valid_time_format(ptr));

    return (10 * (ptr[0] - '0') + (ptr[1] - '0')) - start_time;
}

int getdurationTime(){return end_time-start_time;};

int getStartTime(){
    return start_time;
}

int getEndTime(){
    return end_time;
}

int day_in_year(date sol) {
    int day = 0;
    day+=sol.day-1;
    for (int i = 0; i < sol.month; ++i) {
        day += day_in_month[i];
        if (i == 1 && is_leap_year(sol.year))++day;
    }
    return day;
}

// "YYYY-MM-DD" to store at global date start_date
bool set_start_date(char *date)
{
    assert(is_valid_date_format(date));

    start_date.year = strtol(date,NULL,10);
    start_date.month = strtol(date+5,NULL,10);
    start_date.day = strtol(date+8,NULL,10);
    start_date.day_in_year = day_in_year(start_date);
    return true;
}

// "YYYY-MM-DD" to store at global date end_date
bool set_end_date(char *date)
{
    assert(is_valid_date_format(date));

    end_date.year = strtol(date,NULL,10);
    end_date.month = strtol(date+5,NULL,10);
    end_date.day = strtol(date+8,NULL,10);
    end_date.day_in_year = day_in_year(end_date);
    return true;
}

bool is_leap_year(int year) {
    return ((year%4==0) && (year%100!=0)) || year%400==0;
}

//convert a date "YYYY-MM-DD" to an integer, eg. start Date is "2019-01-10", input "2019-01-15" return 5;
int convert_to_base(char *target_date)
{
    assert(is_valid_date_format(target_date));

    date target;

    target.year = strtol(target_date,NULL,10);
    target.month = strtol(target_date+5,NULL,10);
    target.day = strtol(target_date+8,NULL,10);

    if(start_date.year>target.year&&start_date.year>target.month&&start_date.day>target.day)
        return -1;
    int sol = 0;
    for (int i = start_date.year ; i <target.year; ++i)
        sol+=is_leap_year(i)?366:365;
    sol = sol-start_date.day_in_year+day_in_year(target);

    /*TODO test*/
    return sol;
}

//convert an integer to a date "YYYY-MM-DD", eg. start Date is "2019-01-10", input 5 return "2019-01-15";
char *convert_to_date(int num,char* buf)
{
    assert(num >= 0);
    date sol = start_date;
    sol.day=1;
    sol.month=1;
    num = start_date.day_in_year+num;
    while (num>365){
        if (is_leap_year(sol.year)){
            if (num<366) break;
            sol.year++;
            num-=366;
        } else{
            sol.year++;
            num-=365;
        }
    }
    while(num>=day_in_month[sol.month-1]){
        if (sol.month==2 && is_leap_year(sol.year)){
            if(num<29)break;
            num-=(day_in_month[sol.month-1]+1);
            sol.month++;
        }
        num-=day_in_month[sol.month-1];
        sol.month++;
    }
    sol.day+=num;

    //convert to string
    sprintf(target,"%04d-%02d-%02d",sol.year,sol.month,sol.day);
    /*TODO test*/
    return buf;
}

//check valid date "YYYY-MM-DD"
bool is_valid_date_format(char *date)
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
bool is_valid_time_format(char *cur_time)
{
    assert(strlen(cur_time) > 2);

    int current_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return !(current_time < 0 || current_time > 23);
}

//set hh:00 to start time
bool set_start_time(char *cur_time)
{
    assert(is_valid_time_format(cur_time));

    start_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return true;
}

//set hh:00 to end time
bool set_end_time(char *cur_time)
{
    assert(is_valid_time_format(cur_time));

    end_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return true;
}

Date* newDate(int days_since_base, int time_slot){
    Date* sol = (Date*)malloc(sizeof(Date));
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
