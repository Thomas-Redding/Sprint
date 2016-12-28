all:
	g++ -g -stdlib=libc++ -std=c++0x src/main.cpp src/Tokenizer.cpp src/Sweetener.cpp src/Parser.cpp src/ParserVerifier.cpp src/Compiler.cpp src/TemplateDuplicator.cpp src/ScopeVerifier.cpp -o sprint

clean:
	rm *o; rm sprint; rm -r sprint.dSYM;
