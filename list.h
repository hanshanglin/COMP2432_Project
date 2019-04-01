//
//  list.h
//  2432P
//
//  Created by 贺家澍 on 30/3/2019.
//  Copyright © 2019 HE Jiashu. All rights reserved.
//

#ifndef list_h
#define list_h

#include <stdio.h>
#include "record.h"
struct Node{
    Record* task;
    struct Node* next;
};
struct Node* newNode(Record* task);
int compare(Record* t1, Record* t2, int mode);
int insert(struct Node** head_ref, struct Node* new_node, int mode);
void printList(struct Node* list);
#endif /* list_h */
