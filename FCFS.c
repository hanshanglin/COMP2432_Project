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
char** FCFS(Data_record* input){
   
    int period = getdurationDate();
    int slotsPerDay = getdurationtime();
    char** table = malloc(sizeof(char*)*period*slotsPerDay);
    new_iter(input);
    Record* curTask = next(input);
    int curSlot = 0;
    char* course;
    int duration;
    int assignedSlot=0;
    char* write;
    Date* dateAndTime;
    while(curTask!=NULL){
        task_type type = curTask->type;
        switch(type){
            case Project:
                course = curTask->id;
                dateAndTime = curTask->day;
                int dueP=dateAndTime->days_since_base;
                duration = curTask->duration;
                write=malloc(strlen(course)+strlen("Project")+1);
                strcpy(write, course);
                strcat(write, "Project");
                for(int i =0; i < duration;i++){
                    strcpy(table[assignedSlot], write);
                    assignedSlot++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                free(write);
                break;
            case Assignment:
                course = curTask->id;
                dateAndTime = curTask->day;
                duration = curTask->duration;
                //int dueA = dateAndTime->days_since_base;
                for(int i =0; i < duration;i++){
                    strcpy(table[assignedSlot], course);
                    assignedSlot++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                break;
            
            case Revision:
                course = curTask->id;
                dateAndTime = curTask->day;
                duration = curTask->duration;
                int dayR = dateAndTime->days_since_base;
                int startSlotR = dateAndTime->time_slot;
                int startInTableR = dayR*slotsPerDay+startSlotR;
                if(table[startInTableR]!=NULL)
                    break;
                write=malloc(strlen(course)+strlen("Revision")+1);
                strcpy(write, course);
                strcat(write, "Revision");
                for(int i = 0;i<duration;i++){
                    strcpy(table[assignedSlot], write);
                    assignedSlot++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                free(write);
                break;
                
            case Activity:
                course=curTask->id;
                dateAndTime=curTask->day;
                duration=curTask->duration;
                int dayA = dateAndTime->days_since_base;
                int startSlotA = dateAndTime->time_slot;
                int startInTableA = dayA*slotsPerDay+startSlotA;
                if(table[startInTableA]!=NULL) break;
                for(int i = 0;i<duration;i++){
                    strcpy(table[assignedSlot], course);
                    assignedSlot++;
                    if(assignedSlot>slotsPerDay*period)
                        return table;
                }
                break;
                
        }
        curTask=next(input);
    }
    return table;
}

