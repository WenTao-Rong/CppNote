src=$(wildcard *.cpp)
#dir=$(notdir $(src))
#obj=$(patsubst %.cpp,%.o,$(dir))
target=$(patsubst %.cpp,%,$(src))
libs=-std=c++11
all:$(target)
$(target):%:%.cpp
	g++ $^ -o $@ $(lib)
.phony:clean
clean:
	rm $(target)
