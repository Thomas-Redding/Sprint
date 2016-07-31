all:
	g++ -g -stdlib=libc++ -std=c++0x src/main.cpp src/Tokenizer.cpp src/Sweetener.cpp src/ThomasParser.cpp -o sprint

clean:
	rm *o sprit

