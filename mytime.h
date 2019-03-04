#ifndef MYTIME_H_INCLUDED
#define MYTIME_H_INCLUDED

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

#endif