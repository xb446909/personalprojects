TARGET = webcontrol
MACRO = 
CFLAGS += -O2 -lpthread -std=gnu99
SOURCES = $(wildcard *.c)
OBJS := $(patsubst %.c, %.o,$(SOURCES))
CC = gcc

default: all

$(TARGET): $(OBJS)
	$(CC) $(OBJS)  -D$(MACRO) $(CFLAGS) -o $(TARGET)
	
sinclude $(SOURCES:.c=.d)

%d: %c
	@$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ ,g' < $@.$$$$ > $@; \
	rm $@.$$$$ 

.PHONY: clean all

clean:
	@rm -rf $(OBJS) $(TARGET) *.d

all: $(TARGET)
