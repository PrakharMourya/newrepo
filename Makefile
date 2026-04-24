CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SANFLAGS =

OBJS = Item.o Warehouse.o PriceUtils.o Report.o

.PHONY: clean test test_part1 test_part2 test_part3 test_part4 test_all

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SANFLAGS) -c $< -o $@

test: test_all

test_part1:
	@echo "=== Part 1: Preprocessor ==="
	$(CXX) $(CXXFLAGS) -o test_part1 test_part1.cpp
	test_part1.exe

test_part2:
	@echo "=== Part 2: Compilation ==="
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -c Item.cpp -o Item.o \
		&& echo "PASS: Item.cpp compiles" \
		|| echo "FAIL: Item.cpp has compilation errors"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -c PriceUtils.cpp -o PriceUtils.o \
		&& echo "PASS: PriceUtils.cpp compiles" \
		|| echo "FAIL: PriceUtils.cpp has compilation errors"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -c Warehouse.cpp -o Warehouse.o \
		&& echo "PASS: Warehouse.cpp compiles" \
		|| echo "FAIL: Warehouse.cpp has compilation errors"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -c Report.cpp -o Report.o \
		&& echo "PASS: Report.cpp compiles" \
		|| echo "FAIL: Report.cpp has compilation errors"

test_part3: $(OBJS) test_part3.o
	@echo "=== Part 3: Linking ==="
	$(CXX) $(SANFLAGS) -o test_part3 test_part3.o $(OBJS)
	test_part3.exe

test_part4: Item.o Warehouse.o
	@echo "=== Part 4: Runtime ==="
	@echo "--- Test 4a: Stack buffer overflow ---"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4a test_part4a.cpp Item.o \
		&& test_p4a.exe \
		|| echo "FAIL: Stack buffer overflow detected"
	@echo "--- Test 4b: Double free ---"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4b test_part4b.cpp Item.o \
		&& test_p4b.exe \
		|| echo "FAIL: Double free detected"
	@echo "--- Test 4c: Use-after-free ---"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4c test_part4c.cpp Item.o Warehouse.o \
		&& test_p4c.exe \
		|| echo "FAIL: Use-after-free detected"

test_all: test_part1 test_part2 test_part3 test_part4
	@echo "=== All tests finished ==="

clean:
	del /Q *.o *.exe 2>nul
