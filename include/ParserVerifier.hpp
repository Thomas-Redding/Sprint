#include "../include/ThomasParser.hpp"

class ParserVerifier {
public:
	ParserVerifier(ThomasParser *par);
	void verify(ThomasNode* parent, ThomasNode* tree);
private:
	void error(std::string str);
	Token* getFirstTokenInTree(ThomasNode *tree);
	ThomasParser *parser;
};

