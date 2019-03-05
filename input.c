#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define  _CRT_SECURE_NO_WARNINGS
#pragma warning(dsiable:4996)

#include "mytime.h"
#include "record.h"

#define MAX_INPUT_SIZE 128

int main(void) {

    printf("   ~~WELCOME TO S3~~\n\n");
    printf("Please enter: ");
    char *user_input = malloc(MAX_INPUT_SIZE);
    fgets(user_input, MAX_INPUT_SIZE, stdin);

    Data_record *data_record = (Data_record *) malloc(sizeof(Data_record));
    data_record->count = 0;
    data_record->ptr = 0;
    data_record->next = NULL;

    while (strcmp(user_input, "exitS3\n") != 0) {

        /*parsing the string input by user*/
        char delim[] = " ";/*splitting key*/
        char *split_ptr = strtok(user_input, delim);/*get the first word*/
        if (strcmp(split_ptr, "addPeriod") == 0) {
            split_ptr = strtok(NULL, delim);/*get the second parameter*/
            set_start_date(split_ptr);

            split_ptr = strtok(NULL, delim);/*get the third parameter*/
            set_end_date(split_ptr);

            split_ptr = strtok(NULL, delim);/*get the fourth parameter*/
            //TODO
            split_ptr = strtok(NULL, delim);/*get the fivth parameter*/
            //TODO

        } else if (strcmp(split_ptr, "addAssignment") == 0) {
            Record record;
            record.type = Assignment;
            split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
            record.id = split_ptr;

            split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
            record.day.days_since_base;/*TODO*/
            record.day.time_slot = -1;/*Not Applicable*/

            split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
            record.duration = atoi(split_ptr);

            add_data(data_record, record);

        } else if (strcmp(split_ptr, "addProject") == 0) {
            Record record;
            record.type = Project;
            split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
            record.id = split_ptr;

            split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
            record.day.days_since_base;/*TODO*/
            record.day.time_slot = -1;/*Not Applicable*/

            split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
            record.duration = atoi(split_ptr);

            add_data(data_record, record);

        } else if (strcmp(split_ptr, "addRevision") == 0) {
            Record record;
            record.type = Revision;
            split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
            record.id = split_ptr;

            split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
            record.day.days_since_base;/*TODO*/

            split_ptr = strtok(NULL,delim);/*get time, 4th para*/
            record.day.time_slot;/*TODO*/

            split_ptr = strtok(NULL,delim);/*get duration, 5th para*/
            record.duration = atoi(split_ptr);

            add_data(data_record,record);


        } else if (strcmp(split_ptr, "addActivity") == 0) {
            Record record;
            record.type = Activity;
            split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
            record.id = split_ptr;

            split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
            record.day.days_since_base;/*TODO*/

            split_ptr = strtok(NULL,delim);/*get time, 4th para*/
            record.day.time_slot;/*TODO*/

            split_ptr = strtok(NULL,delim);/*get duration, 5th para*/
            record.duration = atoi(split_ptr);

            add_data(data_record,record);

        } else if (strcmp(split_ptr, "addBatch") == 0) {
            /*TODO*/

        } else if (strcmp(split_ptr, "runS3") == 0) {
            /*TODO*/

        } else {
            printf("Wrong input! Please enter an appropriate task!\n");
        }



        printf("Please enter: ");
        fgets(user_input, MAX_INPUT_SIZE, stdin);

    }
    //following codes is used to test, success.
    /*
    printf("%s\n",data_record->value[0].id);
    printf("%d\n",data_record->value[0].day.time_slot);
    printf("%d\n",data_record->value[0].duration);*/

    free(user_input);
    printf("Byebye~");

}
