//
//  Priority.c
//  2432P
//
//  Created by 贺家澍 on 31/3/2019.
//  Copyright © 2019 HE Jiashu. All rights reserved.
//

#include "Priority.h"

Record** Priority(Data_record* input, int mode){
    //mode 0 is priority, mode 1 is DDL
    
    set_algorithm_name(mode==0?"Priority":"Deadline");
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
    Date* DDL;
    int assignedSlot=0;
    int assignPA=0;
    Date* dateAndTime;
    
    struct Node* head = NULL;
    
    printf("Insertion starts\n");
    while(curTask != NULL){
        insert(&head, newNode(curTask), mode);
        printList(head);
        curTask = next(input);
    }
    
    printf("Insertion passed.\n");
    struct Node* curNode = head;
    curTask = curNode->task;
    
    while(curNode!=NULL){
        task_type type = curTask->type;
        switch(type){
            case Project:
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
                if(table[startInTableR]!=NULL || (startSlotR+duration)>slotsPerDay){
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
                if(table[startInTableA]!=NULL || (startInTableA+duration)>slotsPerDay){
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
        curNode=curNode->next;
        curTask=curNode->task;
    }
    log_stop();
    return table;
}
