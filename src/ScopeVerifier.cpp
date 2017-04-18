#include <string>
#include <iostream>
#include <vector>
#include <assert.h>

#include "../include/ScopeVerifier.hpp"

std::string pad(uint64_t n) {
    std::string rtn = "";
    for (uint64_t i = 0; i < n; ++i) {
        rtn += " ";
    }
    return rtn;
}

// construct scope tree
ScopeNode* build_scope_tree(ParseNode* root) {
	ScopeNode* n = new ScopeNode(ScopeType::_global, root, nullptr, new std::vector<TemplateDec>());
	return n;
}

// doesn't include list_literal or unordered_map_literal
bool is_primitive(TreeType type) {
    return type == P_KEYWORD_INT || type == P_KEYWORD_INT8 || type == P_KEYWORD_INT16 || type == P_KEYWORD_INT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_UINT8 || type == P_KEYWORD_UINT16 || type == P_KEYWORD_UINT32 || type == P_KEYWORD_CHAR || type == P_KEYWORD_BOOL || type == P_KEYWORD_FLOAT || type == P_KEYWORD_DOUBLE || type == P_KEYWORD_VAR;
}

std::string scopeTypeToString(ScopeType type) {
    switch (type) {
        case ScopeType::_class: return "ScopeType::class";
        case ScopeType::_namespace: return "ScopeType::namespace";
        case ScopeType::_enum: return "ScopeType::enum";
        case ScopeType::_function: return "ScopeType::function";
        case ScopeType::_global: return "ScopeType::global";
    }
}

ScopeNode::ScopeNode(ScopeType type, ParseNode* parseNode, ScopeNode* parent, vector<TemplateDec>* templates)
: type(type), parseNode(parseNode), parent(parent), templates(templates), template_owner(false) {

	/*
	- by default, we 'inherit' the templates from our parents
	- we still need to initialize 'this->name'
	- we also need to find:
		- children
		- members
		- friends   (if a class)
		- arguments (if a function)
    */

	switch (type) {
		case ScopeType::_global: construct_global_node(); break;
		case ScopeType::_namespace: construct_namespace_node(); break;
		case ScopeType::_function: construct_function_node(); break;
		case ScopeType::_class: construct_class_node(); break;
		case ScopeType::_enum: construct_enum_node(); break;
	}
}

void ScopeNode::construct_global_node() {

	name = "[GLOBAL]"; // naming 'global' seems silly...

	for (auto it = parseNode->children.begin(); it != parseNode->children.end(); ++it) {
		ParseNode* node = *it;
		if (node->type == TreeType::variable_dec) {
			std::pair<std::string, TypeInst> x = TypeInst::get_variable(this, node);
			if (members.count(x.first) > 0) {
				user_error("duplicate member \"" + x.first + "\" found in \"" + name + "\"");
			}
			std::cout << "adding member variable \"" << x.first << "\"" << std::endl;
			members[x.first] = x.second;
		}
		else if (node->type == TreeType::class_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_class, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate class-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding class \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::function_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_function, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate function-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding function \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::namespace_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_namespace, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate namespace-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding namespace \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::enum_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_enum, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate enum-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding enum \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else {
			user_error("expected a class, function, enum declaration, variable, or namespace while parsing the GLOBAL namespace, but didn't find any.");
		}
	}

}

void ScopeNode::construct_namespace_node() {

	auto it = parseNode->children.begin();

	assert((*it)->type == TreeType::P_KEYWORD_NAMESPACE);
	++it;
	assert((*it)->type == TreeType::P_IDENTIFIER);
	name = (*it)->token.str;
	++it;
	assert((*it)->type == TreeType::block_of_statements_or_class);

	const auto end = (*it)->children.end();

	for (it = (*it)->children.begin(); it != end; ++it) {
		ParseNode* node = *it;
		if (node->type == TreeType::variable_dec) {
			std::pair<std::string, TypeInst> x = TypeInst::get_variable(this, node);
			if (members.count(x.first) > 0) {
				user_error("duplicate member \"" + x.first + "\" found in \"" + name + "\"");
			}
			std::cout << "adding member variable \"" << x.first << "\"" << std::endl;
			members[x.first] = x.second;
		}
		else if (node->type == TreeType::class_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_class, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate class-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding class \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::function_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_function, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate function-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding function \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::namespace_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_namespace, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate namespace-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding namespace \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::enum_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_enum, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate enum-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding enum \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else {
			user_error("expected a class, function, enum declaration, variable, or namespace while parsing the \"" + name + "\" namespace, but didn't find any.");
		}
	}

}

void ScopeNode::construct_function_node() {

	auto it = parseNode->children.begin();
	ParseNode* head = *it;
	++it;
	ParseNode* return_type = *it;
	++it;
	ParseNode* return_template = nullptr;
	if ((*it)->type == TreeType::templates) {
		return_template = *it;
		++it;
	}
	ParseNode* block = *it;

	assert(head->children.front()->type == P_IDENTIFIER);
	name = head->children.front()->token.str;

	for (it = block->children.begin(); it != block->children.end(); ++it) {
		ParseNode* node = *it;
		if (node->type == TreeType::variable_dec) {
			std::pair<std::string, TypeInst> x = TypeInst::get_variable(this, node);
			if (members.count(x.first) > 0) {
				user_error("duplicate member \"" + x.first + "\" found in \"" + name + "\"");
			}
			std::cout << "adding member variable \"" << x.first << "\"" << std::endl;
			members[x.first] = x.second;
		}
		else if (node->type == TreeType::class_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_class, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate class-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding class \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::function_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_function, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate function-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding function \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::namespace_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_namespace, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate namespace-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding namespace \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::enum_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_enum, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate enum-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding enum \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else {
			// TODO: include other control blocks that affect scoping
		}
	}
}

void ScopeNode::construct_class_node() {

	// these are set below
	ParseNode* _templates = nullptr; // not set if it doesn't exist
	ParseNode* block = nullptr;

	auto it = parseNode->children.begin();
	assert((*it)->type == TreeType::P_KEYWORD_CLASS);
	++it;
	assert((*it)->type == TreeType::P_IDENTIFIER);
	name = (*it)->token.str;
	++it;
	if ((*it)->type == TreeType::templates) {
		_templates = *it;
		++it;
	}
	assert((*it)->type == TreeType::block_of_statements_or_class);
	block = *it;

	for (it = block->children.begin(); it != block->children.end(); ++it) {
		ParseNode* node = *it;
		if (node->type == TreeType::variable_dec) {
			std::pair<std::string, TypeInst> x = TypeInst::get_variable(this, node);
			if (members.count(x.first) > 0) {
				user_error("duplicate member \"" + x.first + "\" found in \"" + name + "\"");
			}
			std::cout << "adding member variable \"" << x.first << "\"" << std::endl;
			members[x.first] = x.second;
		}
		else if (node->type == TreeType::class_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_class, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate class-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding class \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::function_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_function, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate function-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding function \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::namespace_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_namespace, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate namespace-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding namespace \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else if (node->type == TreeType::enum_implementation) {
			ScopeNode* child = new ScopeNode(ScopeType::_enum, node, this, templates);
			if (members.count(child->name) > 0) {
				user_error("duplicate enum-member \"" + child->name + "\" found in \"" + name + "\"");
			}
			std::cout << "adding enum \"" << child->name << "\"" << std::endl;
			children[child->name] = child;
		}
		else {
			user_error("expected a class, function, enum declaration, variable, or namespace while parsing the class " + name + ", but didn't find any.");
		}
	}

	// TODO: parse 'templates' and 'block'

}

void ScopeNode::construct_enum_node() {
	
	auto it = parseNode->children.begin();
	++it;
	assert((*it)->type == TreeType::P_IDENTIFIER);
	name = (*it)->token.str;
	++it;
	assert((*it)->type == TreeType::enum_block);

	if ((*it)->children.size() == 1) {
		// only one enum value
		arguments.push_back(std::pair<std::string, ScopeNode*>((*it)->children.front()->token.str, nullptr));
	}
	else {
		it = (*it)->children.begin(); // after this (*it)->type == comma_clause
		while ((*it)->children.back()->type == TreeType::comma_clause) {
			assert((*it)->children.front()->type == TreeType::P_IDENTIFIER);
			arguments.push_back(std::pair<std::string, ScopeNode*>((*it)->children.front()->token.str, nullptr));
			it = ++++((*it)->children.begin());
		}
		arguments.push_back(std::pair<std::string, ScopeNode*>((*it)->children.front()->token.str, nullptr));
		arguments.push_back(std::pair<std::string, ScopeNode*>((*it)->children.back()->token.str, nullptr));
	}
}

void ScopeNode::add_templates(ParseNode* node) {
	template_owner = false;
	// TODO
}

// static function
std::pair<std::string, TypeInst> TypeInst::get_variable(ScopeNode* parent, ParseNode* root) {

	assert(root->type == TreeType::variable_dec);
	TypeInst member;
	std::string name;

	auto it = root->children.begin();
	if ((*it)->type == TreeType::P_KEYWORD_MUT) {
		member.is_mut = true;
		++it;
	}

	auto type = *(it++);
	if ((*it)->type == TreeType::templates) {
		// TODO
		++it;
	}

	assert((*it)->type == TreeType::statement);
	it = (*it)->children.begin();
	if ((*it)->type == TreeType::setting_clause) {
		it = (*it)->children.begin();
	}
	assert((*it)->type == TreeType::P_IDENTIFIER);
	name = (*it)->token.str;

	return std::pair<std::string, TypeInst>(name, member);
}

const ScopeNode* ScopeNode::get_variable_type(std::string identifier) const {
	if (children.count(identifier) > 0) {
		ScopeNode* r = children.at(identifier);
		// TODO: privacy
		return r;
	}
	if (this->parent != nullptr) {
		return this->parent->get_variable_type(identifier);
	}
	else {
		return nullptr;
	}
}

bool ScopeNode::is_type_name(std::string identifier) const {
	return get_variable_type(identifier) != nullptr;
}

// the return value is used to solve for nested list_literals
// e.g. [ [ [ int ] ] ] vs [ [ [ 0 ] ] ]
bool process_list_literals(ScopeNode* sn, ParseNode* pn) {

	// base case
	if (pn->type == TreeType::list_literal) {
		ParseNode* A = pn->children.front();
		bool convert = false; // should I convert list_literal to list_type
		if (A->type == TreeType::list_literal || A->type == TreeType::unordered_map_literal) {
			convert = process_list_literals(sn, A);
			
		}
		else if (is_primitive(A->type)) {
			convert = true;
		}
		else if (A->type == TreeType::P_IDENTIFIER) {
			convert = sn->is_type_name(A->token.str);
		}
		if (convert) {
				pn->type = TreeType::list_type;
				std::cout << "converted a list_literal to a list_type" << std::endl;
		}
		return convert;
	}
	if (pn->type == TreeType::unordered_map_literal) {
		ParseNode* A = pn->children.front(); // colon_type_clause
		ParseNode* first = A->children.front();
		ParseNode* last = A->children.back();

		bool first_is_type = false;
		if (first->type == TreeType::list_literal || first->type == TreeType::unordered_map_literal) {
			first_is_type = process_list_literals(sn, first);
		}
		else if (is_primitive(first->type)) {
			first_is_type = true;
		}
		else if (first->type == TreeType::P_IDENTIFIER) {
			first_is_type = sn->is_type_name(A->token.str);
		}

		bool last_is_type = false;
		if (last->type == TreeType::list_literal || last->type == TreeType::unordered_map_literal) {
			last_is_type = process_list_literals(sn, last);
		}
		else if (is_primitive(last->type)) {
			last_is_type = true;
		}
		else if (last->type == TreeType::P_IDENTIFIER) {
			last_is_type = sn->is_type_name(A->token.str);
		}

		if (first_is_type != last_is_type) {
			user_error("You have an unordered map that maps between types and values");
		}

		if (first_is_type) {
			pn->type = TreeType::list_type;
			std::cout << "converted a unordered_map_literal to a unordered_map_type" << std::endl;
		}
		return first_is_type;
	}

	for (auto it = pn->children.begin(); it != pn->children.end(); ++it) {
		ParseNode* node = *it;
		if (node->type == TreeType::class_implementation
				|| node->type == TreeType::function_implementation
				|| node->type == TreeType::namespace_implementation
				|| node->type == TreeType::enum_implementation) {
			// these are covered in the loop below
			continue;
		}
		else {
			process_list_literals(sn, node);
		}
	}

	if (sn->parseNode == pn) {
		for (auto it = sn->children.begin(); it != sn->children.end(); ++it) {
			ScopeNode* n = it->second;
			process_list_literals(n, n->parseNode);
		}
	}

	return false; // this value doesn't matter
}

void verify_types(ScopeNode* sn, ParseNode* pn) {
	// TODO
}

// void process_list_literals(ClassScopeNode* sn) {
// 	++ScopeNode::recursion_count;
// 	std::cout << "<< process_list_literals (Class)" << std::endl;

// 	// TODO

// 	std::cout << "<< process_list_literals (Class)" << std::endl;
// 	--ScopeNode::recursion_count;
// }
// void process_list_literals(FunctionScopeNode* sn) {
// 	++ScopeNode::recursion_count;
// 	std::cout << "<< process_list_literals (Function)" << std::endl;

// 	auto it = sn->parseNode->children.begin();
// 	ParseNode* head = *it;
// 	++it;
// 	ParseNode* return_type = *it;
// 	++it;
// 	ParseNode* block = *it;

// 	process_list_literals(block, sn);

// 	std::cout << "<< process_list_literals (Function)" << std::endl;
// 	--ScopeNode::recursion_count;
// }
// void process_list_literals(VariableScopeNode* sn) {
// 	++ScopeNode::recursion_count;
// 	std::cout << "<< process_list_literals (Variable)" << std::endl;

// 	// TODO

// 	std::cout << "<< process_list_literals (Variable)" << std::endl;
// 	--ScopeNode::recursion_count;
// }
// void process_list_literals(NamespaceScopeNode* sn) {
// 	++ScopeNode::recursion_count;
// 	std::cout << "<< process_list_literals (Namespace)" << std::endl;

// 	for (auto it = sn->members.begin(); it != sn->members.end(); ++it) {
// 		switch (it->second->type()) {
// 			case ScopeType::_class:
// 				process_list_literals((ClassScopeNode*)(it->second));
// 			break;
// 			case ScopeType::_namespace:
// 				process_list_literals((NamespaceScopeNode*)(it->second));
// 			break;
// 			case ScopeType::_function:
// 				process_list_literals((FunctionScopeNode*)(it->second));
// 			break;
// 			case ScopeType::_variable:
// 				process_list_literals((VariableScopeNode*)(it->second));
// 			break;
// 			default: break;
// 		}
// 	}

// 	std::cout << "<< process_list_literals (Namespace)" << std::endl;
// 	--ScopeNode::recursion_count;
// }

// void process_list_literals(ParseNode* n, ScopeNode* sn) {

// 	++ScopeNode::recursion_count;
// 	std::cout << ">> process_list_literals (" << treeTypeToString(n->type) << ")" << std::endl;

	// for (auto it = n->children.begin(); it != n->children.end(); ++it) {
	// 	if ((*it)->type == TreeType::list_literal) {
	// 		std::cout << "     found a [...]" << std::endl;
	// 		TreeType type = (*it)->children.front()->type;
	// 		if (type == P_KEYWORD_INT || type == P_KEYWORD_INT8 || type == P_KEYWORD_INT16 || type == P_KEYWORD_INT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_UINT8 || type == P_KEYWORD_UINT16 || type == P_KEYWORD_UINT32 || type == P_KEYWORD_CHAR || type == P_KEYWORD_BOOL || type == P_KEYWORD_FLOAT || type == P_KEYWORD_DOUBLE || type == P_KEYWORD_VAR) {
	// 			(*it)->type = TreeType::list_type;
	// 			std::cout << "     changed to list_type" << std::endl;
	// 		}
	// 		else if (type == P_IDENTIFIER) {
	// 			std::string identifier = (*it)->children.front()->token.str;
	// 			ScopeNode* a = sn->search_scope(identifier);
	// 			if (a == nullptr) {
	// 				std::cout << "probably an error" << std::endl;
	// 				exit(1);
	// 			}
	// 			if (a->type() == ScopeType::_class) {
	// 				(*it)->type = TreeType::list_type;
	// 				std::cout << "     [" << identifier << "] is a type" << std::endl;
	// 				std::cout << "     changed to list_type" << std::endl;
	// 			}
	// 			else {
	// 				std::cout << "     [" << identifier << "] is a literal" << std::endl;
	// 			}
	// 		}
	// 		else {
	// 			// TODO
	// 			std::cout << "error 1045815" << std::endl;
	// 			exit(0);
	// 		}
	// 	}
	// 	else if ((*it)->type == TreeType::unordered_map_literal) {
	// 		std::cout << "     found a {...}" << std::endl;
	// 		TreeType type = (*it)->children.front()->children.front()->type;
	// 		if (type == P_KEYWORD_INT || type == P_KEYWORD_INT8 || type == P_KEYWORD_INT16 || type == P_KEYWORD_INT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_UINT8 || type == P_KEYWORD_UINT16 || type == P_KEYWORD_UINT32 || type == P_KEYWORD_CHAR || type == P_KEYWORD_BOOL || type == P_KEYWORD_FLOAT || type == P_KEYWORD_DOUBLE || type == P_KEYWORD_VAR) {
	// 			std::cout << "     changed to unordered_map_type" << std::endl;
	// 			(*it)->type = TreeType::unordered_map_type;
	// 		}
	// 		else if (type == P_IDENTIFIER) {
	// 			std::string identifier = (*it)->children.front()->children.front()->token.str;
	// 			ScopeNode* a = sn->search_scope(identifier);
	// 			if (a == nullptr) {
	// 				std::cout << "probably an error (2)" << std::endl;
	// 				exit(1);
	// 			}
	// 			std::cout << a->type() << std::endl;
	// 			if (a->type() == ScopeType::_class) {
	// 				(*it)->type = TreeType::unordered_map_type;
	// 				std::cout << "     [" << identifier << "] is a type" << std::endl;
	// 				std::cout << "     changed to unordered_map_type" << std::endl;
	// 			}
	// 			else {
	// 				std::cout << "     [" << identifier << "] is a literal" << std::endl;
	// 			}
	// 		}
	// 		else {
	// 			// TODO
	// 			std::cout << "error 555516" << std::endl;
	// 			exit(0);
	// 		}
	// 	}
	// 	process_list_literals(*it, sn);
	// }

// 	std::cout << "<< process_list_literals" << std::endl;
// 	--ScopeNode::recursion_count;
// }

// void process_expressions(ParseNode* n, ScopeNode* sn) {
// 	++ScopeNode::recursion_count;
// 	std::cout << ">> process_expressions" << std::endl;

// 	for (auto it = n->children.begin(); it != n->children.end(); ++it) {
// 		TreeType type = (*it)->type;
// 		if (type == TreeType::variable_dec) {
			
// 		}
// 		else if (type == TreeType::statement) {
			
// 		}
// 	}

// 	std::cout << "<< process_expressions" << std::endl;
// 	--ScopeNode::recursion_count;
// }

// void ClassScopeNode::add_member(ParseNode* node) {
// 	ScopeNode* n;
// 	if (node->type == TreeType::variable_dec) {
// 		n = new VariableScopeNode(node, this, templates);
// 	}
// 	else if (node->type == TreeType::class_implementation) {
// 		ClassScopeNode* _n = new ClassScopeNode(node, this, templates);
// 		for (auto it = _n->statics.begin(); it != _n->statics.end(); ++it) {
// 			std::string name = _n->name + "." + it->first;
// 			statics[name] = it->second;
// 		}
// 		n = _n;
// 	}
// 	else if (node->type == TreeType::function_implementation) {
// 		n = new FunctionScopeNode(node, this, templates);
// 	}
// 	members[n->name] = n;
// }

// void ScopeNode::add_templates(ParseNode* node) {

// 	if (template_owner) {
// 		delete templates; // should probably never run
// 	}

// 	template_owner = true;
// 	std::vector<TemplateDec>* old = templates;
// 	templates = new std::vector<TemplateDec>(*old);

// 	auto i = node->children.begin();
// 	auto end = node->children.end();
// 	while (i != end) {
// 		TemplateDec x;
// 		if (is_int((*i)->token.type)) {
// 			x.integer_size = 1; // TODO: find the actual integer size
// 			++i;
// 		}
// 		else if ((*i)->type == TreeType::P_IDENTIFIER) {
// 			x.integer_size = 0;
// 		}
// 		else {
// 			std::cout << "You can only template by int-values and classes; you created one with " << treeTypeToString((*i)->type) << std::endl;
// 			exit(0);
// 		}

// 		x.name = (*(i))->token.str.c_str();

// 		// this technically makes this code O(n^2), but if you're
// 		// templating with O(n) parameters ... then you are truly evil
// 		for (size_t j = 0; j < templates->size(); ++j) {
// 			if ((*templates)[j].name == x.name) {
// 				error("Error: two template parameters are named the same thing!");
// 			}
// 		}
// 		templates->push_back(x);

// 		++i;
// 	}
// }

// TODO: make 'templates' a pointer to a std::vector
// this should hopefully save quite a bit of space
// ScopeNode* create_scope_tree(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) {
	// ScopeNode* rtn = nullptr;
	// if (root->type == variable_dec) {
	// 	VariableScopeNode* r = new VariableScopeNode(root, parent, templates);
	// 	// r->type = ScopeType::_;
	// 	// auto it = root->children.begin();
	// 	// variable_type = *(it++);
	// 	// if ((*it)->type == TreeType::templates) {
	// 	// 	add_templates(*(it++));
	// 	// }
	// 	// name = (*it)->children.front()->last_token.str;
	// 	rtn = r;
	// }
	// else if (root->type == static_member) {
	// 	VariableScopeNode* r = new VariableScopeNode(root, parent, templates);
	// 	// ScopeNode n(*(++root->children.begin()), this, templates);
	// 	// if (statics.count(n.name) > 0) {
	// 	// 	error("Error: tried to create two static members with the name \"" + n.name + "\"");
	// 	// }
	// 	// statics[n.name] = n;
	// 	rtn = r;
		
	// }
	// else if (root->type == function_implementation) {
	// 	ScopeNode* r = new FunctionScopeNode(root, parent, templates);
	// 	// TODO
	// 	rtn = r;
	// }
	// else if (root->type == class_implementation) {
	// 	ClassScopeNode* r = new ClassScopeNode(root, parent, templates);
	// 	// type = ScopeType::_class;
	// 	// name = (*(++(root->children.begin())))->last_token.str;
	// 	// std::cout << name << std::endl;

	// 	// auto it = ++++(root->children.begin());
	// 	// if ((*it)->type == TreeType::templates) {
	// 	// 	add_templates(*it);
	// 	// 	++it;
	// 	// }
	// 	// if ((*it)->type != TreeType::block_of_statements_or_class) {
	// 	// 	error("Error parsing class " + name);
	// 	// }

	// 	// // now run through the contents of the class and find its members
	// 	// std::map<std::string, ScopeNode*> static_vars;
	// 	// auto end = (*it)->children.end();
	// 	// it = (*it)->children.begin();
	// 	// while (it != end) {
	// 	// 	add_member(*it);
	// 	// 	++it;
	// 	// }
	// 	rtn = r;
	// }
	// else if (root->type == enum_implementation) {
	// 	EnumScopeNode* r = new EnumScopeNode(root, parent, templates);
	// 	// type = ScopeType::_enum;
	// 	// auto it = ++(root->children.begin());
	// 	// name = (*it)->last_token.str;
	// 	// std::cout << name << std::endl;
	// 	// ++it; // '*it' should be of type 'TreeType::enum_block'
	// 	// it = (*it)->children.begin();
	// 	// while ((*it)->type == TreeType::comma_clause) {

	// 	// }
	// 	rtn = r;
	// }
	// else if (root->type == namespace_implementation) {
	// 	NamespaceScopeNode* r = new NamespaceScopeNode(root, parent, templates);
	// 	rtn = r;
	// }
	// else {
	// 	std::cout << "Error: found a node of type \"" << treeTypeToString(root->type) << "\" while verifying scope" << std::endl;
	// 	exit(0);
	// }
	// return rtn;
// 	return nullptr;
// }

// ScopeNode* ScopeNode::search_scope(std::string name) {

// 	// std::cout << "<< search scope (" << name << ")" << std::endl;

// 	std::map<std::string, ScopeNode*>* members = nullptr;
// 	switch (type()) {
// 		case ScopeType::_class      : members = &((ClassScopeNode*)this)->members; break;
// 		case ScopeType::_namespace  : members = &((NamespaceScopeNode*)this)->members; break;
// 		default                     : members = nullptr; break;
// 	}

// 	if (members != nullptr && members->count(name) > 0) {
// 		return (*members)[name];
// 	}

// 	if (parent == nullptr) {
// 		return nullptr;
// 	}
// 	return parent->search_scope(name);
// }

void user_error(std::string message) {
	std::cout << message << std::endl;
	exit(0);
}

// VariableScopeNode::VariableScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, templates) {

// 	++ScopeNode::recursion_count;
// 	std::cout << ">> VariableScopeNode" << std::endl;

// 	auto it = root->children.begin();

// 	ParseNode* type = *it;
// 	++it;
// 	ParseNode* temp = *it;

// 	if (temp->type != TreeType::templates) {
// 		temp = nullptr;
// 	}
// 	else {
// 		++it;
// 	}

// 	assert((*it)->type == TreeType::statement);

// 	name = (*it)->children.front()->token.str;

// 	std::cout << "<< VariableScopeNode (" << name << ")" << std::endl;
// 	--ScopeNode::recursion_count;

// 	// TODO: do stuff with 'type' and 'temp'
// }

// ClassScopeNode::ClassScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, templates) {
// 	++ScopeNode::recursion_count;
// 	std::cout << ">> ClassScopeNode" << std::endl;

// 	auto it = ++root->children.begin();
// 	name = (*it)->token.str;
// 	++it;
// 	ParseNode* temp = nullptr;
// 	if ((*it)->type == TreeType::templates) {
// 		temp = *it;
// 	}

// 	// TODO: do stuff with 'temp'
// 	// TODO: parse members

// 	std::cout << "<< ClassScopeNode (" << name << ")" << std::endl;
// 	--ScopeNode::recursion_count;
// }

// EnumScopeNode::EnumScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, templates) {

// 	++ScopeNode::recursion_count;
// 	std::cout << ">> EnumScopeNode" << std::endl;
// 	// TODO
// 	std::cout << "<< EnumScopeNode" << std::endl;
// 	--ScopeNode::recursion_count;
// }

// FunctionScopeNode::FunctionScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, templates) {
// 	++ScopeNode::recursion_count;
// 	std::cout << ">> FunctionScopeNode" << std::endl;

// 	ParseNode* head = *(root->children.begin());
// 	ParseNode* return_type = *(++root->children.begin());
// 	ParseNode* block = *(++++root->children.begin());
// 	name = head->children.front()->token.str;

// 	// TODO

// 	std::cout << "<< FunctionScopeNode (" << name << ")" << std::endl;
// 	--ScopeNode::recursion_count;
// }

// NamespaceScopeNode::NamespaceScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, templates) {
// 	++ScopeNode::recursion_count;
// 	std::cout << ">> NamespaceScopeNode" << std::endl;


// 	for (auto it = root->children.begin(); it != root->children.end(); ++it) {
// 		ParseNode* node = *it;
// 		ScopeNode* n;
// 		if (node->type == TreeType::variable_dec) {
// 			n = new VariableScopeNode(node, this, templates);
// 		}
// 		else if (node->type == TreeType::class_implementation) {
// 			ClassScopeNode* _n = new ClassScopeNode(node, this, templates);
// 			for (auto it = _n->statics.begin(); it != _n->statics.end(); ++it) {
// 				std::string name = _n->name + "." + it->first;
// 				statics[name] = it->second;
// 			}
// 			n = _n;
// 		}
// 		else if (node->type == TreeType::function_implementation) {
// 			n = new FunctionScopeNode(node, this, templates);
// 		}
// 		members[n->name] = n;
// 	}


// 	std::cout << "<< NamespaceScopeNode" << std::endl;
// 	--ScopeNode::recursion_count;
// }