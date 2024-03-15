# Example makefile - you can use this in your own programs without crediting me.

# this is the compiler and flags
CC = gcc

# you can change the flags - here are some common ones:
# -g           - allows use of GDB to debug your code
# -Wall        - enables warnings (tells you when you're trying to do something bad..)
# -Werror      - treats warnings as errors (stops you from compiling something bad..)
# -std=X       - replace X with version (usually C99 or C11). Gets it to compile to a certain C standard
# I would consider the 3 I use below 'essential'.
CFLAGS = -Wall -Werror -g

# List of source files - assumes all .c files are for this program so be careful.
# if this doesn't work for you (if you have a messy directory!), you can type them out manually:
# SRCS = file1.c file2.c etc.
SRCS = $(wildcard *.c)

# List of object files - this makes itself from your SRCS
OBJS = $(SRCS:.c=.o)

# Name of the executable - YOU EDIT THIS BIT WITH YOUR EXECUTABLE NAME
TARGET = maze

# Build the executablet
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
# if you use the maths library, you will need to add -lm at the end, eg.
# 	$(CC) $(CFLAGS) -c $< -o $@ -lm
# this tells it to link the maths library.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the generated files
clean:
	rm -f $(OBJS) $(TARGET)