//
// Created by 78705 on 2019/3/26.
//

#include "DDL.h"
#include <math.h>

Record** DDL(Data_record* input){
    int DDL_period = getdurationDate();
    int DDL_time = getdurationtime();
    int* bit = (int*)malloc(sizeof(int)*DDL_period);
    memset(bit,0, sizeof(bit));
    int detect = (int)pow((float)2,(float)DDL_period);

    Record* cur = NULL;
    new_iter(input);
    while((cur = next(input))!=NULL) {
        switch (cur->type){
            case Project:
                break;
            case Assignment:
                break;
            case Revision:
                break;
            case Activity:
                break;
        }

    }

}