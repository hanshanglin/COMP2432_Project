#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define  _CRT_SECURE_NO_WARNINGS
#pragma warning(dsiable:4996)

#include "mytime.h"
#include "record.h"
#include "log.h"
#include "FCFS.h"


#define MAX_INPUT_SIZE 128

/*this function is used to handle the date that is out of the range*/
int input_error_handler(int date_in_base) {/*return 0 if error happens, 1 othrewise*/
    if (date_in_base > 0 && date_in_base < getdurationDate())
        return 1;
    else
        return 0;
}

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
    int flag = input_error_handler(days_since_base);/*if out of range, flag = 0; 1 otherwise*/

    int time_slot = -1;/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Assignment, record_id, date, duration);
    if (flag == 0){
        char msg[] = "Date out of range!";
        log_error(record,msg);
        return;/*end*/
    }

    add_data(dataRecord, record);
}

void addProject(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);
    int flag = input_error_handler(days_since_base);/*if out of range, flag = 0; 1 otherwise*/

    int time_slot = -1;/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Project, record_id, date, duration);
    if (flag == 0){
        char msg[] = "Date out of range!";
        log_error(record,msg);
        return;/*end*/
    }
    add_data(dataRecord, record);
}

void addRevision(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get starting date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);
    int flag = input_error_handler(days_since_base);/*if out of range, flag = 0; 1 otherwise*/



    split_ptr = strtok(NULL, delim);/*get time, 4th para*/
    int time_slot = convert_to_timeslot(split_ptr);/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 5th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Revision, record_id, date, duration);
    if (flag == 0){
        char msg[] = "Date out of range!";
        log_error(record,msg);
        return;/*end*/
    }
    add_data(dataRecord, record);
}

void addActivity(Data_record *dataRecord, char *delim, char *split_ptr)  {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get starting date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);
    int flag = input_error_handler(days_since_base);/*if out of range, flag = 0; 1 otherwise*/


    split_ptr = strtok(NULL, delim);/*get time, 4th para*/
    int time_slot = convert_to_timeslot(split_ptr);/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 5th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Activity, record_id, date, duration);
    if (flag == 0){
        char msg[] = "Date out of range!";
        log_error(record,msg);
        return;/*end*/
    }
    add_data(dataRecord, record);

}


int main(void) {

    printf("   ~~WELCOME TO S3~~\n\n");
    printf("Please enter: ");
    char *user_input = malloc(MAX_INPUT_SIZE);
    fgets(user_input, MAX_INPUT_SIZE, stdin);
    user_input[strlen(user_input) - 1] = ' ';

    Data_record *dataRecord = newDataRecord();

    while (strcmp(user_input, "exitS3 ") != 0) {

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

            char line[MAX_INPUT_SIZE];

            split_ptr = strtok(NULL, delim);/*get file name*/

            FILE *fp = fopen("C:\\Users\\incandescentxxc\\desktop\\CodesHQ\\C Language\\OS\\Project\\testcase2.txt",
                             "r");
            //FILE *fp = fopen(split_ptr,"r");
            if (fp == NULL) {
                printf("Could not open file %s", split_ptr);
                return 1;
            }

            while (fgets(line, MAX_INPUT_SIZE, (FILE *) fp) != NULL) {

                char *word = strtok(line, delim);/*get the first word in each line*/

                if (strcmp(word, "addPeriod") == 0)/*if setPeriod more than once, take the last one as real*/
                    setPeriod(delim, word);
                else if (strcmp(word, "addAssignment") == 0)
                    addAssignment(dataRecord, delim, word);
                else if (strcmp(word, "addProject") == 0)
                    addProject(dataRecord, delim, word);
                else if (strcmp(word, "addRevision") == 0)
                    addRevision(dataRecord, delim, word);
                else if (strcmp(word, "addActivity") == 0)
                    addActivity(dataRecord, delim, word);
                else
                    break;
            }
            fclose(fp);


        } else if (strcmp(split_ptr, "runS3") == 0) {
            print_timetable(FCFS(dataRecord));
            /*TODO*/

        } else {
            printf("Wrong input! Please enter an appropriate task!\n");
        }


        printf("Please enter: ");
        fgets(user_input, MAX_INPUT_SIZE, stdin);
        user_input[strlen(user_input) - 1] = ' ';

    }


    free(user_input);
    printf("Byebye~");

}
