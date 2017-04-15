
#include <string>
#include <iostream>
#include <vector>

#include "../include/ScopeVerifier.hpp"

/*
left side of variable declaration
function arguments (i.e. foo(INT x))
constructors
multiple inheritance
*/

// TODO: implement "static" once the ParseRules are added

uint64_t ScopeNode::recursion_count = -1;

std::string pad(uint64_t n) {
    std::string rtn = "";
    for (uint64_t i = 0; i < n; ++i) {
        rtn += "  ";
    }
    return rtn;
}

ScopeNode* verify_scope(ParseNode* root) {
	
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> verify_scope" << std::endl;

	NamespaceScopeNode* n = new NamespaceScopeNode(root, nullptr, new std::vector<TemplateDec>());
	n->templates = new std::vector<TemplateDec>();
	// for (auto it = root->children.begin(); it != root->children.end(); ++it) {
	// 	std::cout << pad(ScopeNode::recursion_count) << "     adding member" << std::endl;
	// 	n->add_member(*it);
	// }
	// TODO: actually *do* stuff with the scope tree!

	// determine if a 'list_literal' is actually a 'list_type'
	// determine if a 'unordered_map_literal' is actually a 'unordered_map_type'
	process_list_literals(n);

	for (auto it = n->members.begin(); it != n->members.end(); ++it) {
		std::cout << "\"" << it->first << "\"  :  " << it->second->name << std::endl;
	}

	std::cout << pad(ScopeNode::recursion_count) << "<< verify_scope" << std::endl;
	--ScopeNode::recursion_count;
	return (ScopeNode*) n;
}

std::string scopeTypeToString(ScopeType type) {
    switch (type) {
        case ScopeType::_null: return "ScopeType::null";
        case ScopeType::_class: return "ScopeType::class";
        case ScopeType::_namespace: return "ScopeType::namespace";
        case ScopeType::_enum: return "ScopeType::enum";
        case ScopeType::_function: return "ScopeType::function";
        case ScopeType::_variable: return "ScopeType::variable";
        case ScopeType::_global: return "ScopeType::global";
    }
}

void process_list_literals(ClassScopeNode* sn) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Class)" << std::endl;

	// TODO

	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Class)" << std::endl;
	--ScopeNode::recursion_count;
}
void process_list_literals(FunctionScopeNode* sn) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Function)" << std::endl;

	auto it = sn->parseNode->children.begin();
	ParseNode* head = *it;
	++it;
	ParseNode* return_type = *it;
	++it;
	ParseNode* block = *it;

	process_list_literals(block, sn);

	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Function)" << std::endl;
	--ScopeNode::recursion_count;
}
void process_list_literals(VariableScopeNode* sn) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Variable)" << std::endl;

	// TODO

	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Variable)" << std::endl;
	--ScopeNode::recursion_count;
}
void process_list_literals(NamespaceScopeNode* sn) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Namespace)" << std::endl;

	for (auto it = sn->members.begin(); it != sn->members.end(); ++it) {
		switch (it->second->type) {
			case ScopeType::_class:
				process_list_literals((ClassScopeNode*)(it->second));
			break;
			case ScopeType::_namespace:
				process_list_literals((NamespaceScopeNode*)(it->second));
			break;
			case ScopeType::_function:
				process_list_literals((FunctionScopeNode*)(it->second));
			break;
			case ScopeType::_variable:
				process_list_literals((VariableScopeNode*)(it->second));
			break;
			default: break;
		}
	}

	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals (Namespace)" << std::endl;
	--ScopeNode::recursion_count;
}
void process_list_literals(ParseNode* n, ScopeNode* sn) {

	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> process_list_literals (" << treeTypeToString(n->type) << ")" << std::endl;

	for (auto it = n->children.begin(); it != n->children.end(); ++it) {
		if ((*it)->type == TreeType::list_literal) {
			std::cout << pad(ScopeNode::recursion_count) << "     found a [...]" << std::endl;
			TreeType type = (*it)->children.front()->type;
			if (type == P_KEYWORD_INT || type == P_KEYWORD_INT8 || type == P_KEYWORD_INT16 || type == P_KEYWORD_INT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_UINT8 || type == P_KEYWORD_UINT16 || type == P_KEYWORD_UINT32 || type == P_KEYWORD_CHAR || type == P_KEYWORD_BOOL || type == P_KEYWORD_FLOAT || type == P_KEYWORD_DOUBLE || type == P_KEYWORD_VAR) {
				(*it)->type = TreeType::list_type;
				std::cout << pad(ScopeNode::recursion_count) << "     changed to list_type" << std::endl;
			}
			else if (type == P_IDENTIFIER) {
				std::string identifier = (*it)->children.front()->token.str;
				ScopeNode* a = sn->search_scope(identifier);
				if (a == nullptr) {
					std::cout << "probably an error" << std::endl;
					exit(1);
				}
				if (a->type == ScopeType::_class) {
					(*it)->type = TreeType::list_type;
					std::cout << pad(ScopeNode::recursion_count) << "     [" << identifier << "] is a type" << std::endl;
					std::cout << pad(ScopeNode::recursion_count) << "     changed to list_type" << std::endl;
				}
				else {
					std::cout << pad(ScopeNode::recursion_count) << "     [" << identifier << "] is a literal" << std::endl;
				}
			}
			else {
				// TODO
				std::cout << "error 1045815" << std::endl;
				exit(0);
			}
		}
		else if ((*it)->type == TreeType::unordered_map_literal) {
			std::cout << pad(ScopeNode::recursion_count) << "     found a {...}" << std::endl;
			TreeType type = (*it)->children.front()->children.front()->type;
			if (type == P_KEYWORD_INT || type == P_KEYWORD_INT8 || type == P_KEYWORD_INT16 || type == P_KEYWORD_INT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_UINT8 || type == P_KEYWORD_UINT16 || type == P_KEYWORD_UINT32 || type == P_KEYWORD_CHAR || type == P_KEYWORD_BOOL || type == P_KEYWORD_FLOAT || type == P_KEYWORD_DOUBLE || type == P_KEYWORD_VAR) {
				std::cout << pad(ScopeNode::recursion_count) << "     changed to unordered_map_type" << std::endl;
				(*it)->type = TreeType::unordered_map_type;
			}
			else if (type == P_IDENTIFIER) {
				std::string identifier = (*it)->children.front()->children.front()->token.str;
				ScopeNode* a = sn->search_scope(identifier);
				if (a == nullptr) {
					std::cout << "probably an error (2)" << std::endl;
					exit(1);
				}
				std::cout << a->type << std::endl;
				if (a->type == ScopeType::_class) {
					(*it)->type = TreeType::unordered_map_type;
					std::cout << pad(ScopeNode::recursion_count) << "     [" << identifier << "] is a type" << std::endl;
					std::cout << pad(ScopeNode::recursion_count) << "     changed to unordered_map_type" << std::endl;
				}
				else {
					std::cout << pad(ScopeNode::recursion_count) << "     [" << identifier << "] is a literal" << std::endl;
				}
			}
			else {
				// TODO
				std::cout << "error 555516" << std::endl;
				exit(0);
			}
		}
		process_list_literals(*it, sn);
	}

	std::cout << pad(ScopeNode::recursion_count) << "<< process_list_literals" << std::endl;
	--ScopeNode::recursion_count;
}



// void process_expressions(ClassScopeNode*);
// void process_expressions(FunctionScopeNode*);
// void process_expressions(NamespaceScopeNode*);
// void process_expressions(VariableScopeNode*);
void process_expressions(ParseNode* n, ScopeNode* sn) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> process_expressions" << std::endl;

	for (auto it = n->children.begin(); it != n->children.end(); ++it) {
		TreeType type = (*it)->type;
		if (type == TreeType::variable_dec) {
			
		}
		else if (type == TreeType::statement) {
			
		}
	}

	std::cout << pad(ScopeNode::recursion_count) << "<< process_expressions" << std::endl;
	--ScopeNode::recursion_count;
}

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

void ScopeNode::add_templates(ParseNode* node) {

	if (template_owner) {
		delete templates; // should probably never run
	}

	template_owner = true;
	std::vector<TemplateDec>* old = templates;
	templates = new std::vector<TemplateDec>(*old);

	auto i = node->children.begin();
	auto end = node->children.end();
	while (i != end) {
		TemplateDec x;
		if (is_int((*i)->token.type)) {
			x.integer_size = 1; // TODO: find the actual integer size
			++i;
		}
		else if ((*i)->type == TreeType::P_IDENTIFIER) {
			x.integer_size = 0;
		}
		else {
			std::cout << "You can only template by int-values and classes; you created one with " << treeTypeToString((*i)->type) << std::endl;
			exit(0);
		}

		x.name = (*(i))->token.str.c_str();

		// this technically makes this code O(n^2), but if you're
		// templating with O(n) parameters ... then you are truly evil
		for (size_t j = 0; j < templates->size(); ++j) {
			if ((*templates)[j].name == x.name) {
				error("Error: two template parameters are named the same thing!");
			}
		}
		templates->push_back(x);

		++i;
	}
}

// TODO: make 'templates' a pointer to a std::vector
// this should hopefully save quite a bit of space
ScopeNode* create_scope_tree(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) {
	ScopeNode* rtn = nullptr;
	if (root->type == variable_dec) {
		VariableScopeNode* r = new VariableScopeNode(root, parent, templates);
		// r->type = ScopeType::_;
		// auto it = root->children.begin();
		// variable_type = *(it++);
		// if ((*it)->type == TreeType::templates) {
		// 	add_templates(*(it++));
		// }
		// name = (*it)->children.front()->last_token.str;
		rtn = r;
	}
	else if (root->type == static_member) {
		VariableScopeNode* r = new VariableScopeNode(root, parent, templates);
		// ScopeNode n(*(++root->children.begin()), this, templates);
		// if (statics.count(n.name) > 0) {
		// 	error("Error: tried to create two static members with the name \"" + n.name + "\"");
		// }
		// statics[n.name] = n;
		rtn = r;
		
	}
	else if (root->type == function_implementation) {
		ScopeNode* r = new FunctionScopeNode(root, parent, templates);
		// TODO
		rtn = r;
	}
	else if (root->type == class_implementation) {
		ClassScopeNode* r = new ClassScopeNode(root, parent, templates);
		// type = ScopeType::_class;
		// name = (*(++(root->children.begin())))->last_token.str;
		// std::cout << name << std::endl;

		// auto it = ++++(root->children.begin());
		// if ((*it)->type == TreeType::templates) {
		// 	add_templates(*it);
		// 	++it;
		// }
		// if ((*it)->type != TreeType::block_of_statements_or_class) {
		// 	error("Error parsing class " + name);
		// }

		// // now run through the contents of the class and find its members
		// std::map<std::string, ScopeNode*> static_vars;
		// auto end = (*it)->children.end();
		// it = (*it)->children.begin();
		// while (it != end) {
		// 	add_member(*it);
		// 	++it;
		// }
		rtn = r;
	}
	else if (root->type == enum_implementation) {
		EnumScopeNode* r = new EnumScopeNode(root, parent, templates);
		// type = ScopeType::_enum;
		// auto it = ++(root->children.begin());
		// name = (*it)->last_token.str;
		// std::cout << name << std::endl;
		// ++it; // '*it' should be of type 'TreeType::enum_block'
		// it = (*it)->children.begin();
		// while ((*it)->type == TreeType::comma_clause) {

		// }
		rtn = r;
	}
	else if (root->type == namespace_implementation) {
		NamespaceScopeNode* r = new NamespaceScopeNode(root, parent, templates);
		rtn = r;
	}
	else {
		std::cout << "Error: found a node of type \"" << treeTypeToString(root->type) << "\" while verifying scope" << std::endl;
		exit(0);
	}
	return rtn;
}

ScopeNode* ScopeNode::search_scope(std::string name) {

	// std::cout << "<< search scope (" << name << ")" << std::endl;

	std::map<std::string, ScopeNode*>* members = nullptr;
	switch (type) {
		case ScopeType::_class      : members = &((ClassScopeNode*)this)->members; break;
		case ScopeType::_namespace  : members = &((NamespaceScopeNode*)this)->members; break;
		default                     : members = nullptr; break;
	}

	if (members != nullptr && members->count(name) > 0) {
		return (*members)[name];
	}

	if (parent == nullptr) {
		return nullptr;
	}
	return parent->search_scope(name);
}

void ScopeNode::error(std::string message) const {
	std::cout << message << std::endl;
	exit(0);
}

VariableScopeNode::VariableScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, ScopeType::_variable, templates) {

	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> VariableScopeNode" << std::endl;

	auto it = root->children.begin();

	ParseNode* type = *it;
	++it;
	ParseNode* temp = *it;

	if (temp->type != TreeType::templates) {
		temp = nullptr;
	}
	else {
		++it;
	}

	assert((*it)->type == TreeType::statement);

	name = (*it)->children.front()->token.str;

	std::cout << pad(ScopeNode::recursion_count) << "<< VariableScopeNode (" << name << ")" << std::endl;
	--ScopeNode::recursion_count;

	// TODO: do stuff with 'type' and 'temp'
}

ClassScopeNode::ClassScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, ScopeType::_class, templates) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> ClassScopeNode" << std::endl;

	auto it = ++root->children.begin();
	name = (*it)->token.str;
	++it;
	ParseNode* temp = nullptr;
	if ((*it)->type == TreeType::templates) {
		temp = *it;
	}

	// TODO: do stuff with 'temp'
	// TODO: parse members

	std::cout << pad(ScopeNode::recursion_count) << "<< ClassScopeNode (" << name << ")" << std::endl;
	--ScopeNode::recursion_count;
}

EnumScopeNode::EnumScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, ScopeType::_enum, templates) {

	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> EnumScopeNode" << std::endl;
	// TODO
	std::cout << pad(ScopeNode::recursion_count) << "<< EnumScopeNode" << std::endl;
	--ScopeNode::recursion_count;
}

FunctionScopeNode::FunctionScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, ScopeType::_function, templates) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> FunctionScopeNode" << std::endl;

	ParseNode* head = *(root->children.begin());
	ParseNode* return_type = *(++root->children.begin());
	ParseNode* block = *(++++root->children.begin());
	name = head->children.front()->token.str;

	// TODO

	std::cout << pad(ScopeNode::recursion_count) << "<< FunctionScopeNode (" << name << ")" << std::endl;
	--ScopeNode::recursion_count;
}

NamespaceScopeNode::NamespaceScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) : ScopeNode::ScopeNode(root, parent, ScopeType::_namespace, templates) {
	++ScopeNode::recursion_count;
	std::cout << pad(ScopeNode::recursion_count) << ">> NamespaceScopeNode" << std::endl;


	for (auto it = root->children.begin(); it != root->children.end(); ++it) {
		ParseNode* node = *it;
		ScopeNode* n;
		if (node->type == TreeType::variable_dec) {
			n = new VariableScopeNode(node, this, templates);
		}
		else if (node->type == TreeType::class_implementation) {
			ClassScopeNode* _n = new ClassScopeNode(node, this, templates);
			for (auto it = _n->statics.begin(); it != _n->statics.end(); ++it) {
				std::string name = _n->name + "." + it->first;
				statics[name] = it->second;
			}
			n = _n;
		}
		else if (node->type == TreeType::function_implementation) {
			n = new FunctionScopeNode(node, this, templates);
		}
		members[n->name] = n;
	}


	std::cout << pad(ScopeNode::recursion_count) << "<< NamespaceScopeNode" << std::endl;
	--ScopeNode::recursion_count;
}