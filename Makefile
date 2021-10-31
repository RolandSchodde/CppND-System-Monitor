.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: test
test:
	
.PHONY: run
run: format build
	./build/monitor
	
.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: rebuild
rebuild: clean build

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build
