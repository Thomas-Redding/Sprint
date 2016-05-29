all:
	g++ -g -stdlib=libc++ -std=c++0x src/main.cpp src/Parser.cpp src/Tokenizer.cpp src/Sweetener.cpp src/Parser_Helper.cpp src/Brace.cpp -o sprint

clean:
	rm *o sprit

