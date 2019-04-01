//
// Created by 78705 on 2019/3/26.
//

#include "DDL.h"

int cmp(const void *ptr1, const void *ptr2) {
    int t1 = getDDL(*(Record **) ptr1);
    int t2 = getDDL(*(Record **) ptr2);
    if (t1 == t2)
        return (*(Record **) ptr1)->duration - (*(Record **) ptr2)->duration;
    return t1 - t2;
}


Record **DDL(Data_record *input) {
    int DDL_period = getdurationDate();
    int DDL_time = getdurationtime();
    Record **timetable = (Record **) malloc(sizeof(Record *) * DDL_period * DDL_time);
    for (int i = 0; i < DDL_period * DDL_time; ++i) {
        timetable[i] = NULL;
    }
    Record *cur = NULL;
    new_iter(input);
    int count[4] = {0, 0, 0, 0};
    int pos[4] = {0, 0, 0, 0};
    while ((cur = next(input)) != NULL)
        count[4 - cur->type]++;

    Record **Project_list = (Record **) malloc(count[0] * sizeof(Record *));
    Record **Assignment_list = (Record **) malloc(count[1] * sizeof(Record *));
    Record **Revision_list = (Record **) malloc(count[2] * sizeof(Record *));
    Record **Activity_list = (Record **) malloc(count[3] * sizeof(Record *));

    new_iter(input);
    while ((cur = next(input)) != NULL) {
        switch (cur->type) {
            case Project:
                Project_list[pos[0]++] = cur;
                break;
            case Assignment:
                Assignment_list[pos[1]++] = cur;
                break;
            case Revision:
                Revision_list[pos[2]++] = cur;
                break;
            case Activity:
                Activity_list[pos[3]++] = cur;
                break;
        }
    }

    qsort(Project_list, (size_t) count[0], sizeof(Project_list[0]), cmp);
    qsort(Assignment_list, (size_t) count[1], sizeof(Record *), cmp);
    qsort(Revision_list, (size_t) count[2], sizeof(Record *), cmp);
    qsort(Activity_list, (size_t) count[3], sizeof(Record *), cmp);
    int *project_time_layer = malloc(sizeof(int) * getdurationDate());
    int *assignment_time_layer = malloc(sizeof(int) * getdurationDate());
    //init layer 1
    for (int i = 0; i < DDL_period; ++i) {
        // project_time_layer[i]: the available time until i-th day end
        project_time_layer[i] = DDL_time * (i + 1);
    }
    //process layer 1
    for (int i = 0; i < count[0]; ++i) {
        cur = Project_list[i];
        int flag = 1;
        //check
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (project_time_layer[j] <= cur->duration) {
                flag = 0;
                break;
            }
        }

        if (flag == 0) //dont have time
        {
            cur->excuted = 1;
            Project_list[i] = NULL;
        } else {
            //arrange time
            for (int j = cur->day->days_since_base; j < DDL_period; ++j)
                project_time_layer[j] -= cur->duration;
        }
    }

    //init layer 2
    for (int i = 0; i < DDL_period; ++i) {
        // assignment_time_layer[i]: the available time until i-th day end
        assignment_time_layer[i] = project_time_layer[i];
    }
    //process layer 2
    for (int i = 0; i < count[1]; ++i) {
        cur = Assignment_list[i];
        int flag = 1;
        //check
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (assignment_time_layer[j] <= cur->duration) {
                flag = 0;
                break;
            }
        }
        if (flag == 0) //dont have time
        {
            cur->excuted = 1;
            Assignment_list[i] = NULL;
        } else {
            //arrange time
            for (int j = cur->day->days_since_base; j < DDL_period; ++j)
                assignment_time_layer[j] -= cur->duration;
        }
    }
    //debug
    /*
    for (int i = 0; i < DDL_period; ++i) {
        printf("day:%d\tlayer1:\t%d\tlayer2:\t%d\n",i,project_time_layer[i],assignment_time_layer[i]);
    }*/

    printf("\n");

    // process revison
    for (int i = 0; i < count[2]; ++i) {
        cur = Revision_list[i];
        int flag = 0;
        //check that day
        if (cur->day->time_slot + cur->duration >= DDL_time) {
            cur->excuted = 1;
            Revision_list[i] = NULL;
            continue;
        }
        for (int j = cur->day->time_slot; j < DDL_time; ++j) {
            if (timetable[(cur->day->days_since_base) * (DDL_time) + j] != NULL) {
                cur->excuted = 1;
                Revision_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //check avaliable time
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (assignment_time_layer[j] < cur->duration) {
                cur->excuted = 1;
                Revision_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //arrange in day
        int pos = (cur->day->days_since_base) + cur->day->time_slot;
        for (int j = 0; j < cur->duration; ++j)
            timetable[pos + j] = cur;
        //arrange in period
        for (int j = cur->day->days_since_base; j < DDL_period; ++j)
            assignment_time_layer[j] -= cur->duration;
    }

    // process activity
    for (int i = 0; i < count[3]; ++i) {
        cur = Activity_list[i];
        int flag = 0;
        //check that day
        if (cur->day->time_slot + cur->duration >= DDL_time) {
            cur->excuted = 1;
            Activity_list[i] = NULL;
            continue;
        }
        for (int j = cur->day->time_slot; j < DDL_time; ++j) {
            if (timetable[(cur->day->days_since_base) * (DDL_time) + j] != NULL) {
                cur->excuted = 1;
                Activity_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //check avaliable time
        for (int j = cur->day->days_since_base; j < DDL_period; ++j) {
            if (assignment_time_layer[j] < cur->duration) {
                cur->excuted = 1;
                Activity_list[i] = NULL;
                flag = 1;
                break;
            }
        }
        if (flag == 1)continue;
        //arrange in day
        for (int j = cur->day->time_slot; j < cur->duration; ++j)
            timetable[(cur->day->days_since_base) * (DDL_time) + j] = cur;
        //arrange in period
        for (int j = cur->day->days_since_base; j < DDL_period; ++j)
            assignment_time_layer[j] -= cur->duration;
    }

    //arrange assignment
    for (int i = 0; i < count[1]; ++i) {
        if (Assignment_list[i] == NULL)continue;
        cur = Assignment_list[i];
        int pos = (cur->day->days_since_base + 1) * DDL_time - 1;
        for (int j = 0; j < cur->duration;) {
            if (timetable[pos] == NULL) {
                j++;
                timetable[pos] = cur;
            }
            pos--;
        }
    }

    //arrange project
    for (int i = 0; i < count[0]; ++i) {
        if (Project_list[i] == NULL)continue;
        cur = Project_list[i];
        int pos = (cur->day->days_since_base + 1) * DDL_time - 1;
        for (int j = 0; j < cur->duration;) {
            if (timetable[pos] == NULL) {
                j++;
                timetable[pos] = cur;
            }
            pos--;
        }
    }

    //set log
    set_algorithm_name("DDL");
    log_start();
    new_iter(input);
    while ((cur = next(input)) != NULL){
        log_log(cur, cur->excuted == 0 ? true : false);
        cur->excuted = cur->excuted == 0 ? cur->duration : 0;
    }

    log_stop();

    return timetable;
    //debug
    /*
    for (int i = 0; i < DDL_period; ++i) {
        printf("day:%d\tlayer1:\t%d\tlayer2:\t%d\n",i,project_time_layer[i],assignment_time_layer[i]);
    }

    //debug

    printf("\n\nproject:\n");
    for(int i = 0;i<count[0];++i){
        if(Project_list[i]==NULL)continue;
        printf("%p\t%s \t\t DDL:%d duration:%d\n",Project_list[i],Project_list[i]->id,Project_list[i]->day->days_since_base,Project_list[i]->duration);
    }

    printf("\n\nassignment:\n");
    for(int i = 0;i<count[1];++i){
        if(Assignment_list[i]==NULL)continue;
        printf("%p\t%s \t\t DDL:%d duration:%d\n",Assignment_list[i],Assignment_list[i]->id,Assignment_list[i]->day->days_since_base,Assignment_list[i]->duration);
    }

    printf("\n\nrevision:\n");
    for(int i = 0;i<count[2];++i){
        if(Revision_list[i]==NULL)continue;
        printf("%p\t%s \t\t start day:%d time_slot:%d duration:%d\n",Revision_list[i],Revision_list[i]->id,Revision_list[i]->day->days_since_base,Revision_list[i]->day->time_slot,Revision_list[i]->duration);
    }

    printf("\n\nactivity:\n");
    for(int i = 0;i<count[3];++i){
        if(Activity_list[i]==NULL)continue;
        printf("%p\t%s \t\t start day:%d time_slot:%d duration:%d\n",Activity_list[i],Activity_list[i]->id,Activity_list[i]->day->days_since_base,Activity_list[i]->day->time_slot,Activity_list[i]->duration);
    }
    printf("\n---------------------\n");*/
    /*char buf[20];
    for(int i=0;i<getdurationDate();i++){
        printf("%s",convert_to_date(i,buf));
        for(int j=0;j<DDL_time;j++){
            cur = timetable[i*DDL_time+j];
            printf("\t%-12s",cur==NULL?"N/A":(cur->id));
        }

        //printf("\t%-12s",timetable[i*DDL_time+j]==NULL?"N/A":(timetable[i*DDL_period+j]->id));
        printf("\n");
    }*/

}