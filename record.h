#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

typedef struct 
{
    Record value[SIZE];
    int count=0;
    int ptr=0;
    Data_record* next;
}Data_record;

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
    task_type type; // enum type
    char *id;       // subjest code
    Date day;       // struct Date
    int duration;   // number of time slot, need to range
} Record;


void add_data(Data_record* block, Record record)

Record next(Data_record* block)

#endif