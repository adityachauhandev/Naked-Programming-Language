run:
	mkdir -p build
	g++ -Iinclude src/main.cpp src/scanner.cpp src/token.cpp src/parser.cpp -o build/compiler
	./build/compiler

debug:
	mkdir -p build
	g++ -g -Iinclude src/main.cpp src/scanner.cpp src/token.cpp src/parser.cpp -o build/compiler
	gdb ./build/compiler

clean:
	rm -rf build
