#include "record.h"

const char* type_to_command(task_type type);


void set_algorithm_name(const char* algorithm_name);

void log_start(void);

void log_log(Record* record, bool accepted);

void log_stop(void);

void print_timetable(Record** table, char* filename);

void print_report(char* filename);

void init_error_log(void);

void log_error(Record* record, char* msg);

void stop_error_log(void);
