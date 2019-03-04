#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#inlcude "my_time.h"

enum task_type
{Assignment,Project,Revision,Activity};

typedef struct 
{
    int days_since_base;
    int time_slot; //if not ,-1
}Date;

typedef struct 
{
    task_type type;
    char* id;
    Date day;
    int duration;
}Record;

char* start_date;
char* end_date;
int start_time;
int end_time;



