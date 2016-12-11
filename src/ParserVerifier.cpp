
#include "../include/ParserVerifier.hpp"


ParserVerifier::ParserVerifier(ThomasParser *par) {
	parser = par;

	
	keywords_are_in_structures.insert({T_KEYWORD_CATCH, std::set<int>({catch_block})});
	keywords_are_in_structures.insert({T_KEYWORD_CLASS, std::set<int>({class_implementation})});
	keywords_are_in_structures.insert({T_KEYWORD_DELETE, std::set<int>({unary2_clause})});
	keywords_are_in_structures.insert({T_KEYWORD_DO, std::set<int>({do_while_loop})});
	keywords_are_in_structures.insert({T_KEYWORD_ELSE, std::set<int>({if_else_statement})});
	keywords_are_in_structures.insert({T_KEYWORD_ENUM, std::set<int>({enum_implementation})});
	keywords_are_in_structures.insert({T_KEYWORD_FOR, std::set<int>({for_loop})});
	keywords_are_in_structures.insert({T_KEYWORD_IF, std::set<int>({if_statement, if_else_statement})});
	keywords_are_in_structures.insert({T_KEYWORD_NAMESPACE, std::set<int>({namespace_implementation})});
	keywords_are_in_structures.insert({T_KEYWORD_NEW, std::set<int>({unary2_clause})});
	keywords_are_in_structures.insert({T_KEYWORD_SWITCH, std::set<int>({switch_statement})});
	keywords_are_in_structures.insert({T_KEYWORD_TRY, std::set<int>({try_block})});
	keywords_are_in_structures.insert({T_KEYWORD_WHILE, std::set<int>({do_while_loop})});
	
	// keywords_are_in_structures.insert({T_KEYWORD_NOT, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({T_KEYWORD_AND, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({T_KEYWORD_XOR, std::set<int>({unary2_clause})});
	// keywords_are_in_structures.insert({T_KEYWORD_OR, std::set<int>({unary2_clause})});

	keywords_are_eventually_in_structures.insert({T_KEYWORD_BREAK, std::set<int>({for_loop, do_while_loop})});
	keywords_are_eventually_in_structures.insert({T_KEYWORD_CASE, std::set<int>({switch_statement})});
	keywords_are_eventually_in_structures.insert({T_KEYWORD_CONTINUE, std::set<int>({if_statement, if_else_statement})});
	keywords_are_eventually_in_structures.insert({T_KEYWORD_DEFAULT, std::set<int>({switch_statement})});
	keywords_are_eventually_in_structures.insert({T_KEYWORD_RETURN, std::set<int>({function_implementation})});

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

	// check for curly-brace blocks
	if (!has_been_verified) {
		if (tree->type == curly_brace_block) {
			has_been_verified = true;
			if (structures_with_blocks_of_statements.find(ancestors.back()->type) != structures_with_blocks_of_statements.end()) {
				verify_block_contains_only_statements(tree);
			}
			else if (ancestors.back()->type == switch_statement) {
				for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
					if (parser->shortcuts[structure_or_statement].find((*it)->type) == parser->shortcuts[structure_or_statement].end()) {
						if ((*it)->type != case_statement)
							error("block contains invalid line", *it);
					}
				}
			}
			else if (ancestors.back()->type == class_implementation) {
				// todo
			}
			else if (ancestors.back()->type == enum_implementation) {
				// todo
			}
			else if (ancestors.back()->type == namespace_implementation) {
				// todo
			}
			else if (0 == 1) {
				// todo
				// {int}
				// {int: int}
			}
			else {
				// this node is either a set or unordered map
				if (tree->children.size() == 0) {
					// empty set
				}
				else if (tree->children.size() != 1) {
					error("Unordered map or set literal parsed incorrectly", tree);
				}
				else {
					ThomasNode* only_child = *tree->children.begin();
					if (only_child->type == T_COLON) {
						// empty unordered map
					}
					else if (only_child->type == colon_clause) {
						// singleton unordered map
					}
					else if (only_child->type == comma_clause) {
						TreeType first_type = (*only_child->children.begin())->type;
						if (first_type == colon_clause) {
							// possibly an unordered map:     {foo: bar, ...}
							ThomasNode* result = is_proper_map(only_child);
							if (result != nullptr) {
								error("Improperly formated unordered map literal", result);
							}
						}
						else if (parser->shortcuts[comma_value].find(first_type) != parser->shortcuts[comma_value].end()) {
							// possibly a set:     {foo, ...}
							ThomasNode* result = is_proper_set_or_list(only_child);
							if (result != nullptr) {
								error("Improperly formated set literal", result);
							}
						}
						else {
							error("First term in set or unordered map literal is improperly formatted", tree);
						}
					}
					else if (parser->shortcuts[comma_value].find(only_child->type) != parser->shortcuts[comma_value].end()) {
						// singleotn set
					}
					else {
						error("First term in set or unordered map literal is improperly formatted", tree);
					}
				}
			}
		}
	}

	if (!has_been_verified) {
		if (tree->type == bracket_block) {
			if (0 == 1) {
				// todo
				// [int]
				// [int: int]
			}
			else {
				// this node is either a list or ordered map
				if (tree->children.size() == 0) {
					// empty list
				}
				else if (tree->children.size() != 1) {
					error("Ordered map or list literal parsed incorrectly", tree);
				}
				else {
					ThomasNode* only_child = *tree->children.begin();
					if (only_child->type == T_COLON) {
						// empty ordered map
					}
					else if (only_child->type == colon_clause) {
						// singleton ordered map
					}
					else if (only_child->type == comma_clause) {
						TreeType first_type = (*only_child->children.begin())->type;
						if (first_type == colon_clause) {
							// possibly an ordered map:     [foo: bar, ...]
							ThomasNode* result = is_proper_map(only_child);
							if (result != nullptr) {
								error("Improperly formated ordered map literal", result);
							}
						}
						else if (parser->shortcuts[comma_value].find(first_type) != parser->shortcuts[comma_value].end()) {
							// possibly a list:     [foo, ...]
							ThomasNode* result = is_proper_set_or_list(only_child);
							if (result != nullptr) {
								error("Improperly formated list literal", result);
							}
						}
						else {
							error("First term in list or ordered map literal is improperly formatted", tree);
						}
					}
					else if (parser->shortcuts[comma_value].find(only_child->type) != parser->shortcuts[comma_value].end()) {
						// singleotn set
					}
					else {
						error("First term in list or ordered map literal is improperly formatted", tree);
					}
				}
			}
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

/*
 * returns nullptr if input is proper
 * otherwise returns the first improper token
 */
ThomasNode* ParserVerifier::is_proper_map(ThomasNode* tree) {
	std::list<ThomasNode*>::iterator it = tree->children.begin();
	if ((*it)->type != colon_clause)
		return *it;
	++it;
	++it;
	if ((*it)->type == comma_clause)
		return is_proper_map(*it);
	else if ((*it)->type == colon_clause)
		return nullptr; // end of literal
	else
		return tree;
}

/*
 * returns nullptr if input is proper
 * otherwise returns the first improper token
 */
ThomasNode* ParserVerifier::is_proper_set_or_list(ThomasNode* tree) {
	std::list<ThomasNode*>::iterator it = tree->children.begin();
	if (parser->shortcuts[comma_value].find((*it)->type) == parser->shortcuts[comma_value].end())
		return *it;
	++it;
	++it;
	if ((*it)->type == comma_clause)
		return is_proper_set_or_list(*it);
	else if (parser->shortcuts[comma_value].find((*it)->type) == parser->shortcuts[comma_value].end())
		return nullptr; // end of literal
	else
		return tree;
}

/*
 * returns nullptr if input is proper
 * otherwise returns the first improper token
 */

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
