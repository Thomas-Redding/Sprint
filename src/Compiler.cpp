#include "../include/Compiler.hpp"

Compiler::Compiler() {

}

std::string Compiler::compile(ParseNode* tree) {
	output.clear();
	compile_children(tree);
	std::cout << "\n\n\n" << output << "\n\n\n";
	return output;
}

void Compiler::compile_tree(ParseNode* tree) {
	std::map<TreeType, std::string>::iterator simple_strings_it = simple_strings.find(tree->type);
	if (simple_strings_it != simple_strings.end()) {
		output += simple_strings_it->second;
		return;
	}

	if (just_compile_children.find(tree->type) != just_compile_children.end()) {
		compile_children(tree);
		return;
	}

	if (just_compile_space_and_str.find(tree->type) != just_compile_space_and_str.end()) {
		output += " ";
		output += tree->token.str;
		return;
	}

	if (tree->type == parenthesis) {
		output += "(";
		compile_children(tree);
		output += ")";
		return;
	}

	if (vanilla_curly_brace_block.find(tree->type) != vanilla_curly_brace_block.end()) {
		output += "{";
		compile_children(tree);
		output += "}";
		return;
	}

	if (tree->type == class_implementation) {
		compile_children(tree);
		output += ";";
		return;
	}

	if (tree->type == class_implementation) {
		compile_children(tree);
		output += ";";
		return;
	}

	if (tree->type == function_implementation) {
		ParseNode* func_head = tree->children.front();
		ParseNode* block = tree->children.back();
		int parts_of_return = tree->children.size() - 2;
		std::list<ParseNode*>::iterator it = tree->children.begin();
		++it;
		for (it = it; it != tree->children.end(); ++it) {
			compile_tree(*it);
			--parts_of_return;
			if (parts_of_return <= 0)
				break;
		}
		output += " ";
		compile_tree(func_head);
		compile_tree(block);
		return;
	}

	if (tree->type == function_head) {
		// compile everything except the '->'
		int counter = tree->children.size();
		for (std::list<ParseNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
			--counter;
			if (counter == 0)
				return;
			compile_tree(*it);			
		}
	}

	// todo
}

void Compiler::compile_children(ParseNode* tree) {
	for (std::list<ParseNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		compile_tree(*it);
	}
}

void Compiler::error(std::string message, ParseNode* tree) {
	std::cout << message << " (" << tree->token.lineNum << ", " << tree->token.charNum << ")\n";
	exit(0);
}
