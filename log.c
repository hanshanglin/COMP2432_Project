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
static int completed_counts[4],total_counts[4];
static int completed_time[4],total_time[4];
static Data_record* unfinished_tasks=NULL;
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
    //err_file=fopen("S3_error.log","w");
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
    unfinished_tasks=newDataRecord();
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
    if(!accepted)
        add_data(rejected_tasks,record);
}

void log_error(Record* record, char* msg){
    err_file=fopen("S3_error.log","a");
    fprintf(err_file,"[Error] %s <%s %s %s ",msg,type_to_command(record->type),record->id,convert_to_date(record->day->days_since_base,_log_date_buf));
    if(record->type==Assignment||record->type==Project){
        fprintf(err_file,"%d>\n",record->duration);
    }

    else{
        fprintf(err_file,"%02d:00 %d>\n",record->day->time_slot+getStartTime(),record->duration);
    }
    fclose(err_file);
}

void log_stop(){
    fclose(log_file);
}

void stop_error_log(){
    //fclose(err_file);
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
                if(table[i*width+j]->excuted>-1){
		    int type=table[i*width+j]->type-1;
                    total_time[type]+=table[i*width+j]->duration;
                    completed_time[type]+=table[i*width+j]->excuted;
                    if(table[i*width+j]->duration==table[i*width+j]->excuted)
			completed_counts[type]++;
		    else
			add_data(unfinished_tasks,table[i*width+j]);
                    table[i*width+j]->excuted=-1;
		    total_counts[type]++;
                }
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
