CC = gcc -Wall
RM = rm -i
CP = cp
TCH = touch
TAR = tar -cvzf
readme = README
srcdir = ./
bindir = ../bin/
SRC = $(wildcard $(srcdir)*.c)
HEAD = $(wildcard $(srcdir)*.h)
SRCP = $(wildcard $(srcdir)*)
OBJ = $(subst $(srcdir), $(bindir), $(SRC:.c=.o))
PROG = main

all : $(PROG)

$(PROG) : $(OBJ)
	$(CC) $(OBJ) -o $(PROG)

../bin/%.o : ../src/%.c
	$(CC) -c $^ -o $@

.PHONY : clean

clean :
	$(RM) $(OBJ)

.PHONY : doc

restore :
	$(CP) $(SAV) $(srcdir)

.PHONY : give

give :
	$(TAR) ../Project_X1085033.tar.gz ./
