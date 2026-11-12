laba3: src/*.cpp main.cpp include/*.h
	g++ -std=c++20 -Wall -Iinclude src/*.cpp main.cpp -o laba3

lint:
	clang-tidy --quiet src/*.cpp main.cpp -- -std=c++20 -Iinclude 2>/dev/null

# ---------- Google Test ----------
test: src/*.cpp tests/gtest/*.cpp include/*.h
	g++ -std=c++20 -Iinclude src/*.cpp tests/gtest/*.cpp -lgtest -lgtest_main -pthread -o run_gtest
	./run_gtest

coverage: src/*.cpp tests/gtest/*.cpp include/*.h
	rm -f *.gcda *.gcno
	g++ -std=c++20 --coverage -Iinclude src/*.cpp tests/gtest/*.cpp -lgtest -lgtest_main -pthread -o run_gtest
	./run_gtest
	mkdir -p coverage/gtest
	gcovr -r . --filter src/ --html-details coverage/gtest/index.html --print-summary

# ---------- Boost Test ----------
test_boost: src/*.cpp tests/boost/*.cpp include/*.h
	g++ -std=c++20 -Iinclude -Itests src/*.cpp tests/boost/*.cpp -o run_boost
	./run_boost

coverage_boost: src/*.cpp tests/boost/*.cpp include/*.h
	rm -f *.gcda *.gcno
	g++ -std=c++20 --coverage -Iinclude -Itests src/*.cpp tests/boost/*.cpp -o run_boost
	./run_boost
	mkdir -p coverage/boost
	gcovr -r . --filter src/ --html-details coverage/boost/index.html --print-summary

# ---------- Catch2 ----------
test_catch2: src/*.cpp tests/catch2/*.cpp include/*.h
	g++ -std=c++20 -Iinclude -Itests src/*.cpp tests/catch2/*.cpp -lCatch2Main -lCatch2 -o run_catch2
	./run_catch2

coverage_catch2: src/*.cpp tests/catch2/*.cpp include/*.h
	rm -f *.gcda *.gcno
	g++ -std=c++20 --coverage -Iinclude -Itests src/*.cpp tests/catch2/*.cpp -lCatch2Main -lCatch2 -o run_catch2
	./run_catch2
	mkdir -p coverage/catch2
	gcovr -r . --filter src/ --html-details coverage/catch2/index.html --print-summary

# ---------- Benchmark ----------
bench: src/*.cpp benchmarks/*.cpp include/*.h
	g++ -std=c++20 -O2 -Iinclude src/*.cpp benchmarks/*.cpp -lbenchmark -pthread -o run_bench
	./run_bench

.PHONY: lint test coverage test_boost coverage_boost test_catch2 coverage_catch2 bench