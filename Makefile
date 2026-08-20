run:
	mkdir -p build
	g++ -Iinclude src/main.cpp src/scanner.cpp src/token.cpp -o build/compiler
	./build/compiler

clean:
	rm -rf build
