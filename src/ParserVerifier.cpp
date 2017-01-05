
#include "../include/ParserVerifier.hpp"

ParserVerifier::ParserVerifier(Parser *par) {
	parser = par;

	
	keywords_are_in_structures.insert({P_KEYWORD_CATCH, std::set<int>({catch_block})});
	keywords_are_in_structures.insert({P_KEYWORD_CLASS, std::set<int>({class_implementation})});
	keywords_are_in_structures.insert({P_KEYWORD_DO, std::set<int>({do_while_loop})});
	keywords_are_in_structures.insert({P_KEYWORD_ELSE, std::set<int>({if_else_statement})});
	keywords_are_in_structures.insert({P_KEYWORD_ENUM, std::set<int>({enum_implementation})});
	keywords_are_in_structures.insert({P_KEYWORD_FOR, std::set<int>({for_loop})});
	keywords_are_in_structures.insert({P_KEYWORD_IF, std::set<int>({if_statement, if_else_statement})});
	keywords_are_in_structures.insert({P_KEYWORD_NAMESPACE, std::set<int>({namespace_implementation})});
	keywords_are_in_structures.insert({P_KEYWORD_SWITCH, std::set<int>({switch_statement})});
	keywords_are_in_structures.insert({P_KEYWORD_TRY, std::set<int>({try_block})});
	keywords_are_in_structures.insert({P_KEYWORD_WHILE, std::set<int>({do_while_loop})});
	
	// keywords_are_in_structures.insert({P_KEYWORD_NOT, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({P_KEYWORD_AND, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({P_KEYWORD_XOR, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({P_KEYWORD_OR, std::set<int>({unary2_clause})});

	keywords_are_eventually_in_structures.insert({P_KEYWORD_BREAK, std::set<int>({for_loop, do_while_loop})});
	keywords_are_eventually_in_structures.insert({P_KEYWORD_CASE, std::set<int>({switch_statement})});
	keywords_are_eventually_in_structures.insert({P_KEYWORD_CONTINUE, std::set<int>({if_statement, if_else_statement})});
	keywords_are_eventually_in_structures.insert({P_KEYWORD_DEFAULT, std::set<int>({switch_statement})});
	keywords_are_eventually_in_structures.insert({P_KEYWORD_RETURN, std::set<int>({function_implementation})});

	structures_with_blocks_of_statements = {
		function_implementation,
		for_loop,
		while_loop,
		do_while_loop,
		if_statement,
		if_else_statement,
		try_block,
		catch_block
	};
};

void ParserVerifier::verify(ParseNode* tree) {
	// verify current node is legal
	verify_children_recursively(tree);
	bool has_been_verified = false;

	// if the node is a relevant keyword, check that it actually has the parent it should have
	std::unordered_map<int, std::set<int>>::iterator valid_parents_it = keywords_are_in_structures.find(tree->type);
	if (valid_parents_it != keywords_are_in_structures.end()) {
		if (valid_parents_it->second.find(ancestors.back()->type) == valid_parents_it->second.end()) {
			error("invalid parent (" + treeTypeToString(tree->type) + ")", tree);
		}
		has_been_verified = true;
	}

	// if the node is a relevant keyword, check that it actually has the ancestor it should have
	if (!has_been_verified) {
		std::unordered_map<int, std::set<int>>::iterator valid_ancestors_it = keywords_are_eventually_in_structures.find(tree->type);
		if (valid_parents_it != keywords_are_eventually_in_structures.end()) {
			bool found_valid_ancestor = false;
			for (std::list<ParseNode*>::reverse_iterator it = ancestors.rbegin(); it != ancestors.rend(); --it) {
				if (valid_ancestors_it->second.find((*it)->type) != valid_parents_it->second.end()) {
					found_valid_ancestor = true;
					break;
				}
			}
			if (!found_valid_ancestor) {
				error("invalid ancestor", tree);
			}
			has_been_verified = true;
		}
	}
};

void ParserVerifier::verify_children_recursively(ParseNode* tree) {
	ancestors.push_back(tree);
	for (std::list<ParseNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		verify(*it);
	}
	ancestors.pop_back();
}

void ParserVerifier::error(std::string message, ParseNode* tree) {
	std::cout << message << " (" << tree->token.lineNum << ", " << tree->token.charNum << ")\n";
	exit(0);
}
