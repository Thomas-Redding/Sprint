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
		ParseNode* head = tree->children.front();
		ParseNode* head_name = head->children.front();
		ParseNode* head_parentheses = nullptr;
		ParseNode* head_template = nullptr;
		if (head->children.size() == 3) {
			head_parentheses = at(head->children, 1);
		}
		else if (head->children.size() == 4) {
			head_template = at(head->children, 1);
			head_parentheses = at(head->children, 2);
		}

		ParseNode* block = tree->children.back();

		ParseNode* return_type = at(tree->children, 1);
		ParseNode* return_template = nullptr;
		if (tree->children.size() == 4) {
			return_template = at(tree->children, 2);
		}

		if (head_template != nullptr) {
			output += "\ntemplate ";
			compile_tree(head_template);
			output += "\n";
		}
		compile_tree(return_type);
		if (return_template != nullptr) {
			compile_tree(return_template);
		}
		compile_tree(head_name);
		compile_tree(head_parentheses);
		compile_tree(block);
		return;
	}

	if (tree->type == templates) {
		output += "<";
		compile_children(tree);
		output += ">";
	}

	std::cout << treeTypeToString(tree->type) << "\n";
}

ParseNode* Compiler::at(std::list<ParseNode*> lst, int index) {
	if (index >= lst.size())
		return nullptr;
	for (std::list<ParseNode*>::iterator it = lst.begin(); it != lst.end(); ++it) {
		--index;
		if (index < 0)
			return *it;
	}
	return nullptr;
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
