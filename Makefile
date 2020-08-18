src=$(wildcard *.cpp Base/*.cpp)
obj=$(patsubst %.cpp,%.o,$(src))
All:main.out

main.out:$(obj)
	g++ -g -rdynamic $^ -o $@ -lpthread
%.o:%.cpp
	g++ -g -Wall -rdynamic -c $< -o $@  


.PHONY:
clean:
	rm -rf main.out $(obj)


