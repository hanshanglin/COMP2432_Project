#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

typedef struct 
{
    Record value[SIZE];
    int count=0;
    int ptr=0;
    Data_record* next;
}Data_record;

void add_data(Data_record* block, Record record)

Record next(Data_record* block)

#endif