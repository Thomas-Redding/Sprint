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

std::string scopeTypeToString(ScopeType type);

struct TemplateDec {
    uint8_t integer_size; // use 'zero' for classes
    const char* name;
    bool is_class() const { return integer_size == 0; }
    bool is_int() const { return integer_size > 0; }
};

std::string pad(uint64_t n);

struct ScopeNode {

    static uint64_t recursion_count;

    // ScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);
    ScopeNode() : parseNode(nullptr), parent(nullptr), type(ScopeType::_null), templates(nullptr), template_owner(false) {};
    ScopeNode(ParseNode* parseNode, ScopeNode* parent, ScopeType type, std::vector<TemplateDec>* templates) : parseNode(parseNode), parent(parent), type(type), templates(templates) {};
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
    NamespaceScopeNode(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);

    std::map<std::string, ScopeNode*> members;
    std::map<std::string, ScopeNode*> statics;
};

ScopeNode* verify_scope(ParseNode* root);
ScopeNode* create_scope_tree(ParseNode* root, ScopeNode* parent, std::vector<TemplateDec>* templates);

// determines wither [...] is a type (e.g. "[int]") or a literal (e.g. "[0, 1, 2, 3]")
void process_list_literals(ClassScopeNode*);
void process_list_literals(FunctionScopeNode*);
void process_list_literals(NamespaceScopeNode*);
void process_list_literals(VariableScopeNode*);
void process_list_literals(ParseNode*, ScopeNode*); // uses the ScopeNode* argument to search for classes

void process_expressions(ClassScopeNode*);
void process_expressions(FunctionScopeNode*);
void process_expressions(NamespaceScopeNode*);
void process_expressions(VariableScopeNode*);
void process_expressions(ParseNode*, ScopeNode*);



