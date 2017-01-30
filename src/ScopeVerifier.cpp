
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

ScopeNode* verify_scope(ParseNode* root) {
	NamespaceScopeNode* n = new NamespaceScopeNode();
	n->templates = new std::vector<TemplateDec>();
	for (auto it = root->children.begin(); it != root->children.end(); ++it) {
		std::cout << "adding member" << std::endl;
		n->add_member(*it);
	}
	// TODO: actually *do* stuff with the scope tree!

	// determine if a 'list_literal' is actually a 'list_type'
	process_list_literals(n);

	for (auto it = n->members.begin(); it != n->members.end(); ++it) {
		std::cout << "\"" << it->first << "\"  :  " << it->second->name << std::endl;
	}

	return (ScopeNode*) n;
}

void process_list_literals(ClassScopeNode* sn) {
	std::cout << "process_list_literals; cla" << std::endl;
}
void process_list_literals(FunctionScopeNode* sn) {
	std::cout << "process_list_literals; fun" << std::endl;

	auto it = sn->parseNode->children.begin();
	ParseNode* head = *it;
	++it;
	ParseNode* return_type = *it;
	++it;
	ParseNode* block = *it;

	process_list_literals(block, sn);
}
void process_list_literals(VariableScopeNode* sn) {
	std::cout << "process_list_literals; var" << std::endl;
}
void process_list_literals(NamespaceScopeNode* sn) {
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
}
void process_list_literals(ParseNode* n, ScopeNode* sn) {
	// for (auto it = n->children.begin(); it != n->children.end(); ++it) {
	// 	if ((*it)->type == TreeType::list_literal) {
	// 		TreeType type = 
	// 		if (type == P_KEYWORD_INT || type == P_KEYWORD_INT8 || type == P_KEYWORD_INT16 || type == P_KEYWORD_INT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_UINT8 || type == P_KEYWORD_UINT16 || type == P_KEYWORD_UINT32 || type == P_KEYWORD_CHAR || type == P_KEYWORD_BOOL || type == P_KEYWORD_FLOAT || type == P_KEYWORD_DOUBLE || type == P_KEYWORD_VAR) {
	// 			(*it)->type = TreeType::list_type;
	// 		}
	// 		else if (type == P_IDENTIFIER) {
	// 			// TODO
	// 		}
	// 		else {
	// 			// TODO
	// 		}
	// 	}
	// 	process_list_literals(*it, sn);
	// }
}

void ClassScopeNode::add_member(ParseNode* node) {
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

void NamespaceScopeNode::add_member(ParseNode* node) {
	ScopeNode* n;
	if (node->type == TreeType::variable_dec) {
		std::cout << "variable dec" << std::endl;
		n = new VariableScopeNode(node, this, templates);
	}
	else if (node->type == TreeType::class_implementation) {
		ClassScopeNode* _n = new ClassScopeNode(node, this, templates);
		for (auto it = _n->statics.begin(); it != _n->statics.end(); ++it) {
			std::string name = _n->name + "." + it->first;
			statics[name] = it->second;
		}
		n = _n;
		std::cout << "class imp \"" << n->name << "\"" << std::endl;
	}
	else if (node->type == TreeType::function_implementation) {
		n = new FunctionScopeNode(node, this, templates);
		std::cout << "function imp \"" << n->name << "\"" << std::endl;
	}
	members[n->name] = n;
}

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
		if (is_int((*i)->last_token->type)) {
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

		x.name = (*(i))->last_token->str.c_str();

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
// ScopeNode* create_scope_tree(ParseNode* root, ScopeNode* parent, std::vector<ScopeNode> templates) {
// 	ScopeNode* rtn = nullptr;
// 	if (root->type == variable_dec) {
// 		VariableScopeNode* r = new VariableScopeNode();
// 		// r->type = ScopeType::_;
// 		// auto it = root->children.begin();
// 		// variable_type = *(it++);
// 		// if ((*it)->type == TreeType::templates) {
// 		// 	add_templates(*(it++));
// 		// }
// 		// name = (*it)->children.front()->last_token->str;
// 		rtn = r;
// 	}
// 	else if (root->type == static_member) {
// 		VariableScopeNode* r = new VariableScopeNode();
// 		// ScopeNode n(*(++root->children.begin()), this, templates);
// 		// if (statics.count(n.name) > 0) {
// 		// 	error("Error: tried to create two static members with the name \"" + n.name + "\"");
// 		// }
// 		// statics[n.name] = n;
// 		rtn = r;
		
// 	}
// 	else if (root->type == function_implementation) {
// 		ScopeNode* r = new ScopeNode();
// 		// TODO
// 		rtn = r;
// 	}
// 	else if (root->type == class_implementation) {
// 		ClassScopeNode* r = new ClassScopeNode();
// 		// type = ScopeType::_class;
// 		// name = (*(++(root->children.begin())))->last_token->str;
// 		// std::cout << name << std::endl;

// 		// auto it = ++++(root->children.begin());
// 		// if ((*it)->type == TreeType::templates) {
// 		// 	add_templates(*it);
// 		// 	++it;
// 		// }
// 		// if ((*it)->type != TreeType::block_of_statements_or_class) {
// 		// 	error("Error parsing class " + name);
// 		// }

// 		// // now run through the contents of the class and find its members
// 		// std::map<std::string, ScopeNode*> static_vars;
// 		// auto end = (*it)->children.end();
// 		// it = (*it)->children.begin();
// 		// while (it != end) {
// 		// 	add_member(*it);
// 		// 	++it;
// 		// }
// 		rtn = r;
// 	}
// 	else if (root->type == enum_implementation) {
// 		EnumScopeNode* r = new EnumScopeNode();
// 		// type = ScopeType::_enum;
// 		// auto it = ++(root->children.begin());
// 		// name = (*it)->last_token->str;
// 		// std::cout << name << std::endl;
// 		// ++it; // '*it' should be of type 'TreeType::enum_block'
// 		// it = (*it)->children.begin();
// 		// while ((*it)->type == TreeType::comma_clause) {

// 		// }
// 		rtn = r;
// 	}
// 	else if (root->type == namespace_implementation) {
// 		NamespaceScopeNode* r = new NamespaceScopeNode();
// 		rtn = r;
// 	}
// 	else {
// 		std::cout << "Error: found a node of type \"" << treeTypeToString(root->type) << "\" while verifying scope" << std::endl;
// 		exit(0);
// 	}
// }

ScopeNode* ScopeNode::search_scope(std::string name) {

	std::map<std::string, ScopeNode*>* members;
	switch (type) {
		case _class      : members = &((ClassScopeNode*)this)->members;
		case _namespace  : members = &((NamespaceScopeNode*)this)->members;
		default          : members = nullptr;
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

VariableScopeNode::VariableScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) {
	type = ScopeType::_variable;
	parseNode = root;
}

ClassScopeNode::ClassScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) {
	type = ScopeType::_class;
	parseNode = root;
	auto it = ++root->children.begin();
	name = (*it)->last_token->str;
}

EnumScopeNode::EnumScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) {
	type = ScopeType::_enum;
	parseNode = root;
}

FunctionScopeNode::FunctionScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) {
	type = ScopeType::_function;
	parseNode = root;
	auto head = root->children.begin();
	auto return_type = ++root->children.begin();
	auto block = ++++root->children.begin();
	name = (*((*head)->children.begin()))->last_token->str;

}

NamespaceScopeNode::NamespaceScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates) {
	type = ScopeType::_namespace;
	parseNode = root;
}