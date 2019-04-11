####################################
#   OS project                     #
#   contributors:                  #
#                  HAN Shanglin    #
#                  LYU Zhian       #
#                  LIAN Xiang      #
#                  XU Xiaochuan    #
#                  HE Jiashu       #
####################################
How to run:
        1. unzip the file
        2. use `gcc std=gnu99 -o projectS3 S3.c`  and `./projectS3`  to run the program
        3. you may enter `addBatch s3_data_1.dat` or `addBatch s3_data_2.dat` to import data file
        4. you can run the scheduling algorithm by enter:
                `runS3 [algorithm name] [filename]`
                algorithm name can be :
                        1.Priority
                        2.Deadline
                        3.FCFS
                        4.SDDL
                eg. `runS3 SDDL SDDL.out`
        5. use `exitS3` to exit the program

About the generated file:
        1. you can find an error log in "S3_error.log" (only if the input have some error)
        2. you can find algorithm log in "S3_XXXX.log"
