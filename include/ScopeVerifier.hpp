#ifndef SCOPE_VERIFIER_HPP
#define SCOPE_VERIFIER_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "Parser.hpp"

using namespace std;

enum Privacy {
    _public,
    _private,
    _protected
};

enum ScopeType {
    _global,    
    _function,
    _class,
    _enum,
};

string scopeTypeToString(ScopeType type);

struct TemplateDec {
    uint8_t integer_size; // use 'zero' for classes
    const char* name;
    bool is_class() const { return integer_size == 0; }
    bool is_int() const { return integer_size > 0; }
};

// function defined for convenience
string pad(uint64_t n);

// used to throw errors
void user_error(string message);

// doesn't include list_literal or unordered_map_literal
bool is_primitive(TreeType type);

struct ScopeNode;

// represents instances (including enum-values like "Direction::UP")
struct TypeInst {
    TypeInst() : myClass(nullptr), is_static(false), is_mut(false) {};

    // returns member's name and the member itself
    static std::pair<std::string, TypeInst> get_variable(ScopeNode* parent, ParseNode* node);

    // the type of the member
    ScopeNode* myClass;
    bool is_static, is_mut;

    // TODO: template instantiation
};


// abstract class inherited from by EnumValue, VariableScopeNode, ClassScopeNode, etc.
// represents a node in the stack-tree of the program
// i.e. most things that have curly braces (aside from loops, if-statements, etc) have a ScopeNode
struct ScopeNode {

    // ScopeNode(ParseNode* root, ScopeNode* parent, vector<TemplateDec>* templates);
    ScopeNode() : parseNode(nullptr), parent(nullptr), templates(nullptr), template_owner(false) {};
    ScopeNode(ScopeType type, ParseNode* parseNode, ScopeNode* parent, vector<TemplateDec>* templates);
    ~ScopeNode() { if (template_owner) delete templates; }

    // these are the "real" constructors for 'ScopeNode'
    void construct_global_node();
    void construct_function_node();
    void construct_class_node();
    void construct_enum_node();

    ScopeNode* search_scope(string);

    // search scope for a class name
    const ScopeNode* get_variable_type(string) const;
    bool is_type_name(string) const;

    void add_templates(ParseNode* node);

    ScopeType type;
    ParseNode* parseNode; // corresponding node in the parse tree
    ScopeNode* parent; // parent in the tree
    string name;       // name of class/function/whatever
    
    vector<TemplateDec>* templates; // name of templates currently applying to the scope
    bool template_owner;            // used to garbage-collect 'templates'

    unordered_map     < string, ScopeNode* > children;  // children (classes, functions, and enums)
    unordered_map     < string, TypeInst   > members;   // member variables
    set               < string, ScopeNode* > friends;   // only used by classes

    // used by functions
    // (hackily) used by enums to store string
    vector    < pair< string, ScopeNode* > > arguments;
};

// returns a tree of different scopes
// terminates the program with a message if it finds an error in the user's code
ScopeNode* build_scope_tree(ParseNode* root);

// run through program and ensures all types "work"
// a.x = y; // make sure there is an "=(type(x), type(y))" function that exists
// foo(c);  // make sure there is an "foo(type(c))" function that exists
void verify_types(ScopeNode*, ParseNode*);


// runs through program and converts list_literal and unordered_map_literal
// to types (where appropriate)
// ignore the return-value (it's used internally)
bool process_list_literals(ScopeNode*, ParseNode*);

#endif