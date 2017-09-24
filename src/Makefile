PROG = baobui
CXXFLAGS = -Wall -Werror -pedantic -std=c++11
CPPSOURCES = $(shell find -name '*.cpp')
INC = -I ../common/include
LOC_LIB = ../common/linux_x86_64/libGLEW.a -lglfw
SYS_LIB = -lGL

.PHONY : all clean

all:
	$(CXX) $(CXXFLAGS) -o $(PROG) $(CPPSOURCES) $(INC) $(LOC_LIB) $(SYS_LIB)

clean:
	rm -f *~ $(shell find -name '*.o') $(PROG)
