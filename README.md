# COMP2432_Project

- [x] 小川 test case+input  
- [x] hansama fix time+record etc al  
- [x] Jason 算法(FCFS)  
- [ ] Lza print,log  
- [x] Lian test case   
- [x] hansama boxing time+record  

## Input(part 1):
1. add details (duration,subject,priority,etc)
2. assign an ID
3. record `"accepted"` or `"rejected"` in a log file base on algorithms (in S3_XXX.log)

## Kernel(part 2):
  using a scheduler to generate timetables.  
  (implement some scheduling algorithms)

## Output(part 3):
print out time table

## Analyze(part 4):
generate a summary report and analyze the results produced in Part 3  
compare the different timetables(from different algorithms),find out which one would be the most suitable one  
An outstanding list may be included in the report for those tasks cannot be scheduled.  

## Interface

### input
- addPeriod `[start date]` `[end date]` `[start time]` `[end time]`  
  eg. addPeriod 2019-04-08 2019-04-21 19:00 23:00
  >specify the period __[dates of start and end]__ and the time slots __[start time and end time]__ for scheduling events.  
  >Date and time format is year-month-day and hour (__YYYY-MM-DD__ and __hh:mm__, simply take the hour and ignore the minute (i.e. mm=00) and use the 24-hour).   
  >Actually, this should be __the first command__ for the application.

- addAssignment `[subject code with assignment number]` `[due date]` `[duration]`  
  eg. addAssignment COMP2432A1 2019-04-18 12
  >[addAssignment] is to add a subject assignment to the scheduler. Follows by the due date and hours required to finish the assignment. The format __[YYYY-MM-DD]__ is that YYYY is year, MM is month and DD is day. Then the [duration] is the number of hours that required to finish the assignment.
  >for a task whose duration is more than 4 hours it could be allocated to the timeslots of the next coming days.  
  >for assignment, you __may not finish it at one time__.   
  >that may cause some assignments which __could not be fully completed__, i.e. less than 100 percent. If so, you should indicate the percentage of completion __in the report__.
  
- addProject `[subject code with project number]` `[due date]` `[duration]`  
  eg. addProject COMP2422P1 2019-04-20 26
  >__[addProject] is similar to [addAssignment]__. It is to add a subject project to the scheduler. Follows by the due date in the format [YYYY-MM-DD]. Then the duration is specified (hours).  
  >If you are considering to use "priority" as one of the algorithms that to be implemented in your application, [addProject] would have a __higher priority__ than [addAssignment].
  
- addRevision `[subject code]` `[date and time]` `[duration]`  
  eg.addRevision COMP2000 2019-04-14 19:00 2  
  >Revision date in the format __[YYYY-MM-DD hh:mm]__. Then the duration is specified (number of hours required).  
  >this task __should be done in one off__. If the task cannot be done in one go, simply reject it.  
  >__For considering "priority"__, the order looks like the following. __`[addProject]` -> `[addAssignment]` -> `[addRevision]` -> `[addActivity]`__.  

- addActivity `[name of the event]` `[date and time of the event]` `[duration]`
  eg.addActivity Meeting 2019-04-18 20:00 2
  >date and time of the event in the format [__YYYY-MM-DD hh:mm]__.
  
- addBatch `[filename]`
  eg.addBatch S3_tasks_01.dat
  > [addBatch] allows user to prepare a text file which contains multiple lines of different requests and import into the program.

- runS3 `[algorithm]` `[filename]`
  eg.runS3 FCFS S3_report_fcfs_01.dat
  >For the output file, for example S3_report_fcfs_01.dat, it is just a text file that include the time table and the analysis report (Output Module and Analyzer Module).  
  >In addition, the acceptance or rejection of a request should be recorded in a corresponding log file (stored in Input Module).

- exitS3
  > terminate the program properly.
