#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

#define  _CRT_SECURE_NO_WARNINGS
#pragma warning(dsiable:4996)

#include "mytime.h"
#include "record.h"
#include "log.h"
#include "FCFS.h"
#include "DDL.h"
#include "Priority.h"


#define MAX_INPUT_SIZE 128

/*this function is used to handle the date that is out of the range*/
int input_error_handler(int date_in_base, int timeslot) {
    if ((date_in_base >= 0 && date_in_base < getdurationDate()) && (timeslot >= 0 && timeslot < getdurationtime()))
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
    if (getdurationtime() < 0)
        printf("Wrong period input, please enter a new time period!\n");
}

void addAssignment(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;


    split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);
    int flag = input_error_handler(days_since_base, 0);/*if out of range, flag = 0; 1 otherwise*/

    int time_slot = -1;/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Assignment, record_id, date, duration);
    if (flag == 0) {
        char msg[] = "Date out of range!";

        log_error(record, msg);
        return;
    }

    add_data(dataRecord, record);
}

void addProject(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get due date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);
    int flag = input_error_handler(days_since_base, 0);/*if out of range, flag = 0; 1 otherwise*/

    int time_slot = -1;/*Time slot Not Applicable Here*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 4th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Project, record_id, date, duration);
    if (flag == 0) {
        char msg[] = "Date out of range!";
        log_error(record, msg);
        return;
    }
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

    int flag = input_error_handler(days_since_base, time_slot);/*if out of range, flag = 0; 1 otherwise*/
    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 5th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Revision, record_id, date, duration);
    if (flag == 0) {
        char msg[] = "Date or time out of range!";

        log_error(record, msg);

        return;
    }
    add_data(dataRecord, record);
}

void addActivity(Data_record *dataRecord, char *delim, char *split_ptr) {
    split_ptr = strtok(NULL, delim);/*get subject code, 2nd para*/
    char *record_id = split_ptr;

    split_ptr = strtok(NULL, delim);/*get starting date, 3rd para*/
    char *due_date = split_ptr;
    int days_since_base = convert_to_base(due_date);


    split_ptr = strtok(NULL, delim);/*get time, 4th para*/
    int time_slot = convert_to_timeslot(split_ptr);
    int flag = input_error_handler(days_since_base, time_slot);/*if out of range, flag = 0; 1 otherwise*/

    Date *date = newDate(days_since_base, time_slot);

    split_ptr = strtok(NULL, delim);/*get duration, 5th para*/
    int duration = atoi(split_ptr);

    Record *record = newRecord(Activity, record_id, date, duration);


    if (flag == 0) {

        char msg[] = "Date out of range!";

        log_error(record, msg);

        return;
    }

    add_data(dataRecord, record);

}


int main(void) {
    /*pipe creation*/
    int fd1[2];/*parent writes, child reads*/
    if (pipe(fd1) < 0) {
        printf("Pipe creation fails\n");
        exit(1);
    }

    if (fork() == 0) {
        /*child process*/
        /*this child used to store the input information*/
        close(fd1[1]);

        Data_record *dataRecord = newDataRecord();

        char *user_input = malloc(MAX_INPUT_SIZE);
        while (true) {
            read(fd1[0], user_input, MAX_INPUT_SIZE);/*read once the pipe is empty*/

            if (strcmp(user_input, " ") == 0) {
                printf("Wrong input!Please enter an appropriate task!\n");
                continue;
            }
            if (strcmp(user_input, "exitS3") != 0) {    /*parsing the string input by user*/

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

                    FILE *fp = fopen(split_ptr, "r");
                    if (fp == NULL) {
                        printf("Could not open file %s", split_ptr);
                        /*write(fd2[1], "cont", 4);*/
                        continue;
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
                    /*schedule process is operating in the son process of child process*/
                    /*create pipe for the communication between son and child of child processes*/
                    if (fork() == 0) {
                        /*grandchild process*/
                        split_ptr = strtok(NULL, delim);/*get the name of algorithm, 2nd para*/
                        char *algorithm_name = split_ptr;

                        split_ptr = strtok(NULL, delim);/*get output file name, 3rd para*/
                        char *output_file_name = split_ptr;
                        if (strcmp(algorithm_name, "Priority") == 0) {
                            print_timetable(Priority(dataRecord, 0), output_file_name);
                            print_report(output_file_name);
                        } else if (strcmp(algorithm_name, "Deadline") == 0) {
                            print_timetable(Priority(dataRecord, 1), output_file_name);
                            print_report(output_file_name);
                        } else if (strcmp(algorithm_name, "FCFS") == 0) {
                            print_timetable(FCFS(dataRecord), output_file_name);
                            print_report(output_file_name);
                        } else if (strcmp(algorithm_name, "SDDL") == 0) {
                            print_timetable(DDL(dataRecord), output_file_name);
                            print_report(output_file_name);
                        }

                        exit(0);
                    } else {
                        /*child process*/
                        wait(NULL);/*synchronized method, wating for the grandchild ends*/
                    }

                } else {
                    printf("Wrong input! Please enter an appropriate task!\n");
                }


            } else {/*when user enter exitS3*/
                break;
            }
        }

        close(fd1[0]);

        exit(0);
    } else {
        /*parent process*/
        close(fd1[0]);
        printf("   ~~WELCOME TO S3~~\n\n");

        char *par_user_input = malloc(MAX_INPUT_SIZE);
        printf("Please enter: ");
        fgets(par_user_input, MAX_INPUT_SIZE, stdin);
        par_user_input[strlen(par_user_input) - 1] = ' ';
        while (strcmp(par_user_input, "exitS3 ") != 0) {
            write(fd1[1], par_user_input, MAX_INPUT_SIZE);/*pass the user input to the child*/
            sleep(1);
            printf("Please enter: ");
            fgets(par_user_input, MAX_INPUT_SIZE, stdin);
            par_user_input[strlen(par_user_input) - 1] = ' ';
        }
        write(fd1[1], "exitS3", MAX_INPUT_SIZE);/*inform the children of ending*/
        free(par_user_input);
        wait(NULL);/*wait for child s ending*/

        printf("Byebye~\n");
        close(fd1[1]);


    }

}
