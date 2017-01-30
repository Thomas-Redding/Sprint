#pragma once

#include <vector>
#include <string>
#include <map>
#include <map>

#include "Parser.hpp"

enum ScopeType {
    _null,
    _class,
    _namespace,
    _enum,
    _function,
    _variable,
    _global,
};

struct TemplateDec {
    uint8_t integer_size; // use 'zero' for classes
    const char* name;
    bool is_class() const { return integer_size == 0; }
    bool is_int() const { return integer_size > 0; }
};

struct ScopeNode {

    // ScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);
    ScopeNode() : parseNode(nullptr), parent(nullptr), type(ScopeType::_null), templates(nullptr), template_owner(false) {};
    ~ScopeNode() { if (template_owner) delete templates; }

    ScopeNode* search_scope(std::string);


    void add_templates(ParseNode* node);

    void error(std::string message) const;

    ParseNode* parseNode;
    ScopeNode* parent;
    ScopeType type;
    std::string name;

    std::vector<TemplateDec>* templates;
    bool template_owner; // used to garbage-collect 'templates'
};

/*
    classes, enums, and namespaces have "members"
*/

struct EnumValue : ScopeNode {
    EnumValue(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);
    uint64_t value;
};

struct VariableScopeNode : ScopeNode {
    VariableScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);
    bool mut;
    ParseNode* variable_type;
};

struct ClassScopeNode : ScopeNode {
    ClassScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);

    void add_member(ParseNode* node);

    std::map<std::string, ScopeNode*> members;
    std::map<std::string, ScopeNode*> statics;
};

struct FunctionScopeNode : ScopeNode {
    FunctionScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);
    std::map<std::string, ParseNode*> arguments; // maps an argument's name to the ParseNode of it's type
};

struct EnumScopeNode : ScopeNode {
    EnumScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);
    std::map<std::string, ScopeNode*> statics;
};

struct NamespaceScopeNode : ScopeNode {
    NamespaceScopeNode() {};
    NamespaceScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);

    void add_member(ParseNode* node);

    std::map<std::string, ScopeNode*> members;
    std::map<std::string, ScopeNode*> statics;
};

ScopeNode* verify_scope(ParseNode* root);
ScopeNode* create_scope_tree(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);

void process_list_literals(ClassScopeNode*);
void process_list_literals(FunctionScopeNode*);
void process_list_literals(NamespaceScopeNode*);
void process_list_literals(VariableScopeNode*);

// determines wither [...] is a type (e.g. "[int]") or a literal (e.g. "[0, 1, 2, 3]")
// uses the ScopeNode* argument to search for classes
void process_list_literals(ParseNode*, ScopeNode*);




