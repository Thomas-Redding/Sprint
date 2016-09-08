
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

void ParserVerifier::verify(ThomasNode* parent, ThomasNode* tree) {
	if (tree->type == curly_brace_block) {
		if (parent->type == if_else_statement) {
			// todo
		}
		else if (parent->type == if_statement) {
			// todo
		}
		else if (parent->type == while_loop) {
			// todo
		}
		else if (parent->type == for_loop) {
			// todo
		}
		else if (parent->type == do_while_loop) {
			// todo
		}
		else if (parent->type == switch_statement) {
			// todo
		}
	}
	else if (tree->type == parenthesis_block) {
		if (parent->type == if_else_statement) {
			// todo
		}
		else if (parent->type == if_statement) {
			// todo
		}
		else if (parent->type == while_loop) {
			// todo
		}
		else if (parent->type == for_loop) {
			// todo
		}
		else if (parent->type == do_while_loop) {
			// todo
		}
		else if (parent->type == switch_statement) {
			// todo
		}
	}
	else if (tree->type == bracket_block) {
	}
	else if (tree->type == template_block) {
		//
	}

	for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		verify(tree, *it);
	}
}

void ParserVerifier::error(std::string str) {
	std::cout << str << "\n";
	exit(1);
}