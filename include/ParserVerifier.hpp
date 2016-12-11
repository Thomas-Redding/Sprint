#include <list>
#include <unordered_map>
#include <set>
#include "../include/ThomasParser.hpp"


class ParserVerifier {
public:
	ParserVerifier(ThomasParser *par);
	void verify(ThomasNode* tree);
private:
	ThomasParser *parser;
	std::unordered_map<int, std::set<int>> keywords_are_in_structures;
	std::unordered_map<int, std::set<int>> keywords_are_eventually_in_structures;
	std::set<int> structures_with_blocks_of_statements;

	std::list<ThomasNode*> ancestors;
	void error(std::string message, ThomasNode* tree);
	void verify_block_contains_only_statements(ThomasNode* tree);
	ThomasNode* is_proper_map(ThomasNode* tree);
	ThomasNode* is_proper_set_or_list(ThomasNode* tree);
};
