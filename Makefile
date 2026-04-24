CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SANFLAGS = -fsanitize=address

OBJS = Item.o Warehouse.o PriceUtils.o Report.o

.PHONY: clean test_part1 test_part2 test_part3 test_part4 test_all

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SANFLAGS) -c $< -o $@

# Part 1: Preprocessor — can all headers be included?
test_part1:
	@echo "=== Part 1: Preprocessor ==="
	$(CXX) $(CXXFLAGS) -o test_part1 test_part1.cpp
	MallocNanoZone=0 ./test_part1

# Part 2: Compilation — does each .cpp compile to .o?
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

# Part 3: Linking — do all symbols resolve?
test_part3: $(OBJS) test_part3.o
	@echo "=== Part 3: Linking ==="
	$(CXX) $(SANFLAGS) -o test_part3 test_part3.o $(OBJS)
	MallocNanoZone=0 ./test_part3

# Part 4: Runtime — does the program run without memory errors?
test_part4: Item.o Warehouse.o
	@echo "=== Part 4: Runtime (AddressSanitizer) ==="
	@echo "--- Test 4a: Stack buffer overflow ---"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4a test_part4a.cpp Item.o \
		&& MallocNanoZone=0 ./test_p4a \
		|| echo "FAIL: Stack buffer overflow detected"
	@echo "--- Test 4b: Double free ---"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4b test_part4b.cpp Item.o \
		&& MallocNanoZone=0 ./test_p4b \
		|| echo "FAIL: Double free detected"
	@echo "--- Test 4c: Use-after-free ---"
	@$(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4c test_part4c.cpp Item.o Warehouse.o \
		&& MallocNanoZone=0 ./test_p4c \
		|| echo "FAIL: Use-after-free detected"

# Full test suite with pass/fail summary
test_all: $(OBJS) main.o
	@tmpfile=$$(mktemp); \
	echo "=== Running All Tests ==="; \
	\
	echo ""; \
	echo "--- Part 1: Preprocessor ---"; \
	{ $(CXX) $(CXXFLAGS) -o test_part1 test_part1.cpp 2>/dev/null \
	    && ./test_part1 \
	    || echo "FAIL: Part 1 failed to build"; } | tee -a $$tmpfile; \
	\
	echo ""; \
	echo "--- Part 2: Compilation ---"; \
	{ $(CXX) $(CXXFLAGS) $(SANFLAGS) -c Item.cpp -o Item.o 2>/dev/null \
	    && echo "PASS: Item.cpp compiles" \
	    || echo "FAIL: Item.cpp has compilation errors"; \
	  $(CXX) $(CXXFLAGS) $(SANFLAGS) -c PriceUtils.cpp -o PriceUtils.o 2>/dev/null \
	    && echo "PASS: PriceUtils.cpp compiles" \
	    || echo "FAIL: PriceUtils.cpp has compilation errors"; \
	  $(CXX) $(CXXFLAGS) $(SANFLAGS) -c Warehouse.cpp -o Warehouse.o 2>/dev/null \
	    && echo "PASS: Warehouse.cpp compiles" \
	    || echo "FAIL: Warehouse.cpp has compilation errors"; \
	  $(CXX) $(CXXFLAGS) $(SANFLAGS) -c Report.cpp -o Report.o 2>/dev/null \
	    && echo "PASS: Report.cpp compiles" \
	    || echo "FAIL: Report.cpp has compilation errors"; } | tee -a $$tmpfile; \
	\
	echo ""; \
	echo "--- Part 3: Linking ---"; \
	{ $(CXX) $(CXXFLAGS) $(SANFLAGS) -c test_part3.cpp -o test_part3.o 2>/dev/null \
	    && $(CXX) $(SANFLAGS) -o test_part3 test_part3.o $(OBJS) 2>/dev/null \
	    && MallocNanoZone=0 ./test_part3 \
	    || echo "FAIL: Part 3 failed to build or link"; } | tee -a $$tmpfile; \
	\
	echo ""; \
	echo "--- Part 4a: Stack buffer overflow ---"; \
	{ $(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4a test_part4a.cpp Item.o 2>/dev/null \
	    && MallocNanoZone=0 ./test_p4a \
	    || echo "FAIL: Stack buffer overflow detected"; } | tee -a $$tmpfile; \
	echo "--- Part 4b: Double free ---"; \
	{ $(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4b test_part4b.cpp Item.o 2>/dev/null \
	    && MallocNanoZone=0 ./test_p4b \
	    || echo "FAIL: Double free detected"; } | tee -a $$tmpfile; \
	echo "--- Part 4c: Use-after-free ---"; \
	{ $(CXX) $(CXXFLAGS) $(SANFLAGS) -o test_p4c test_part4c.cpp Item.o Warehouse.o 2>/dev/null \
	    && MallocNanoZone=0 ./test_p4c \
	    || echo "FAIL: Use-after-free detected"; } | tee -a $$tmpfile; \
	\
	echo ""; \
	echo "=== SUMMARY ==="; \
	pass=$$(grep "^PASS:" $$tmpfile | wc -l); \
	fail=$$(grep "^FAIL:" $$tmpfile | wc -l); \
	echo "Tests passed: $$pass"; \
	echo "Tests failed: $$fail"; \
	rm -f $$tmpfile; \
	test $$fail -eq 0

clean:
	rm -f *.o inventory test_part1 test_part3 test_p4a test_p4b test_p4c
