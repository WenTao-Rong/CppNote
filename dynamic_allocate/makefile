src=$(wildcard *.cpp)
target=$(patsubst %.cpp,%,$(src))
libs=-std=c++11
all:$(target)
$(target):%:%.cpp
	g++ $^ -o $@ $(libs)
.phony:clean
clean:
	rm $(target)
