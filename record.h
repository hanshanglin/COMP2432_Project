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

typedef struct Record
{
    enum task_type type; // enum type
    char *id;       // subject code
    Date day;       // struct Date
    int duration;   // number of time slot, need to range
} Record;

typedef struct Data_record
{
    Record value[SIZE];
    int count;  //the number of record  in this block
    int ptr;    // the next available record position in record array
    struct Data_record* next;
} Data_record;



void add_data(Data_record* block, Record record);

Record* next(Data_record* block);

void new_iter(Data_record *block);

Record* newRecord(task_type type, char* id, Date day, int duration);

void delDataRecord(Data_record* ptr);

void delRecord(Record* ptr);



#endif
