CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++11
SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES: .cpp=.o)
PROG = exe

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -f *~ *.o *.gch $(PROG)
