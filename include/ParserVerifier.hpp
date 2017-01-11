#pragma once


#include <list>
#include <unordered_map>
#include <set>
#include "../include/Parser.hpp"

class ParserVerifier {
public:
	ParserVerifier(Parser *par);
	void verify(ParseNode* tree);
private:
	Parser *parser;
	std::unordered_map<int, std::set<int>> keywords_are_in_structures;
	std::unordered_map<int, std::set<int>> keywords_are_eventually_in_structures;
	std::set<int> structures_with_blocks_of_statements;

	std::list<ParseNode*> ancestors;
	void error(std::string message, ParseNode* tree);
	void verify_block_contains_only_statements(ParseNode* tree);
	ParseNode* is_proper_map(ParseNode* tree);
	ParseNode* is_proper_set_or_list(ParseNode* tree);
	void verify_children_recursively(ParseNode* tree);
};