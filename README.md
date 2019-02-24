# COMP2432_Project

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
  >Date and time format is year-month-day and hour (__YYYY-MM-DD__ and __hh:mm__, simply take the hour and ignore the minute (i.e. mm=00)
and use the 24-hour).   
  >Actually, this should be __the first command__ for the application.
