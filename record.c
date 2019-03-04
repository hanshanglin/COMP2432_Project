#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "mytime.h"
#include "record.h"

#define SIZE 128
typedef struct 
{
    Record value[SIZE];
    int count=0;
    int ptr=0;
    Data_record* next;
}Data_record;

void add_data(Data_record* block, Record record){
    while(block->next!=null&&block->count==SIZE)
        block=block->next;
    if(block.count==SIZE){
        block->next=(Data_record*)malloc(sizeof(Data_record));
        block=block->next;
    }
    block->value[count++]=record;
}

Record next(Data_record* block){
    while(block->next!=null&&block->ptr==SIZE)
        block=block->next;
    return block->value[ptr++];
}