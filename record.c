#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mytime.h"
#include "record.h"

void add_data(Data_record *block, Record record)
{
    while (block->next != NULL && block->count == SIZE)
        block = block->next;
    if (block->count == SIZE)
    {
        block->next = (Data_record *)malloc(sizeof(Data_record));
        block = block->next;
        block->count = 0;
        block->next = NULL;
        block->ptr = 0;
    }
    block->value[block->count++] = record;
}

Record next(Data_record *block)
{
    while (block->next != NULL && block->ptr == SIZE)
        block = block->next;
    return block->value[block->ptr++];
}

void new_iter(Data_record *block){
    while(block!=NULL){
        block->ptr = 0;
        block = block->next;
    }
}