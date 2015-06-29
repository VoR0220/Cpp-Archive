#
# PROGRAM:    Assignment 2
# PROGRAMMER: RJ Catalano
# LOGON ID:   z1716703
# DATE DUE:   1-30-2015 by 11:59 pm
#

# Compiler variables

CCFLAGS = -ansi -Wall

# Rule to link object code files to create executable file
z1716703_project2: z1716703_project2.o
	g++ $(CCFLAGS) -o z1716703_project2 z1716703_project2.o

# Rules to compile source code files to object code
z1716703_project2.o: z1716703_project2.cpp
	g++ $(CCFLAGS) -c z1716703_project2.cpp

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o z1716703_project2


