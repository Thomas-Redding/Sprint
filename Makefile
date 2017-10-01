
all:
	g++ -std=c++0x -o sprint src/Compiler.cpp src/main.cpp src/Parser.cpp src/ParserVerifier.cpp src/ScopeVerifier.cpp src/Sweetener.cpp src/Tokenizer.cpp

clean:
	rm *o; rm sprint; rm -r sprint.dSYM;
