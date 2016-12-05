
#include "../include/ParserVerifier.hpp"


ParserVerifier::ParserVerifier(ThomasParser *par) {
	parser = par;

	keywords_are_in_structures.insert({T_KEYWORD_CLASS, std::set<int>({class_implementation})});
	keywords_are_in_structures.insert({T_KEYWORD_ENUM, std::set<int>({enum_implementation})});
	keywords_are_in_structures.insert({T_KEYWORD_NAMESPACE, std::set<int>({namespace_implementation})});
	keywords_are_in_structures.insert({T_KEYWORD_ELSE, std::set<int>({if_else_statement})});
	keywords_are_in_structures.insert({T_KEYWORD_FOR, std::set<int>({for_loop})});
	keywords_are_in_structures.insert({T_KEYWORD_DO, std::set<int>({do_while_loop})});
	keywords_are_in_structures.insert({T_KEYWORD_WHILE, std::set<int>({do_while_loop})});
	keywords_are_in_structures.insert({T_KEYWORD_NEW, std::set<int>({unary2_clause})});
	keywords_are_in_structures.insert({T_KEYWORD_DELETE, std::set<int>({unary2_clause})});
	keywords_are_in_structures.insert({T_KEYWORD_SWITCH, std::set<int>({switch_statement})});
	keywords_are_in_structures.insert({T_KEYWORD_TRY, std::set<int>({try_block})});
	keywords_are_in_structures.insert({T_KEYWORD_CATCH, std::set<int>({catch_block})});
	// keywords_are_in_structures.insert({T_KEYWORD_NOT, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({T_KEYWORD_AND, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({T_KEYWORD_XOR, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({T_KEYWORD_OR, std::set<int>({unary2_clause})});

	keywords_are_eventually_in_structures.insert({T_KEYWORD_RETURN, std::set<int>({function_implementation})});
	keywords_are_eventually_in_structures.insert({T_KEYWORD_DEFAULT, std::set<int>({switch_statement})});
	keywords_are_eventually_in_structures.insert({T_KEYWORD_CASE, std::set<int>({switch_statement})});

	keywords_are_eventually_in_structures.insert({T_KEYWORD_BREAK, std::set<int>({for_loop, do_while_loop})});
	keywords_are_in_structures.insert({T_KEYWORD_IF, std::set<int>({if_statement, if_else_statement})});
	keywords_are_eventually_in_structures.insert({T_KEYWORD_CONTINUE, std::set<int>({if_statement, if_else_statement})});

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

void ParserVerifier::verify(ThomasNode* tree) {
	// verify current node is legal
	bool has_been_verified = false;


	std::unordered_map<int, std::set<int>>::iterator valid_parents_it = keywords_are_in_structures.find(tree->type);
	if (valid_parents_it != keywords_are_in_structures.end()) {
		if (valid_parents_it->second.find(ancestors.back()->type) == valid_parents_it->second.end()) {
			error("invalid parent (" + treeTypeToString(tree->type) + ")", tree);
		}
		has_been_verified = true;
	}

	if (!has_been_verified) {
		std::unordered_map<int, std::set<int>>::iterator valid_ancestors_it = keywords_are_eventually_in_structures.find(tree->type);
		if (valid_parents_it != keywords_are_eventually_in_structures.end()) {
			bool found_valid_ancestor = false;
			for (std::list<ThomasNode*>::reverse_iterator it = ancestors.rbegin(); it != ancestors.rend(); --it) {
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
	
	if (!has_been_verified) {
		if (tree->type == curly_brace_block) {
			if (structures_with_blocks_of_statements.find(ancestors.back()->type) != structures_with_blocks_of_statements.end()) {
				verify_block_contains_only_statements(tree);
				has_been_verified = true;
			}
			else if (ancestors.back()->type == switch_statement) {
				for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
					if (parser->shortcuts[structure_or_statement].find((*it)->type) == parser->shortcuts[structure_or_statement].end()) {
						if ((*it)->type != case_statement)
							error("block contains invalid line", *it);
					}
				}
				has_been_verified = true;
			}
			else if (ancestors.back()->type == class_implementation) {
				// todo
				has_been_verified = true;
			}
			else if (ancestors.back()->type == enum_implementation) {
				// todo
				has_been_verified = true;
			}
			else if (ancestors.back()->type == namespace_implementation) {
				// todo
				has_been_verified = true;
			}
		}
	}

	if (!has_been_verified) {
		if (tree->type == curly_brace_block || tree->type == bracket_block) {
			/*
				[int]					{int}
				[int:int]				{int: int}
				[x, y, z]				{x, y, z}
				[x:y, z:a, b:c]			{x:y, z:a, b:c}
			*/
		}
	}

	if (!has_been_verified) {
		if (tree->type == parenthesis_block) {
			/*
				(int, int, int)
				(x, y, z)
			*/
		}
	}

	// verify children recursively
	ancestors.push_back(tree);
	for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		verify(*it);
	}
	ancestors.pop_back();
};

void ParserVerifier::verify_block_contains_only_statements(ThomasNode* tree) {
	for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		if (parser->shortcuts[structure_or_statement].find((*it)->type) == parser->shortcuts[structure_or_statement].end())
			error("block contains invalid line", *it);
	}
}

void ParserVerifier::error(std::string message, ThomasNode* tree) {
	std::cout << message << " (" << tree->token.lineNum << ", " << tree->token.charNum << ")\n";
	exit(0);
}
