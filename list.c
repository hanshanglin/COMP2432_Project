//
//  list.c
//  2432P
//
//  Created by 贺家澍 on 30/3/2019.
//  Copyright © 2019 HE Jiashu. All rights reserved.
//

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include "record.h"
struct Node* newNode(Record* task){
    struct Node* sol = (struct Node*)malloc(sizeof(struct Node));
    sol->next = NULL;
    sol->task=task;
    return sol;
}
int compare(Record* t1, Record* t2, int mode){
    //return 1 if t1 has higher priority
    //return -1 if t1 has lower priority
    //return 0 if t1 has the same priority with t2
    //mode 0: priority, mode 1: DDL
    if(mode==0){
        task_type type1 = t1->type;
        task_type type2 = t2->type;
        if(type1>type2) return 1;
        else if(type1==type2) return 0;
        else return -1;
    }
    else{
        task_type type1 = t1->type;
        task_type type2 = t2->type;
        if(type1 <= 2 && type2 <=2){
            if(getDDL(t1) < getDDL(t2)) return 1;
            else if(getDDL(t1) == getDDL(t2)) return 0;
            else return -1;
        }
        else if(type1 >= 3 && type2 >= 3) return 0;
        else{
            if(type1 <= 2) return 1;
            else return -1;
        }
    }
    return -2;//-2 means an error has occur
}

int insert(struct Node** head_ref, struct Node* new_node, int mode){//mode 0: priority, mdoe 1: DDL
    struct Node* current;
    if(*head_ref==NULL || (compare((*head_ref)->task, new_node->task, mode)==-1)){
        new_node->next=*head_ref;
        *head_ref=new_node;
    }
    else{
        current = *head_ref;
        while(current->next!=NULL && (compare(current->next->task, new_node->task,mode)==1 || compare(current->next->task, new_node->task,mode)==0)){
            current = current->next;
        }
        new_node->next=current->next;
        current->next = new_node;
    }
    return 0;
    
    
    
}
void printList(struct Node* list){
    struct Node* cur = list;
    while(cur!=NULL){
        printf("%s->",cur->task->id);
        cur = cur->next;
    }
    printf("NULL\n");
}
