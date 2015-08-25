TARGET = udpserver
MACRO = 
CFLAGS += -O2 -lpthread
SOURCES = $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o,$(SOURCES))
CC = g++

default: all

$(TARGET): $(OBJS)
	$(CC) $(OBJS)  -D$(MACRO) $(CFLAGS) -o $(TARGET)
	
sinclude $(SOURCES:.cpp=.d)

%d: %c
	@$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ ,g' < $@.$$$$ > $@; \
	rm $@.$$$$ 

.PHONY: clean all

clean:
	@rm -rf $(OBJS) $(TARGET) *.d

all: $(TARGET)
