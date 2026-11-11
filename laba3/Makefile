laba3: src/*.cpp main.cpp include/*.h
	g++ -std=c++20 -Wall -Iinclude src/*.cpp main.cpp -o laba3

lint:
	clang-tidy --quiet src/*.cpp main.cpp -- -std=c++20 -Iinclude 2>/dev/null

test: src/*.cpp tests/*.cpp include/*.h
	g++ -std=c++20 -Iinclude src/*.cpp tests/*.cpp -lgtest -lgtest_main -pthread -o run_tests
	./run_tests

coverage: src/*.cpp tests/*.cpp include/*.h
	rm -f *.gcda *.gcno
	g++ -std=c++20 --coverage -Iinclude src/*.cpp tests/*.cpp -lgtest -lgtest_main -pthread -o run_tests
	./run_tests
	mkdir -p coverage
	gcovr -r . --filter src/ --html-details coverage/index.html --print-summary

.PHONY: lint test coverage
bench: src/*.cpp benchmarks/*.cpp include/*.h
	g++ -std=c++20 -O2 -Iinclude src/*.cpp benchmarks/*.cpp -lbenchmark -pthread -o run_bench
	./run_bench