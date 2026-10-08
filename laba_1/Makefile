CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = main.cpp src/*.cpp

dbms: $(SRC) include/*.h
	$(CXX) $(CXXFLAGS) $(SRC) -o dbms

run: dbms
	./dbms --file data.txt --query 'PRINT mystack'

clean:
	rm -f dbms

.PHONY: run clean