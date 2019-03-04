#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "mytime.h"

// "YYYY-MM-DD" to store at globol char *start_date 
bool set_start_date(char* date){
    assert(is_valid_date(date));

    start_date = date;
    return true;
}

// "YYYY-MM-DD" to store at globol char *end_date 
bool set_end_date(char* date){
    assert(is_valid_date(date));

    start_date = date;
    return true;
}

//get date_to_base
int convert_to_base(char* date){
    assert(is_valid_date(date));

    //todomark
    return 0;
}

//get date_to_realdate
char* convert_to_date(int num){

    //todomark
    return NULL;
}


//check valid date "YYYY-MM-DD"
bool is_valid_date(char* date){
        if(strlen(date) != 10)
        return false;
    for(size_t i = 0; i < strlen(date); i++)
    {   
        if(i == 4 || i == 7)// '-'
            continue;
        if('9'<date[i] || '0'>date[i])
            return false;
    }
    return true;
}
