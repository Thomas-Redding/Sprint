all:
	clang++ -stdlib=libc++ -std=c++0x src/main.cpp src/Tokenizer.cpp src/Sweetener.cpp -o sprint

clean:
	rm *o sprit

