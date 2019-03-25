#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mytime.h"
#include "record.h"

void add_data(Data_record *block, Record* record)
{
    while (block->next != NULL && block->count == SIZE)
        block = block->next;
    if (block->count == SIZE)
    {
        block->next = newDataRecord();
        block = block->next;
    }
    block->value[block->count++] = *record;
}

Record* next(Data_record *block)
{
    while (block->next != NULL && block->ptr == SIZE)
        block = block->next;
    return &(block->value[block->ptr++]);
}

void new_iter(Data_record *block)
{
    while (block != NULL)
    {
        block->ptr = 0;
        block = block->next;
    }
}

Data_record* newDataRecord(void)
{
    Data_record* sol = (Data_record*)malloc(sizeof(Data_record));
    sol->next = NULL;
    sol->count = 0;
    sol->ptr = 0;
    return sol;
}

Record* newRecord(task_type type, char *id, Date *day, int duration)
{
    Record* sol = (Record*)malloc(sizeof(Record));
    sol->type = type;
    sol->id = (char*)malloc((strlen(id)+1)* sizeof(char));
    sprintf(sol->id,"%s",id);
    sol->day = day;
    sol->duration = duration;
    return sol;
}

void delDataRecord(Data_record *ptr)
{
    if (ptr == NULL)
        return;
    for (size_t i = 0; i < ptr->count; i++)
        delRecord(((ptr->value) + i));
    delDataRecord(ptr->next);
    free(ptr);
}

void delRecord(Record *ptr)
{
    if (ptr == NULL)
        return;
    free(ptr);
}
