#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

#define  _CRT_SECURE_NO_WARNINGS
#pragma warning(dsiable:4996)

#define SIZE 128
#define MAX_INPUT_SIZE 128

typedef struct {
    int year;
    int month;
    int day;
    int day_in_year;
} date_translate;

typedef struct {
    int days_since_base; //the day since the base day
    int time_slot;       //if not ,-1
} Date;

//priority Project > Assignment > Revision > Activity
typedef enum {
    Project = 4,
    Assignment = 3,
    Revision = 2,
    Activity = 1
} task_type;

typedef struct Record {
    task_type type; // enum type
    char *id;       // subject code
    Date *day;       // struct pointer Date
    int duration;   // number of time slot, need to range
    int excuted;    //number of time slots excuted
} Record;

typedef struct Data_record {
    Record value[SIZE];
    int count;  //the number of record  in this block
    int ptr;    // the next available record position in record array
    struct Data_record *next;
} Data_record;

struct Node {
    Record *task;
    struct Node *next;
};

struct Node *newNode(Record *task);


static int day_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static int log_id = 1;
static char *algo_name = NULL;
static FILE *log_file = NULL;
static FILE *err_file = NULL;
static char _log_date_buf[11];
static int acc_count = 0;
static int rej_count = 0;
static int slot_occupied = 0;
static int completed_counts[4], total_counts[4];
static int completed_time[4], total_time[4];
static Data_record *unfinished_tasks = NULL;
static Data_record *rejected_tasks = NULL;

// the period start date "YYYY-MM-DD"
static date_translate start_date;

// the period end date "YYYY-MM-DD"
static date_translate end_date;

// the period start time "hh" (0~24)
static int start_time = -1;

// the period end time "hh" (0~24)
static int end_time = -1;

int compare(Record *t1, Record *t2, int mode);

int insert(struct Node **head_ref, struct Node *new_node, int mode);

const char *type_to_command(task_type type);

void set_algorithm_name(const char *algorithm_name);

void log_start(void);

void log_log(Record *record, bool accepted);

void log_stop(void);

void print_timetable(Record **table, char *filename);

void print_report(char *filename);

void log_error(Record *record, char *msg);

int getDDL(Record *);

Data_record *newDataRecord(void);

void add_data(Data_record *block, Record *record);

Record *next(Data_record *block);

void new_iter(Data_record *block);

Record *newRecord(task_type type, char *id, Date *day, int duration);

void delDataRecord(Data_record *ptr);

void delRecord(Record *ptr);

Record **SDDL(Data_record *input);

Record **FCFS(Data_record *input);

Record **Priority(Data_record *input, int mode);

int day_in_year(date_translate sol);

bool is_leap_year(int year);

// the timeslot of a real time eg. "15:00" with start time "12:00" return 3;
int convert_to_timeslot(char *ptr);

int get_start_time();

int get_end_time();

void get_start_date(char *buf);

void get_end_date(char *buf);

// the duration data of the period eg."4.1~4.3" return 3;
int get_duration_date();

// the duration time of the period eg."11~15" return 4;
int get_duration_time();

Date *newDate(int days_since_base, int time_slot);


//check valid date "YYYY-MM-DD"
bool is_valid_date_format(char *date);

// "YYYY-MM-DD" to store at global char *end_date
bool set_end_date(char *date);

// "YYYY-MM-DD" to store at global char *start_date
bool set_start_date(char *date);

//get date_to_base
int convert_to_base(char *date);

//get date_to_real date
char *convert_to_date(int num, char *buf);

//check valid time "hh:00"
bool is_valid_time_format(char *time);

//set hh:00 to start time
bool set_start_time(char *time);

//set hh:00 to end time
bool set_end_time(char *time);

void get_start_date(char *buf) {
    sprintf(buf, "%04d-%02d-%02d", start_date.year, start_date.month, start_date.day);
}

void get_end_date(char *buf) {

    sprintf(buf, "%04d-%02d-%02d", end_date.year, end_date.month, end_date.day);
}

int get_duration_time() {
    return end_time - start_time;
}

int get_duration_date() {
    int sol = end_date.day_in_year - start_date.day_in_year;
    for (int i = start_date.year; i < end_date.year; ++i) {
        if (is_leap_year(i)) {
            sol += 367;
        } else {
            sol += 366;
        }
    }
    return sol + 1;
};

int convert_to_timeslot(char *ptr) {
    assert(is_valid_time_format(ptr));

    return (10 * (ptr[0] - '0') + (ptr[1] - '0')) - start_time;
}

int get_start_time() {
    return start_time;
}

int get_end_time() {
    return end_time;
}

int day_in_year(date_translate sol) {
    int day = 0;
    day += sol.day - 1;
    for (int i = 0; i < sol.month - 1; ++i) {
        day += day_in_month[i];
        if (i == 1 && is_leap_year(sol.year))++day;
    }
    return day;
}

// "YYYY-MM-DD" to store at global date start_date
bool set_start_date(char *date) {
    assert(is_valid_date_format(date));

    start_date.year = (int)strtol(date, NULL, 10);
    start_date.month = (int)strtol(date + 5, NULL, 10);
    start_date.day = (int)strtol(date + 8, NULL, 10);
    start_date.day_in_year = day_in_year(start_date);
    return true;
}

// "YYYY-MM-DD" to store at global date end_date
bool set_end_date(char *date) {
    assert(is_valid_date_format(date));

    end_date.year = (int)strtol(date, NULL, 10);
    end_date.month = (int)strtol(date + 5, NULL, 10);
    end_date.day = (int)strtol(date + 8, NULL, 10);
    end_date.day_in_year = day_in_year(end_date);
    return true;
}

bool is_leap_year(int year) {
    return ((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0;
}

//convert a date "YYYY-MM-DD" to an integer, eg. start Date is "2019-01-10", input "2019-01-15" return 5;
int convert_to_base(char *target_date) {
    assert(is_valid_date_format(target_date));

    date_translate target;

    target.year = (int)strtol(target_date, NULL, 10);
    target.month = (int)strtol(target_date + 5, NULL, 10);
    target.day = (int)strtol(target_date + 8, NULL, 10);


    int sol = 0;
    if (target.year < start_date.year ||
        (target.year == start_date.year && day_in_year(target) < start_date.day_in_year)) {
        for (int i = target.year; i < start_date.year; ++i) {
            sol += is_leap_year(i) ? 366 : 365;
        }
        sol = -sol - start_date.day_in_year + day_in_year(target);
    } else {
        for (int i = start_date.year; i < target.year; ++i)
            sol += is_leap_year(i) ? 366 : 365;
        sol = sol - start_date.day_in_year + day_in_year(target);
    }
    return sol;
}

//convert an integer to a date "YYYY-MM-DD", eg. start Date is "2019-01-10", input 5 return "2019-01-15";
char *convert_to_date(int num, char *buf) {
    date_translate sol = start_date;
    sol.day = 1;
    sol.month = 1;
    num = start_date.day_in_year + num;
    while (num < 0) {
        if (is_leap_year(sol.year - 1)) {
            sol.year--;
            num += 366;
        } else {
            sol.year--;
            num += 365;
        }
    }
    while (num > 365) {
        if (is_leap_year(sol.year)) {
            if (num < 366) break;
            sol.year++;
            num -= 366;
        } else {
            sol.year++;
            num -= 365;
        }
    }
    while (num >= day_in_month[sol.month - 1]) {
        if (sol.month == 2 && is_leap_year(sol.year)) {
            if (num < 29)break;
            num -= (day_in_month[sol.month - 1] + 1);
            sol.month++;
            continue;
        }
        num -= day_in_month[sol.month - 1];
        sol.month++;
    }
    sol.day += num;

    //convert to string
    sprintf(buf, "%04d-%02d-%02d", sol.year, sol.month, sol.day);
    return buf;
}

//check valid date "YYYY-MM-DD"
bool is_valid_date_format(char *date) {
    if (strlen(date) < 10)
        return false;
    for (size_t i = 0; i < 10; i++) {
        if (i == 4 || i == 7) // '-'
            continue;
        if ('9' < date[i] || '0' > date[i])
            return false;
    }
    return true;
}

//check valid time "hh:00"
bool is_valid_time_format(char *cur_time) {

    assert(strlen(cur_time) > 2);

    int current_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return !(current_time < 0 || current_time > 23);
}

//set hh:00 to start time
bool set_start_time(char *cur_time) {
    assert(is_valid_time_format(cur_time));

    start_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return true;
}

//set hh:00 to end time
bool set_end_time(char *cur_time) {
    assert(is_valid_time_format(cur_time));

    end_time = 10 * (cur_time[0] - '0') + (cur_time[1] - '0');
    return true;
}

Date *newDate(int days_since_base, int time_slot) {
    Date *sol = (Date *) malloc(sizeof(Date));
    sol->days_since_base = days_since_base;
    sol->time_slot = time_slot;
    return sol;
};

/*this function is used to handle the date that is out of the range*/
int input_error_handler(int date_in_base, int timeslot) {
    if ((date_in_base >= 0 && date_in_base < get_duration_date()) && (timeslot >= 0 && timeslot < get_duration_time()))
        return 1;
    else
        return 0;
}

struct Node *newNode(Record *task) {
    struct Node *sol = (struct Node *) malloc(sizeof(struct Node));
    sol->next = NULL;
    sol->task = task;
    return sol;
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
    if (get_duration_time() < 0)
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

const char *type_to_command(task_type type) {
    switch (type) {
        case Project:
            return "addProject";
        case Assignment:
            return "addAssignment";
        case Revision:
            return "addRevision";
        case Activity:
            return "addActivity";
    }
    return "nope";
}

void set_algorithm_name(const char *algorithm_name) {
    algo_name = (char *) malloc(strlen(algorithm_name) + 1);
    strcpy(algo_name, algorithm_name);
}

void add_data(Data_record *block, Record *record) {
    while (block->next != NULL && block->count == SIZE)
        block = block->next;
    if (block->count == SIZE) {
        block->next = newDataRecord();
        block = block->next;
    }
    block->value[block->count++] = *record;
}

Record *next(Data_record *block) {
    while (block->next != NULL && block->ptr == SIZE)
        block = block->next;
    return block->ptr == block->count ? NULL : &(block->value[block->ptr++]);
}

void new_iter(Data_record *block) {
    while (block != NULL) {
        block->ptr = 0;
        block = block->next;
    }
}

Data_record *newDataRecord(void) {
    Data_record *sol = (Data_record *) malloc(sizeof(Data_record));
    sol->next = NULL;
    sol->count = 0;
    sol->ptr = 0;
    return sol;
}

int getDDL(Record *cur) {
    if (cur == NULL)return -1;
    else
        return cur->day->days_since_base * get_duration_time() +
               (cur->day->time_slot == -1 ? get_duration_time() : cur->day->time_slot);
}

Record *newRecord(task_type type, char *id, Date *day, int duration) {
    Record *sol = (Record *) malloc(sizeof(Record));
    sol->type = type;
    sol->id = (char *) malloc((strlen(id) + 1) * sizeof(char));
    sprintf(sol->id, "%s", id);
    sol->day = day;
    sol->duration = duration;
    sol->excuted = 0;
    return sol;
}

void delDataRecord(Data_record *ptr) {
    if (ptr == NULL)
        return;
    for (size_t i = 0; i < ptr->count; i++)
        delRecord(((ptr->value) + i));
    delDataRecord(ptr->next);
    free(ptr);
}

void delRecord(Record *ptr) {
    if (ptr == NULL)
        return;
    free(ptr);
}

void log_start() {
    assert(algo_name != NULL);
    char *filename = (char *) malloc(strlen(algo_name) + 8);
    sprintf(filename, "S3_%s.log", algo_name);
    log_file = fopen(filename, "w");
    free(filename);

    fprintf(log_file, "****Log File - %s***\n", algo_name);
    fprintf(log_file, "ID\t%-54s\tAccepted/Rejected\n", "Event");
    fprintf(log_file, "==================================================================================\n");

    log_id = 1;
    acc_count = 0;
    rej_count = 0;
    slot_occupied = 0;
    unfinished_tasks = newDataRecord();
    rejected_tasks = newDataRecord();
}

void log_log(Record *record, bool accepted) {
    fprintf(log_file, "%04d\t%s %s %s ", log_id++, type_to_command(record->type), record->id,
            convert_to_date(record->day->days_since_base, _log_date_buf));
    if (record->type == Assignment || record->type == Project)
        fprintf(log_file, "%d", record->duration);
    else
        fprintf(log_file, "%02d:00 %d", record->day->time_slot + get_start_time(), record->duration);

    int pad = strlen(record->id) + 10 + 4 + (record->type == Assignment ? 13 : (record->type == Project ? 10 : 11 + 6));
    pad = 54 - pad;
    fprintf(log_file, "%*c", pad, '\t');

    fprintf(log_file, "%s\n", accepted ? "Accepted" : "         Rejected");

    accepted ? acc_count++ : rej_count++;
    if (!accepted)
        add_data(rejected_tasks, record);
}

void log_error(Record *record, char *msg) {
    err_file = fopen("S3_error.log", "a");
    fprintf(err_file, "[Error] %s <%s %s %s ", msg, type_to_command(record->type), record->id,
            convert_to_date(record->day->days_since_base, _log_date_buf));
    if (record->type == Assignment || record->type == Project) {
        fprintf(err_file, "%d>\n", record->duration);
    } else {
        fprintf(err_file, "%02d:00 %d>\n", record->day->time_slot + get_start_time(), record->duration);
    }
    fclose(err_file);
}

void log_stop() {
    fclose(log_file);
}

void print_timetable(Record **table, char *filename) {
    FILE *out = fopen(filename, "w");
    int width = get_end_time() - get_start_time();
    char *buf = _log_date_buf;
    get_start_date(buf);
    printf("Alice Timetable\nPeriod: %s", buf);
    fprintf(out, "Alice Timetable\nPeriod: %s", buf);
    get_end_date(buf);
    printf(" to %s\nAlgorithm used: %s\n", buf, algo_name);
    fprintf(out, " to %s\nAlgorithm used: %s\n", buf, algo_name);

    printf("Date      ");
    fprintf(out, "Date      ");
    for (int i = get_start_time(); i < get_end_time(); i++)
        printf("\t%02d:00       ", i), fprintf(out, "\t%02d:00       ", i);
    printf("\n");
    fprintf(out, "\n");

    char *str;
    get_start_date(buf);
    for (int i = 0; i < get_duration_date(); i++) {
        convert_to_date(i, buf);
        printf("%s", buf);
        fprintf(out, "%s", buf);
        for (int j = 0; j < width; j++) {
            if (table[i * width + j] == NULL) {
                str = "N/A";
            } else {
                str = table[i * width + j]->id;
                slot_occupied++;
                if (table[i * width + j]->excuted > -1) {
                    int type = table[i * width + j]->type - 1;
                    total_time[type] += table[i * width + j]->duration;
                    completed_time[type] += table[i * width + j]->excuted;
                    if (table[i * width + j]->duration == table[i * width + j]->excuted)
                        completed_counts[type]++;
                    else
                        add_data(unfinished_tasks, table[i * width + j]);
                    table[i * width + j]->excuted = -1;
                    total_counts[type]++;
                }
            }
            printf("\t%-12s", str);
            fprintf(out, "\t%-12s", str);
        }
        printf("\n");
        fprintf(out, "\n");
    }
    fclose(out);
}

void print_report(char* filename){
    FILE* out=fopen(filename,"a");
    printf("***Summary Report***\nAlgorithm used: %s\n",algo_name);
    printf("There are %d requests.\nNumber of request accepted: %d\nNumber of request rejected: %d\n",acc_count+rej_count,acc_count,rej_count);
    printf("Number of time slots used: %d (%.2f%%)\n",slot_occupied,1e2*slot_occupied / ((getEndTime()-getStartTime())*getdurationDate()));

    fprintf(out,"***Summary Report***\nAlgorithm used: %s\n",algo_name);
    fprintf(out,"There are %d requests.\nNumber of request accepted: %d\nNumber of request rejected: %d\n",acc_count+rej_count,acc_count,rej_count);
    fprintf(out,"Number of time slots used: %d (%.2f%%)\n",slot_occupied,1e2*slot_occupied / ((getEndTime()-getStartTime())*getdurationDate()));


    int completed_count=0,time_completed=0,time_total=0;
    float percent1[5],percent2[5];
    for(int i=0;i<4;i++){
	completed_count+=completed_counts[i];
	time_completed+=completed_time[i];
	time_total+=total_time[i];
	if(total_counts[i]) percent1[i]=1e2*completed_counts[i]/total_counts[i]; else percent1[i]=0;
	if(total_time[i]) percent2[i]=1e2*completed_time[i]/total_time[i]; else percent2[i]=0;
    }
    if(acc_count) percent1[4]=1e2*completed_count/acc_count; else percent1[4]=0;
    if(time_total) percent2[4]=1e2*time_completed/time_total; else percent2[4]=0;

    printf("\nProject:    %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[3],total_counts[3],percent1[3],completed_time[3],total_time[3],percent2[3]);
    printf("\nAssignment: %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[2],total_counts[2],percent1[2],completed_time[2],total_time[2],percent2[2]);
    printf("\nRevision:   %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[1],total_counts[1],percent1[1],completed_time[1],total_time[1],percent2[1]);
    printf("\nActivity:   %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[0],total_counts[0],percent1[0],completed_time[0],total_time[0],percent2[0]);
    
    fprintf(out,"\nProject:    %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[3],total_counts[3],percent1[3],completed_time[3],total_time[3],percent2[3]);
    fprintf(out,"\nAssignment: %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[2],total_counts[2],percent1[2],completed_time[2],total_time[2],percent2[2]);
    fprintf(out,"\nRevision:   %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[1],total_counts[1],percent1[1],completed_time[1],total_time[1],percent2[1]);
    fprintf(out,"\nActivity:   %d out of %d completely arranged (%.2f%%), %d out of %d hours of work (%.2f%%)",
	completed_counts[0],total_counts[0],percent1[0],completed_time[0],total_time[0],percent2[0]);


    printf("\n\nTotal:      %d out of %d tasks completely arranged (%.2f%%)\n",completed_count,acc_count,percent1[4]);
    printf("            %d out of %d hours arranged (%.2f%%)\n",time_completed,time_total,percent2[4]);
    
    fprintf(out,"\n\nTotal:      %d out of %d tasks completely arranged (%.2f%%)\n",completed_count,acc_count,percent1[4]);
    fprintf(out,"            %d out of %d hours arranged (%.2f%%)\n",time_completed,time_total,percent2[4]);
    
    if(rejected_tasks->count){
        printf("\nThe following task(s) will be rejected:\n");
        fprintf(out,"\nThe following task(s) will be rejected:\n");
        new_iter(rejected_tasks);
        for(Record* rec=next(rejected_tasks);rec!=NULL;rec=next(rejected_tasks))
            printf("%s %s\n",type_to_command(rec->type),rec->id),fprintf(out,"%s %s\n",type_to_command(rec->type),rec->id);
    }

    if(unfinished_tasks->count){
        printf("\nThe following task(s) will not be completely done:\n");
        fprintf(out,"\nThe following task(s) will not be completely done:\n");
        new_iter(unfinished_tasks);
        for(Record* rec=next(unfinished_tasks);rec!=NULL;rec=next(unfinished_tasks))
            printf("%s %s\n",type_to_command(rec->type),rec->id),fprintf(out,"%s %s\n",type_to_command(rec->type),rec->id);
    }

    printf("\n");
    fprintf(out,"\n");

    fclose(out);
    free(rejected_tasks);
    free(unfinished_tasks);
}

int compare(Record *t1, Record *t2, int mode) {
    //return 1 if t1 has higher priority
    //return -1 if t1 has lower priority
    //return 0 if t1 has the same priority with t2
    //mode 0: priority, mode 1: DDL
    if (mode == 0) {
        task_type type1 = t1->type;
        task_type type2 = t2->type;
        if (type1 > type2) return 1;
        else if (type1 == type2) return 0;
        else return -1;
    } else {
        task_type type1 = t1->type;
        task_type type2 = t2->type;
        if (type1 <= 2 && type2 <= 2) {
            if (getDDL(t1) < getDDL(t2)) return 1;
            else if (getDDL(t1) == getDDL(t2)) return 0;
            else return -1;
        } else if (type1 >= 3 && type2 >= 3) return 0;
        else {
            if (type1 <= 2) return 1;
            else return -1;
        }
    }
    return -2;//-2 means an error has occur
}

int insert(struct Node **head_ref, struct Node *new_node, int mode) {//mode 0: priority, mdoe 1: DDL
    struct Node *current;
    if (*head_ref == NULL || (compare((*head_ref)->task, new_node->task, mode) == -1)) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    } else {
        current = *head_ref;
        while (current->next != NULL && (compare(current->next->task, new_node->task, mode) == 1 ||
                                         compare(current->next->task, new_node->task, mode) == 0)) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
    return 0;


}

Record **FCFS(Data_record *input) {
    set_algorithm_name("FCFS");
    log_start();

    int accepted = 0;
    int rejected = 0;

    int period = get_duration_date();
    int slotsPerDay = get_duration_time();

    Record **table = malloc(sizeof(Record *) * period * slotsPerDay);
    Record **rejectedList = malloc(sizeof(Record *) * period * slotsPerDay);
    memset(table, 0, sizeof(Record *) * period * slotsPerDay);

    new_iter(input);
    Record *curTask = next(input);
    char *course;
    int duration;
    Date *DDL;
    int intDDL;
    int assignedSlot = 0;
    int assignPA = 0;
    Date *dateAndTime;
    while (curTask != NULL) {
        task_type type = curTask->type;
        switch (type) {
            case Project:
                while (table[assignPA] != NULL) assignPA++;
                intDDL = getDDL(curTask);
                if (assignPA >= intDDL) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                duration = curTask->duration;
                DDL = curTask->day;
                if (assignPA == slotsPerDay * period) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                } else accepted++;
                log_log(curTask, true);
                for (int i = 0; i < duration; i++) {
                    table[assignPA++] = curTask;
                    curTask->excuted++;
                    if (assignPA >= intDDL) break;
                    assignedSlot++;
                    while (table[assignPA] != NULL) assignPA++;
                    if (assignPA >= slotsPerDay * period)
                        return table;
                }

                break;

            case Assignment:
                while (table[assignPA] != NULL) assignPA++;
                intDDL = getDDL(curTask);
                if (assignPA >= intDDL) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                DDL = curTask->day;
                duration = curTask->duration;
                if (assignPA == slotsPerDay * period) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                } else accepted++;
                log_log(curTask, true);
                for (int i = 0; i < duration; i++) {
                    table[assignPA++] = curTask;
                    curTask->excuted++;
                    if (assignPA >= intDDL) break;
                    assignedSlot++;
                    while (table[assignPA] != NULL) assignPA++;
                    if (assignPA >= slotsPerDay * period)
                        return table;
                }
                break;

            case Revision:
                dateAndTime = curTask->day;
                duration = curTask->duration;
                int dayR = dateAndTime->days_since_base;
                int startSlotR = dateAndTime->time_slot;
                int startInTableR = dayR * slotsPerDay + startSlotR;
                int ableR = 1;
                if (startSlotR + duration > slotsPerDay)
                    ableR = 0;
                else {
                    for (int i = 0; i < duration; i++) {
                        if (table[startInTableR + i] != NULL) {
                            ableR = 0;
                            break;
                        }
                    }
                }
                if (ableR == 0) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                accepted++;
                log_log(curTask, true);
                int assignR = startInTableR;
                for (int i = 0; i < duration; i++) {
                    table[assignR] = curTask;
                    curTask->excuted++;
                    assignedSlot++;
                    assignR++;
                    if (assignedSlot > slotsPerDay * period)
                        return table;
                }
                break;

            case Activity:
                course = curTask->id;
                dateAndTime = curTask->day;
                duration = curTask->duration;
                int dayA = dateAndTime->days_since_base;
                int startSlotA = dateAndTime->time_slot;
                int startInTableA = dayA * slotsPerDay + startSlotA;
                int ableA = 1;
                if (startSlotA + duration > slotsPerDay)
                    ableA = 0;
                else {
                    for (int i = 0; i < duration; i++) {
                        if (table[startInTableA + i] != NULL) {
                            ableA = 0;
                            break;
                        }
                    }
                }

                if (ableA == 0) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                accepted++;
                log_log(curTask, true);
                int assignA = startInTableA;
                for (int i = 0; i < duration; i++) {
                    table[assignA] = curTask;
                    curTask->excuted++;
                    assignA++;
                    assignedSlot++;
                    if (assignedSlot > slotsPerDay * period)
                        return table;
                }
                break;

        }
        curTask = next(input);
    }
    log_stop();

    return table;
}

Record **Priority(Data_record *input, int mode) {
    //mode 0 is priority, mode 1 is DDL

    set_algorithm_name(mode == 0 ? "Priority" : "Deadline");
    log_start();

    int accepted = 0;
    int rejected = 0;

    int period = get_duration_date();
    int slotsPerDay = get_duration_time();

    Record **table = malloc(sizeof(Record *) * period * slotsPerDay);
    Record **rejectedList = malloc(sizeof(Record *) * period * slotsPerDay);
    memset(table, 0, sizeof(Record *) * period * slotsPerDay);

    new_iter(input);
    Record *curTask = next(input);
    char *course;
    int duration;
    int intDDL;
    Date *DDL;
    int assignedSlot = 0;
    int assignPA = 0;
    Date *dateAndTime;

    struct Node *head = NULL;
    while (curTask != NULL) {
        insert(&head, newNode(curTask), mode);
        curTask = next(input);
    }

    curTask = head->task;
    struct Node *curNode = head;
    while (curNode != NULL) {
        curTask = curNode->task;
        //printf("%s\n",curTask->id);
        task_type type = curTask->type;
        switch (type) {
            case Project:
                while (table[assignPA] != NULL) assignPA++;
                intDDL = getDDL(curTask);
                if (assignPA >= intDDL) {
                    //printf("rejecting because of DDL: assignPA = %d\n", assignPA);
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                duration = curTask->duration;
                DDL = curTask->day;
                if (assignPA == slotsPerDay * period) {
                    //printf("rejecting because of full: assignPA = %d\n", assignPA);
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                } else accepted++;
                log_log(curTask, true);
                for (int i = 0; i < duration; i++) {
                    table[assignPA++] = curTask;
                    curTask->excuted++;
                    if (assignPA >= intDDL) break;
                    assignedSlot++;
                    while (table[assignPA] != NULL) assignPA++;
                    if (assignPA >= slotsPerDay * period)
                        return table;
                }

                break;

            case Assignment:
                while (table[assignPA] != NULL) assignPA++;
                intDDL = getDDL(curTask);
                if (assignPA >= intDDL) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                DDL = curTask->day;
                duration = curTask->duration;
                if (assignPA == slotsPerDay * period) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                } else accepted++;
                log_log(curTask, true);
                for (int i = 0; i < duration; i++) {
                    table[assignPA++] = curTask;
                    curTask->excuted++;
                    if (assignPA >= intDDL) break;
                    assignedSlot++;
                    while (table[assignPA] != NULL) assignPA++;
                    if (assignPA >= slotsPerDay * period)
                        return table;
                }
                break;

            case Revision:
                dateAndTime = curTask->day;
                duration = curTask->duration;
                int dayR = dateAndTime->days_since_base;
                int startSlotR = dateAndTime->time_slot;
                int startInTableR = dayR * slotsPerDay + startSlotR;
                int ableR = 1;
                if (startSlotR + duration > slotsPerDay)
                    ableR = 0;
                else {
                    for (int i = 0; i < duration; i++) {
                        if (table[startInTableR + i] != NULL) {
                            ableR = 0;
                            break;
                        }
                    }
                }
                if (ableR == 0) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                accepted++;
                log_log(curTask, true);
                int assignR = startInTableR;
                for (int i = 0; i < duration; i++) {
                    table[assignR] = curTask;
                    curTask->excuted++;
                    assignedSlot++;
                    assignR++;
                    if (assignedSlot > slotsPerDay * period)
                        return table;
                }
                break;

            case Activity:
                course = curTask->id;
                dateAndTime = curTask->day;
                duration = curTask->duration;
                int dayA = dateAndTime->days_since_base;
                int startSlotA = dateAndTime->time_slot;
                int startInTableA = dayA * slotsPerDay + startSlotA;
                int ableA = 1;
                if (startSlotA + duration > slotsPerDay)
                    ableA = 0;
                else {
                    for (int i = 0; i < duration; i++) {
                        if (table[startInTableA + i] != NULL) {
                            ableA = 0;
                            break;
                        }
                    }
                }

                if (ableA == 0) {
                    rejectedList[rejected] = curTask;
                    rejected++;
                    log_log(curTask, false);
                    break;
                }
                accepted++;
                log_log(curTask, true);
                int assignA = startInTableA;
                for (int i = 0; i < duration; i++) {
                    table[assignA] = curTask;
                    curTask->excuted++;
                    assignA++;
                    assignedSlot++;
                    if (assignedSlot > slotsPerDay * period)
                        return table;
                }
                break;

        }
        curNode = curNode->next;
        //printf("pass\n");
    }
    log_stop();
    return table;
}

int SDDL_cmp(const void *ptr1, const void *ptr2) {
    int t1 = getDDL(*(Record **) ptr1);
    int t2 = getDDL(*(Record **) ptr2);
    if (t1 == t2)
        return (*(Record **) ptr1)->duration - (*(Record **) ptr2)->duration;
    return t1 - t2;
}

Record **SDDL(Data_record *input) {
    int DDL_period = get_duration_date();
    int DDL_time = get_duration_time();
    Record **timetable = (Record **) malloc(sizeof(Record *) * DDL_period * DDL_time);
    for (int i = 0; i < DDL_period * DDL_time; ++i) {
        timetable[i] = NULL;
    }
    Record *cur = NULL;
    new_iter(input);
    int count[4] = {0, 0, 0, 0};
    int pos[4] = {0, 0, 0, 0};
    while ((cur = next(input)) != NULL)
        count[4 - cur->type]++;

    Record **Project_list = (Record **) malloc(count[0] * sizeof(Record *));
    Record **Assignment_list = (Record **) malloc(count[1] * sizeof(Record *));
    Record **Revision_list = (Record **) malloc(count[2] * sizeof(Record *));
    Record **Activity_list = (Record **) malloc(count[3] * sizeof(Record *));

    new_iter(input);
    while ((cur = next(input)) != NULL) {
        switch (cur->type) {
            case Project:
                Project_list[pos[0]++] = cur;
                break;
            case Assignment:
                Assignment_list[pos[1]++] = cur;
                break;
            case Revision:
                Revision_list[pos[2]++] = cur;
                break;
            case Activity:
                Activity_list[pos[3]++] = cur;
                break;
        }
    }

    qsort(Project_list, (size_t) count[0], sizeof(Project_list[0]), SDDL_cmp);
    qsort(Assignment_list, (size_t) count[1], sizeof(Record *), SDDL_cmp);
    qsort(Revision_list, (size_t) count[2], sizeof(Record *), SDDL_cmp);
    qsort(Activity_list, (size_t) count[3], sizeof(Record *), SDDL_cmp);
    int *project_time_layer = malloc(sizeof(int) * get_duration_date());
    int *assignment_time_layer = malloc(sizeof(int) * get_duration_date());
    //init layer 1
    for (int i = 0; i < DDL_period; ++i) {
        // project_time_layer[i]: the available time until i-th day end
        project_time_layer[i] = DDL_time * (i + 1);
    }
    //process layer 1
    for (int i = 0; i < count[0]; ++i) {
        cur = Project_list[i];
        int flag = 1;
        //check
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (project_time_layer[j] <= cur->duration) {
                flag = 0;
                break;
            }
        }

        if (flag == 0) //dont have time
        {
            cur->excuted = 1;
            Project_list[i] = NULL;
        } else {
            //arrange time
            for (int j = cur->day->days_since_base; j < DDL_period; ++j)
                project_time_layer[j] -= cur->duration;
        }
    }

    //init layer 2
    for (int i = 0; i < DDL_period; ++i) {
        // assignment_time_layer[i]: the available time until i-th day end
        assignment_time_layer[i] = project_time_layer[i];
    }
    //process layer 2
    for (int i = 0; i < count[1]; ++i) {
        cur = Assignment_list[i];
        int flag = 1;
        //check
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (assignment_time_layer[j] <= cur->duration) {
                flag = 0;
                break;
            }
        }
        if (flag == 0) //dont have time
        {
            cur->excuted = 1;
            Assignment_list[i] = NULL;
        } else {
            //arrange time
            for (int j = cur->day->days_since_base; j < DDL_period; ++j)
                assignment_time_layer[j] -= cur->duration;
        }
    }

    printf("\n");

    // process revison
    for (int i = 0; i < count[2]; ++i) {
        cur = Revision_list[i];
        int flag = 0;
        //check that day
        if (cur->day->time_slot + cur->duration >= DDL_time) {
            cur->excuted = 1;
            Revision_list[i] = NULL;
            continue;
        }
        for (int j = cur->day->time_slot; j < DDL_time; ++j) {
            if (timetable[(cur->day->days_since_base) * (DDL_time) + j] != NULL) {
                cur->excuted = 1;
                Revision_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //check avaliable time
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (assignment_time_layer[j] < cur->duration) {
                cur->excuted = 1;
                Revision_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //arrange in day
        int pos = (cur->day->days_since_base) + cur->day->time_slot;
        for (int j = 0; j < cur->duration; ++j)
            timetable[pos + j] = cur;
        //arrange in period
        for (int j = cur->day->days_since_base; j < DDL_period; ++j)
            assignment_time_layer[j] -= cur->duration;
    }

    // process activity
    for (int i = 0; i < count[3]; ++i) {
        cur = Activity_list[i];
        int flag = 0;
        //check that day
        if (cur->day->time_slot + cur->duration >= DDL_time) {
            cur->excuted = 1;
            Activity_list[i] = NULL;
            continue;
        }
        for (int j = cur->day->time_slot; j < DDL_time; ++j) {
            if (timetable[(cur->day->days_since_base) * (DDL_time) + j] != NULL) {
                cur->excuted = 1;
                Activity_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //check avaliable time
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (assignment_time_layer[j] < cur->duration) {
                cur->excuted = 1;
                Activity_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //arrange in day
        for (int j = cur->day->time_slot; j < cur->duration; ++j)
            timetable[(cur->day->days_since_base) * (DDL_time) + j] = cur;
        //arrange in period
        for (int j = cur->day->days_since_base; j < DDL_period; ++j)
            assignment_time_layer[j] -= cur->duration;
    }

    //arrange assignment
    for (int i = 0; i < count[1]; ++i) {
        if (Assignment_list[i] == NULL)continue;
        cur = Assignment_list[i];
        int pos = (cur->day->days_since_base + 1) * DDL_time - 1;
        for (int j = 0; j < cur->duration;) {
            if (timetable[pos] == NULL) {
                j++;
                timetable[pos] = cur;
            }
            pos--;
        }
    }

    //arrange project
    for (int i = 0; i < count[0]; ++i) {
        if (Project_list[i] == NULL)continue;
        cur = Project_list[i];
        int pos = (cur->day->days_since_base + 1) * DDL_time - 1;
        for (int j = 0; j < cur->duration;) {
            if (timetable[pos] == NULL) {
                j++;
                timetable[pos] = cur;
            }
            pos--;
        }
    }

    //set log
    set_algorithm_name("SDDL");
    log_start();
    new_iter(input);
    while ((cur = next(input)) != NULL) {
        log_log(cur, cur->excuted == 0 ? true : false);
        cur->excuted = (cur->excuted == 0) ? cur->duration : 0;
    }

    log_stop();

    return timetable;

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
                        printf("Could not open file %s\n", split_ptr);
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
                            print_timetable(SDDL(dataRecord), output_file_name);
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
            usleep(100000);
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
