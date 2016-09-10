
#include "../include/ParserVerifier.hpp"

/*
 * we have to verify all (), {}, [], and <> blocks
 * idea: a "Rule" consists of
 *   1. a block-type
 *   2. a parent type
 *   3. approporiate children patterns
 * For instance, for while-loops, we might have something like
 * {curly_bracket, while_loop, statements}
 * Form this point, we just have to loop through all the nodes in the tree.
 * when we find a block, we check the parent, then we check if the block's contents are good
 */

ParserVerifier::ParserVerifier(ThomasParser *par) {
	parser = par;
}

Token* ParserVerifier::getFirstTokenInTree(ThomasNode *tree) {
	if (tree->type < general) {
		return &tree->token;
	}
	else {
		Token *rtn;
		for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
			rtn = getFirstTokenInTree(*it);
			if (rtn != nullptr)
				return rtn;
		}
		return getNextTokenInTree(tree);
	}
}

Token* ParserVerifier::getNextTokenInTree(ThomasNode *tree) {
	while(!stack.empty()) {
		bool shouldCheck = false;
		ThomasNode *par = stack.top();
		Token *rtn = nullptr;
		for (std::list<ThomasNode*>::iterator it = par->children.begin(); it != par->children.end(); ++it) {
			if (*it == tree)
				shouldCheck = true;
			else if (shouldCheck) {
				rtn = getFirstTokenInTree(*it);
				if (rtn != nullptr)
					return rtn;
			}
		}
		tree = par;
		stack.pop();
	}
	// there are no tokens next
	return nullptr;
}

void ParserVerifier::verify(ThomasNode* parent, ThomasNode* tree) {
	if (tree->type == curly_brace_block) {
		if (parent->type == if_else_statement || parent->type == if_statement || parent->type == while_loop || parent->type == for_loop || parent->type == do_while_loop || parent->type == function_implementation /* || parent->type == constructor_implementation */) {
			for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
				if (parser->shortcuts[structure_or_statement].find((*it)->type) == parser->shortcuts[structure_or_statement].end()) {
					stack.push(parent);
					stack.push(tree);
					Token* token = getFirstTokenInTree(*it);
					if (token == nullptr)
						error("Parser Error 1 (contact tfredding@gmail.com)");
					else
						error("Parser Error: found unexpected class, namespace, or function (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
				}
			}
		}
		else if (parent->type == switch_statement) {
			// todo
		}
		// todo: x = {"one": 1, "two": 2}
	}
	else if (tree->type == parenthesis_block) {
		if (parent->type == if_else_statement || parent->type == if_statement || parent->type == while_loop || parent->type == do_while_loop || parent->type == switch_statement) {
			if (tree->children.size() == 0) {
				error("Parser Error 2 (contact tfredding@gmail.com)");
			}
			else if (tree->children.size() > 1) {
				stack.push(parent);
				stack.push(tree);
				Token* token = getFirstTokenInTree(tree->children.front());
				if (token == nullptr)
					error("Parser Error 3 (contact tfredding@gmail.com)");
				else
					error("Parser Error: Expected a single statement in condition, but found multiple (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
			}
			else if (parser->shortcuts[setting_value].find(tree->children.front()->type) == parser->shortcuts[setting_value].end()) {
				stack.push(parent);
				stack.push(tree);
				Token* token = getFirstTokenInTree(tree->children.front());
				if (token == nullptr)
					error("Parser Error 4 (contact tfredding@gmail.com)");
				else
					error("Parser Error: Expected (and did not find) single statement in condition (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
			}
		}
		else if (parent->type == for_loop) {
			// for(x;y;z) <- x, y, and z are optional
			if (tree->children.size() == 2) {
				if (tree->children.front()->type == statement) {
					if (tree->children.back()->type == statement) {
						// for(x;y;)
					}
					else if (tree->children.back()->type == T_SEMI_COLON) {
						// for(x;;)
					}
					else {
						stack.push(parent);
						stack.push(tree);
						Token* token = getFirstTokenInTree(tree->children.back());
						if (token == nullptr)
							error("Parser Error 5 (contact tfredding@gmail.com)");
						else
							error("Parser Error: found unexpected object in for-loop (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
					}
				}
				else if (tree->children.front()->type == T_SEMI_COLON) {
					if (tree->children.back()->type == statement) {
						// for(;y;)
					}
					else if (tree->children.back()->type == T_SEMI_COLON) {
						// for(;;)
					}
					else {
						stack.push(parent);
						stack.push(tree);
						Token* token = getFirstTokenInTree(tree->children.back());
						if (token == nullptr)
							error("Parser Error 6 (contact tfredding@gmail.com)");
						else
							error("Parser Error: found unexpected object in for-loop (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
					}
				}
				else {
					stack.push(parent);
					stack.push(tree);
					Token* token = getFirstTokenInTree(tree->children.front());
					if (token == nullptr)
						error("Parser Error 7 (contact tfredding@gmail.com)");
					else
						error("Parser Error: found unexpected object in for-loop (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
				}
			}
			else if (tree->children.size() == 3) {
				ThomasNode *first = tree->children.front();
				auto it = tree->children.begin();
				++it;
				ThomasNode *second = *it;
				ThomasNode *third = tree->children.back();
				// for(x;y;z) <- x and y are optional; z is not
				if (first->type == statement) {
					// for(x;???)
					if (second->type == statement) {
						// for(x;y;???)
						if (parser->shortcuts[comma_value].find(third->type) == parser->shortcuts[comma_value].end()) {
							// for(x;y;z)
						}
						else {
							stack.push(parent);
							stack.push(tree);
							Token* token = getFirstTokenInTree(third);
							if (token == nullptr)
								error("Parser Error 8 (contact tfredding@gmail.com)");
							else
								error("Parser Error: found unexpected object in for-loop (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
						}
					}
					else if (second->type == T_SEMI_COLON) {
						// for(x;;???)
						if (parser->shortcuts[comma_value].find(third->type) == parser->shortcuts[comma_value].end()) {
							// for(x;;z)
						}
						else {
							stack.push(parent);
							stack.push(tree);
							Token* token = getFirstTokenInTree(third);
							if (token == nullptr)
								error("Parser Error 9 (contact tfredding@gmail.com)");
							else
								error("Parser Error: found unexpected object in for-loop (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
						}
					}
				}
				else if (first->type == T_SEMI_COLON) {
					// for(;???)
					if (second->type == statement) {
						// for(;y;???)
						if (parser->shortcuts[comma_value].find(third->type) == parser->shortcuts[comma_value].end()) {
							// for(;y;z)
						}
						else {
							stack.push(parent);
							stack.push(tree);
							Token* token = getFirstTokenInTree(third);
							if (token == nullptr)
								error("Parser Error 10 (contact tfredding@gmail.com)");
							else
								error("Parser Error: found unexpected object in for-loop (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
						}
					}
					else if (second->type == T_SEMI_COLON) {
						// for(;;???)
						if (parser->shortcuts[comma_value].find(third->type) == parser->shortcuts[comma_value].end()) {
							// for(;;z)
						}
						else {
							stack.push(parent);
							stack.push(tree);
							Token* token = getFirstTokenInTree(third);
							if (token == nullptr)
								error("Parser Error 11 (contact tfredding@gmail.com)");
							else
								error("Parser Error: found unexpected object in for-loop (line: " + std::to_string(token->lineNum) + ", char: " + std::to_string(token->charNum) + ", string: \"" + token->str + "\")");
						}
					}
				}
				else {
					//
				}
			}
		}
		else if (parent->type == function_implementation /* || parent->type == constructor_implementation */) {
			// todo
		}
		// todo: x(42);
	}
	else if (tree->type == bracket_block) {
		// todo
		/*
		 * square brackets are used in
		 *   1. declarations:		int[] x;
		 *   2. constructions		x = new int[];
		 *   3. literal constructs	x = [1, 2, 3];
		 *   4. accessors			x[5];
		 */
	}
	else if (tree->type == template_block) {
		// todo
	}

	stack.push(parent);
	for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		verify(tree, *it);
	}
	stack.pop();
}

void ParserVerifier::error(std::string str) {
	std::cout << str << "\n";
	exit(1);
}