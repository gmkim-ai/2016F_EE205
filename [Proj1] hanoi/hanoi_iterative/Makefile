CC = gcc
OBJS = hanoi.o
TARGET = hanoi
 
OBJS = hanoi.o main.o
SRCS = hanoi.c main.c
 
all : $(TARGET)
 
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean :
	rm -f $(OBJS) $(TARGET)
