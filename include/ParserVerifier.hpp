#include <stack>
#include "../include/ThomasParser.hpp"

class ParserVerifier {
public:
	ParserVerifier(ThomasParser *par);
	void verify(ThomasNode* parent, ThomasNode* tree);
private:
	void error(std::string str);
	Token* getFirstTokenInTree(ThomasNode *tree);
	Token* getNextTokenInTree(ThomasNode *tree);
	ThomasParser *parser;
	std::stack<ThomasNode*> stack;
};

