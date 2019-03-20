#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define  _CRT_SECURE_NO_WARNINGS
#pragma warning(dsiable:4996)

#include "mytime.h"
#include "record.h"

#define MAX_INPUT_SIZE 128

void setPeriod(char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get the second parameter*/
    set_start_date(split_ptr);/*char */

    split_ptr = strtok(NULL, delim);/*get the third parameter*/
    set_end_date(split_ptr);

    split_ptr = strtok(NULL, delim);/*get the fourth parameter*/
    set_start_time(split_ptr);/*int*/
    split_ptr = strtok(NULL, delim);/*get the fifth parameter*/
    set_end_time(split_ptr);
}

void addAssignment(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;


    split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);
    int time_slot = -1;/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Assignment, record_id, date, duration);
    add_data(dataRecord, record);
}

void addProject(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);
    int time_slot = -1;/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Project, record_id, date, duration);
    add_data(dataRecord, record);
}

void addRevision(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get starting date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);


    split_ptr = strtok(NULL, delim);/*get time, 4th para*/
    int time_slot = convert_to_timeslot(split_ptr);/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 5th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Revision, record_id, date, duration);
    add_data(dataRecord, record);
}

void addActivity(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get starting date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);

    split_ptr = strtok(NULL, delim);/*get time, 4th para*/
    int time_slot = convert_to_timeslot(split_ptr);/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 5th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Activity, record_id, date, duration);
    add_data(dataRecord, record);

}

int main(void) {

    printf("   ~~WELCOME TO S3~~\n\n");
    printf("Please enter: ");
    char *user_input = malloc(MAX_INPUT_SIZE);
    fgets(user_input, MAX_INPUT_SIZE, stdin);

    Data_record *dataRecord = newDataRecord();

    while (strcmp(user_input, "exitS3\n") != 0) {

        /*parsing the string input by user*/
        char delim[] = " ";/*splitting key*/
        char *split_ptr = strtok(user_input, delim);/*get the first word*/
        if (strcmp(split_ptr, "addPeriod") == 0) {
            setPeriod(delim, split_ptr);
        } else if (strcmp(split_ptr, "addAssignment") == 0) {
            addAssignment(dataRecord, delim, split_ptr);
        } else if (strcmp(split_ptr, "addProject") == 0) {
            addProject(dataRecord, delim, split_ptr);
        } else if (strcmp(split_ptr, "addRevision") == 0) {
            addRevision(dataRecord, delim, split_ptr);
        } else if (strcmp(split_ptr, "addActivity") == 0) {
            addActivity(dataRecord, delim, split_ptr);
        } else if (strcmp(split_ptr, "addBatch") == 0) {
            FILE *fp;
            char line[MAX_INPUT_SIZE];

            split_ptr = strtok(NULL, delim);/*get file name*/
            fp = fopen(split_ptr, "r");

            while (fgets(line, MAX_INPUT_SIZE, fp) != NULL) {
                char *word = strtok(line, delim);/*get the first word in each line*/
                if (strcmp(word, "addPeriod") == 0)
                    setPeriod(delim, word);
                else if (strcmp(word, "addAssignment") == 0)
                    addAssignment(dataRecord, delim, word);
                else if (strcmp(word, "addProject") == 0)
                    addAssignment(dataRecord, delim, word);
                else if (strcmp(word, "addRevision") == 0)
                    addRevision(dataRecord, delim, word);
                else if (strcmp(word, "addActivity") == 0)
                    addActivity(dataRecord, delim, word);
                else
                    continue;
            }
            free(line);

        } else if (strcmp(split_ptr, "runS3") == 0) {
            /*TODO*/

        } else {
            printf("Wrong input! Please enter an appropriate task!\n");
        }


        printf("Please enter: ");
        fgets(user_input, MAX_INPUT_SIZE, stdin);

    }


    free(user_input);
    printf("Byebye~");

}
