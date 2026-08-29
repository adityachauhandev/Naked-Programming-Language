build:
	mkdir -p build
	g++ -O3 -Iinclude src/main.cpp src/scanner.cpp src/parser.cpp src/interpreter.cpp -o build/compiler

run: build
	./build/compiler

debug: build
	g++ -g -Iinclude src/main.cpp src/scanner.cpp src/parser.cpp src/interpreter.cpp -o build/compiler
	gdb ./build/compiler

clean:
	rm -rf build
