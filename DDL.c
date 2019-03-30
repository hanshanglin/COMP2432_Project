//
// Created by 78705 on 2019/3/26.
//

#include "DDL.h"
#include <math.h>

int cmp_1(const void *ptr1, const void *ptr2) {
    int day1 = ((Record *) ptr1)->day->days_since_base;
    int day2 = ((Record *) ptr2)->day->days_since_base;
    if (day1 > day2)
        return 1;
    else if (day1 == day2)
        return ((Record *) ptr1)->duration > ((Record *) ptr2)->duration;
    else return -1;
}

int cmp_2(const void *ptr1, const void *ptr2) {
    int day1 = ((Record *) ptr1)->day->days_since_base;
    int slot1 = ((Record *) ptr1)->day->time_slot;
    int day2 = ((Record *) ptr2)->day->days_since_base;
    int slot2 = ((Record *) ptr2)->day->time_slot;
    if (day1 > day2)
        return 1;
    else if (day1 == day2) {
        if (slot1 > slot2)return 1;
        else if (slot1 == slot2) {
            return ((Record *) ptr1)->duration > ((Record *) ptr2)->duration;
        } else
            return -1;
    } else return -1;
}

Record **DDL(Data_record *input) {
    int DDL_period = getdurationDate();
    int DDL_time = getdurationtime();
    int *bit = (int *) malloc(sizeof(int) * DDL_period);
    memset(bit, 0, sizeof(bit));
    int detect = (int) pow((float) 2, (float) DDL_period);

    Record *cur = NULL;
    new_iter(input);
    int count[4] = {0, 0, 0, 0};
    int pos[4] = {0, 0, 0, 0};
    while ((cur = next(input)) != NULL)
        count[cur->type]++;
    Record **Project_list = (Record **) malloc(count[0] * sizeof(record * ));
    Record **Assignment_list = (Record **) malloc(count[1] * sizeof(record * ));
    Record **Revision_list = (Record **) malloc(count[2] * sizeof(record * ));
    Record **Activity_list = (Record **) malloc(count[3] * sizeof(record * ));

    new_iter(input);
    while ((cur = next(input)) != NULL) {
        switch (cur->type) {
            case Project:
                Project_list[pos[0]++] = cur;
                break;
            case Assignment:
                Assignment_list[pos[1]++] = cur;
                break;
            case Revision_list[pos[2]++]=cur:
                break;
            case Activity_list[pos[3]++]=cur:
                break;
        }
    }
    qsort(Project_list, count[0], sizeof(Record *), cmp_1);
    qsort(Assignment_list, count[1], sizeof(Record *), cmp_1);
    qsort(Revision_list, count[2], sizeof(Record *), cmp_2);
    qsort(Activity_list, count[3], sizeof(Record *), cmp_2);

    

}