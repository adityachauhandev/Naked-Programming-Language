run:
	mkdir -p build
	g++ -Iinclude src/main.cpp src/scanner.cpp src/token.cpp src/parser.cpp -o build/compiler
	./build/compiler

clean:
	rm -rf build
