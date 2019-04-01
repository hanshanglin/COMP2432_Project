//
//  FCFS.c
//  OS project
//
//  Created by 贺家澍 on 10/3/2019.
//  Copyright © 2019 HE Jiashu. All rights reserved.
//

#include "FCFS.h"
#include "record.h"
#include "mytime.h"
#include <string.h>
#include <stdlib.h>
#include "log.h"
Record** FCFS(Data_record* input){
    set_algorithm_name("FCFS");
    log_start();
   
    int accepted=0;
    int rejected = 0;
    
    int period = getdurationDate();
    int slotsPerDay = getdurationtime();
    
    Record** table = malloc(sizeof(Record*)*period*slotsPerDay);
    Record** rejectedList=malloc(sizeof(Record*)*period*slotsPerDay);
    memset(table,0,sizeof(Record*)*period*slotsPerDay);
    
    new_iter(input);
    Record* curTask = next(input);
    char* course;
    int duration;
    Date* DDL;
    int assignedSlot=0;
    int assignPA=0;
    Date* dateAndTime;
    while(curTask!=NULL){
        task_type type = curTask->type;
        switch(type){
            case Project:
                while(table[assignPA]!=NULL) assignPA++;
                duration = curTask->duration;
                DDL = curTask->day;
                if(assignPA==slotsPerDay*period){
                    rejectedList[rejected]=curTask;
                    rejected++;
                    log_log(curTask,false);
                    break;
                }
                else accepted++;
                log_log(curTask,true);
                for(int i =0; i < duration;i++){
                    table[assignPA++]=curTask;
                    if(assignPA > DDL->days_since_base*slotsPerDay) break;
                    assignedSlot++;
                    while(table[assignPA]!=NULL) assignPA++;
                    if(assignPA>=slotsPerDay*period)
                        return table;
                }
                
                break;
            
            case Assignment:
                while(table[assignPA]!=NULL) assignPA++;
                DDL = curTask->day;
                duration = curTask->duration;
                if(assignPA==slotsPerDay*period){
                    rejectedList[rejected]=curTask;
                    rejected++;
                    log_log(curTask,false);
                    break;
                }
                else accepted++;
                log_log(curTask,true);
                for(int i =0; i < duration;i++){
                    table[assignPA++]=curTask;
                    if(assignPA > DDL->days_since_base*slotsPerDay) break;
                    assignedSlot++;
                    while(table[assignPA]!=NULL) assignPA++;
                    if(assignPA>=slotsPerDay*period)
                        return table;
                }
                break;

            case Revision:
                dateAndTime = curTask->day;
                duration = curTask->duration;
                int dayR = dateAndTime->days_since_base;
                int startSlotR = dateAndTime->time_slot;
                int startInTableR = dayR*slotsPerDay+startSlotR;
                int ableR=1;
                if(startSlotR+duration>slotsPerDay)
                    ableR=0;
                else{
                    for(int i = 0; i < duration;i++){
                        if(table[startInTableR+i]!=NULL){
                            ableR=0;
                            break;
                        }
                    }
                }
                if(ableR==0){
                    rejectedList[rejected]=curTask;
                    rejected++;
                    log_log(curTask,false);
                    break;}
                accepted++;
                log_log(curTask,true);
                int assignR = startInTableR;
                for(int i = 0;i<duration;i++){
                    table[assignR]=curTask;
                    assignedSlot++;
                    assignR++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                break;
                
            case Activity:
                course=curTask->id;
                dateAndTime=curTask->day;
                duration=curTask->duration;
                int dayA = dateAndTime->days_since_base;
                int startSlotA = dateAndTime->time_slot;
                int startInTableA = dayA*slotsPerDay+startSlotA;
                int ableA=1;
                if(startSlotA+duration>slotsPerDay)
                    ableA=0;
                else{
                    for(int i = 0; i < duration;i++){
                        if(table[startInTableA+i]!=NULL){
                            ableA=0;
                            break;
                        }
                    }
                }
                
                if(ableA==0){
                    rejectedList[rejected]=curTask;
                    rejected++;
                    log_log(curTask,false);
                    break;
                }
                accepted++;
                log_log(curTask,true);
                int assignA = startInTableA;
                for(int i = 0;i<duration;i++){
                    table[assignA]=curTask;
                    assignA++;
                    assignedSlot++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                break;
                
        }
        curTask=next(input);
    }
    log_stop();
    
    return table;
}

