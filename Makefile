all:
	clang++ -stdlib=libc++ -std=c++0x src/main.cpp src/Parser.cpp src/Tokenizer.cpp src/Compiler.cpp src/Sweetener.cpp -o sprint
	# g++ src/main.cpp src/Parser.cpp src/Tokenizer.cpp src/Compiler.cpp src/Sweetener.cpp src/ResourcePath.mm -o sprint

clean:
	rm *o sprit

