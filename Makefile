all:
	g++ -g -stdlib=libc++ -std=c++0x src/main.cpp src/Tokenizer.cpp src/Sweetener.cpp src/ThomasParser.cpp src/ParserVerifier.cpp -o sprint

clean:
	rm *o; rm sprint; rm -r sprint.dSYM;
