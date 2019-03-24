#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "record.h"
#include "mytime.h"

static int log_id=1;
static char *algo_name=NULL;
static FILE *log_file=NULL;
static char *start_date=NULL;

const char* type_to_command(task_type type){
    switch(type){
        case Project: return "addProject";
        case Assignment: return "addAssignment";
        case Revision: return "addRevision";
        case Activity: return "addActivity";
    }
    return "nope";
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
    fprintf(log_file,"ID\tEvent\t\t\t\t\tAccepted/Rejected\n");
    fprintf(log_file,"==================================================================");

    log_id=1;
    start_date=(char*)malloc(11);
    get_start_date(start_date);

}

void log_log(Record* record, bool accepted){
    fprintf(log_file,"%4d\t%s %s %s ",log_id++,type_to_command(record->type),record->id,convert_to_date(record->day->days_since_base,start_date));
    if(record->type==Assignment||record->type==Project)
        fprintf(log_file,"%d\t",record->duration);
    else
        fprintf(log_file,"%d:00 %d\t",record->day->time_slot+getStartTime(),record->duration);

    fprintf(log_file,"%s\n",accepted?"Accepted":"Rejected");
}

void print_timetable(Record** table){
    int width=getEndTime()-getStartTime()+1;
    char buf[11];
    get_start_date(buf);
    printf("Timetable\nPeriod: %s",buf);
    get_end_date(buf);
    printf(" to %s\nAlgorithm used: %s\n",buf,algo_name);
    printf("Date");
    for(int i=getStartTime();i<=getEndTime();i++)
        printf("\t%d:00",i);
    printf("\n");

    get_start_date(buf);
    for(int i=0;i<getDurationDate();i++){
        printf("%s",convert_to_date(i,buf));
        for(int j=0;j<width;j++)
            printf("\t%s",table[i*width+j]==NULL?"N/A":table[i*width+j]->id);
        printf("\n");
    }
}

void print_report(int acc,int rej,int slot_used,int slot_num){
    printf("***Summary Report***\nAlgorithm used: %s\n",algo_name);
    printf("There are %d requests.\nNumber of request accepted: %d\nNumber of request rejected: %d\n",acc+rej,acc,rej);
    printf("Number of time slots used: %d (%.2f%%)\n",slot_used,slot_used%((float)slot_num));
}
