#ifndef FCFS_C_INCLUDED
#define FCFS_C_INCLUDED

//
//  FCFS.c
//  OS project
//
//  Created by 贺家澍 on 10/3/2019.
//  Copyright © 2019 HE Jiashu. All rights reserved.
//

#include "record.h"
#include "mytime.h"
#include <string.h>
#include "log.c"
Record** FCFS(Data_record* input){
   
   //log system init
   set_algorithm_name("FCFS");
   log_start();
   
    int accepted=0;
    int rejected = 0;
    
    int period = getdurationDate();
    int slotsPerDay = getdurationtime();
    
    Record** table = malloc(sizeof(Record*)*period*slotsPerDay);
    Record** rejectedList=malloc(sizeof(Record*)*period*slotsPerDay);
    
    new_iter(input);
    Record* curTask = next(input);
    char* course;
    int duration;
    
    int assignedSlot=0;
    
    Date* dateAndTime;
    while(curTask!=NULL){
        task_type type = curTask->type;
        switch(type){
            case Project:
                duration = curTask->duration;
                
                if(assignedSlot==slotsPerDay*period){
                    rejectedList[rejected]=curTask;
                    rejected++;
                   log_log(curTask,false);
                    break;
                }
                
                else accepted++;
              log_log(curTask,true);
                for(int i =0; i < duration;i++){
                    table[assignedSlot]=curTask;
                    assignedSlot++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                
                break;
            
            case Assignment:
                duration = curTask->duration;
                if(assignedSlot==slotsPerDay*period){
                    rejectedList[rejected]=curTask;
                    rejected++;
                   log_log(curTask,false);
                    break;
                }
                else accepted++;
              log_log(curTask,true);
                
                for(int i =0; i < duration;i++){
                    table[assignedSlot]=curTask;
                    assignedSlot++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                break;

            case Revision:
                dateAndTime = curTask->day;
                duration = curTask->duration;
                int dayR = dateAndTime->days_since_base;
                int startSlotR = dateAndTime->time_slot;
                int startInTableR = dayR*slotsPerDay+startSlotR;
                if(table[startInTableR]!=NULL || (startSlotR+duration)>slotsPerDay){
                    rejectedList[rejected]=curTask;
                    rejected++;
                   log_log(curTask,false);
                    break;}
                accepted++;
              log_log(curTask,true);
                
                for(int i = 0;i<duration;i++){
                    table[assignedSlot]=curTask;
                    assignedSlot++;
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
                if(table[startInTableA]!=NULL || (startInTableA+duration)>slotsPerDay){
                    rejectedList[rejected]=curTask;
                    rejected++;
                   log_log(curTask,false);
                    break;
                }
                accepted++;
              log_log(curTask,true);
                for(int i = 0;i<duration;i++){
                    table[assignedSlot]=curTask;
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

#endif
