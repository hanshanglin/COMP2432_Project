CXX = gcc
CC = gcc
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g -fsanitize=address
CFLAGS = -std=c99

OBJ = input.o log.o FCFS.o mytime.o record.o DDL.o list.o Priority.o
EXEC = projectS3

$(EXEC): $(OBJ)
	$(CXX) -o $(EXEC) $(OBJ)

Priority.o FCFS.o input.o log.o record.o DDL.o list.o: record.h
FCFS.o input.o log.o DDL.o: log.h
$(OBJ): mytime.h
FCFS.o input.o: FCFS.h
DDL.o input.o: DDL.h
Priority.o list.o :list.h
Priority.o :Priority.h
.PHONY :clean
clean:
	rm -rf $(OBJ) $(EXEC)