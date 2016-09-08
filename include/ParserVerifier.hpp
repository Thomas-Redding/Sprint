#include "../include/ThomasParser.hpp"

class ParserVerifier {
public:
	void verify(ThomasNode* parent, ThomasNode* tree);
private:
	void error(std::string str);
};

