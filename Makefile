CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g -fsanitize=address
LDFLAGS =  -fsanitize=address

OBJ = input.o log.o FCFS.o mytime.o record.o
EXEC = projectS3

$(EXEC): $(OBJ)
	$(CXX) -o $(EXEC) $(OBJ)

FCFS.o input.o log.o record.o: record.h
FCFS.o input.o log.o: log.h
$(OBJ): mytime.h
FCFS.o input.o: FCFS.h

.PHONY :clean
clean:
	rm -rf $(OBJ) $(EXEC)