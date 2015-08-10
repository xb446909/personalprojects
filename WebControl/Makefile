CC=gcc
TARGET=webcontrol
CFLAGS=-g

OBJ=main.o process.o

default: all

$(OBJ): %.o: %.c
	@$(CC) $< -c

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

.PHONY: clean all

clean:
	@rm $(OBJ) $(TARGET) -f
	
all:$(TARGET)


