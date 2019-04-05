#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "record.h"
#include "mytime.h"
#include "log.h"

static int log_id=1;
static char *algo_name=NULL;
static FILE *log_file=NULL;
static FILE *err_file=NULL;
static char _log_date_buf[11];

static int acc_count=0;
static int rej_count=0;
static int slot_occupied=0;
static Data_record* accepted_tasks=NULL;
static Data_record* rejected_tasks=NULL;

const char* type_to_command(task_type type){
    switch(type){
        case Project: return "addProject";
        case Assignment: return "addAssignment";
        case Revision: return "addRevision";
        case Activity: return "addActivity";
    }
    return "nope";
}


void init_error_log(){
    err_file=fopen("S3_error.log","w");
}

void set_algorithm_name(const char* algorithm_name){
    algo_name=(char*)malloc(strlen(algorithm_name)+1);
    strcpy(algo_name,algorithm_name);
}

void log_start(){
    assert(algo_name!=NULL);
    char* filename=(char*)malloc(strlen(algo_name)+8);
    sprintf(filename,"S3_%s.log",algo_name);
    log_file=fopen(filename,"w");
    free(filename);

    fprintf(log_file,"****Log File - %s***\n",algo_name);
    fprintf(log_file,"ID\t%-54s\tAccepted/Rejected\n","Event");
    fprintf(log_file,"==================================================================================\n");

    log_id=1;
    acc_count=0;
    rej_count=0;
    slot_occupied=0;
    accepted_tasks=newDataRecord();
    rejected_tasks=newDataRecord();
}

void log_log(Record* record, bool accepted){
    fprintf(log_file,"%04d\t%s %s %s ",log_id++,type_to_command(record->type),record->id,convert_to_date(record->day->days_since_base,_log_date_buf));
    if(record->type==Assignment||record->type==Project)
        fprintf(log_file,"%d",record->duration);
    else
        fprintf(log_file,"%02d:00 %d",record->day->time_slot+getStartTime(),record->duration);

    int pad=strlen(record->id)+10+4+(record->type==Assignment?13:(record->type==Project?10:11+6));
    pad=54-pad;
    fprintf(log_file,"%*c",pad,'\t');

    fprintf(log_file,"%s\n",accepted?"Accepted":"         Rejected");

    accepted?acc_count++:rej_count++;
    if(accepted)
        add_data(accepted_tasks,record);
    else
        add_data(rejected_tasks,record);
}

void log_error(Record* record, char* msg){
    fprintf(err_file,"[Error] %s <%s %s %s ",msg,type_to_command(record->type),record->id,convert_to_date(record->day->days_since_base,_log_date_buf));
    if(record->type==Assignment||record->type==Project){
        fprintf(err_file,"%d>\n",record->duration);
    }

    else{
        fprintf(err_file,"%02d:00 %d>\n",record->day->time_slot+getStartTime(),record->duration);
    }
}

void log_stop(){
    fclose(log_file);
}

void stop_error_log(){
    fclose(err_file);
}

void print_timetable(Record** table, char* filename){
    FILE* out=fopen(filename,"w");
    int width=getEndTime()-getStartTime();
    char *buf=_log_date_buf;
    get_start_date(buf);
    printf("Alice Timetable\nPeriod: %s",buf);
    fprintf(out,"Alice Timetable\nPeriod: %s",buf);
    get_end_date(buf);
    printf(" to %s\nAlgorithm used: %s\n",buf,algo_name);
    fprintf(out," to %s\nAlgorithm used: %s\n",buf,algo_name);

    printf("Date      ");fprintf(out,"Date      ");
    for(int i=getStartTime();i<getEndTime();i++)
        printf("\t%02d:00       ",i),fprintf(out,"\t%02d:00       ",i);
    printf("\n");fprintf(out,"\n");

    char* str;
    get_start_date(buf);
    for(int i=0;i<getdurationDate();i++){
        convert_to_date(i,buf);
        printf("%s",buf);fprintf(out,"%s",buf);
        for(int j=0;j<width;j++){
            if(table[i*width+j]==NULL){
                str="N/A";
            }else{
                str=table[i*width+j]->id;
                slot_occupied++;
            }
            printf("\t%-12s",str);
            fprintf(out,"\t%-12s",str);
        }
        printf("\n");fprintf(out,"\n");
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

    int completed_count=0,completed_time=0;
    int total_time=0;
    new_iter(accepted_tasks);
    for(Record* rec=next(accepted_tasks);rec!=NULL;rec=next(accepted_tasks)){
        total_time+=rec->duration;
        completed_time+=rec->excuted;
        if(rec->duration==rec->excuted) completed_count++;
    }
    
    printf("%d out of %d tasks completely arranged (%.2f%%)\n",completed_count,acc_count,1e2*completed_count/acc_count);
    printf("%d out of %d hours arranged (%.2f%%)\n",completed_time,total_time,1e2*completed_time/total_time);
    
    fprintf(out,"%d out of %d tasks completely arranged (%.2f%%)\n",completed_count,acc_count,1e2*completed_count/acc_count);
    fprintf(out,"%d out of %d hours arranged (%.2f%%)\n",completed_time,total_time,1e2*completed_time/total_time);
    
    if(rejected_tasks->count){
        printf("The following task(s) will be rejected:\n");
        fprintf(out,"The following task(s) will be rejected:\n");
        new_iter(rejected_tasks);
        for(Record* rec=next(rejected_tasks);rec!=NULL;rec=next(rejected_tasks))
            printf("%s %s\n",type_to_command(rec->type),rec->id),fprintf(out,"%s %s\n",type_to_command(rec->type),rec->id);
    }

    fclose(out);
    free(rejected_tasks);
}
