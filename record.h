#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED
#define SIZE 128

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
    enum task_type type; // enum type
    char *id;       // subject code
    Date day;       // struct Date
    int duration;   // number of time slot, need to range
} Record;

typedef struct
{
    Record value[SIZE];
    int count;  //the number of record  in this block
    int ptr;    // the next available record position in record array
    struct Data_record* next;
}Data_record;



void add_data(Data_record* block, Record record);

Record next(Data_record* block);

#endif
