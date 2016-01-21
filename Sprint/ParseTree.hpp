//
//  ParseTree.hpp
//  Sprint
//
//  Created by Morgan Redding on 12/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef ParseTree_h
#define ParseTree_h

#include "Token.hpp"
#include <vector>

struct ParseTree {
    ParseTree(const std::pair<const Token*, size_t>& value) : value(value), parent(nullptr), _number_of_leaves(0), children(_children) {};
    ParseTree(ParseTree* parent) : parent(parent), _number_of_leaves(0), children(_children) {};
    ParseTree() : children(_children) {
        delete value.first;
        for (size_t i = 0; i < _children.size(); ++i) {
            delete _children[i];
        }
    }
    void add_child(ParseTree* child) {
        child->parent = this;
        _children.push_back(child);
        _number_of_leaves++;
        if (_children.size() > 1) {
            ParseTree* p = parent;
            while (p != nullptr) {
                p->_number_of_leaves++;
                p = p->parent;
            }
        }
    }
    void add_child(const std::pair<const Token*, size_t>& value) {
        add_child(new ParseTree(value));
    }
    size_t number_of_children() const {
        return _children.size();
    }
    size_t number_of_leaves() const {
        return _number_of_leaves;
    }
    ParseTree* operator[](size_t i) {
        return _children[i];
    }
    std::pair<const Token*, size_t> value;
    
    friend std::ostream& operator<<(std::ostream& o, const ParseTree& t) {
        return o << t.toString();
    }
    
    std::string toString(int depth = 0) const {
        
        std::string rtn = "";
        for (int i = 0; i < depth; ++i) {
            rtn += "    ";
        }
        rtn += valueToString();
        if (children.size() == 0) {
            return rtn;
        }
        for (int i = 0; i < children.size(); ++i) {
            rtn += '\n';
            rtn += children[i]->toString(depth + 1);
        }
        return rtn;
    }
    
    std::string valueToString() const {
        return "  " + token_to_string(value.first->type) + "  ";
    }
    
    const std::vector<ParseTree*>& children;
    
private:
    
    int width() const {
        int x = int(valueToString().size());
        int y = 0;
        for (int i = 0; i < children.size(); ++i) {
            y += children[i]->width();
        }
        return x > y ? x : y;
    }
    
    std::string token_to_string(const TokenType& t) const {
        std::string arr[141] = { "etc","etc_not","INDENT","DEDENT","NEWLINE","KEYWORD","IDENTIFIER","INTEGER_LITERAL","FLOAT_LITERAL","CHARACTER_LITERAL","STRING_LITERAL","PUNCTUATION","BRACKET","UNKNOWN","NULL_TOKEN_TYPE","OPEN_PARENTHESIS","CLOSE_PARENTHESIS","OPEN_BRACKET","CLOSE_BRACKET","OPEN_CURLY_BRACE","CLOSE_CURLY_BRACE","KEYWORD_ABSTRACT","KEYWORD_BREAK","KEYWORD_CASE","KEYWORD_CATCH","KEYWORD_CLASS","KEYWORD_CONST","KEYWORD_CONTINUE","KEYWORD_DELETE","KEYWORD_DO","KEYWORD_ELSE","KEYWORD_ENUM","KEYWORD_FALSE","KEYWORD_FOR","KEYWORD_IF","KEYWORD_IN","KEYWORD_INLINE","KEYWORD_NEW","KEYWORD_NULL","KEYWORD_PROTECTED","KEYWORD_PRIVATE","KEYWORD_PTR","KEYWORD_REF","KEYWORD_RETURN","KEYWORD_SIZEOF","KEYWORD_STATIC","KEYWORD_STRUCT","KEYWORD_THIS","KEYWORD_THROW","KEYWORD_TRUE","KEYWORD_TRY","KEYWORD_VIRTUAL","KEYWORD_WHILE","KEYWORD_INT","KEYWORD_INT8","KEYWORD_INT16","KEYWORD_INT32","KEYWORD_INT64","KEYWORD_UINT","KEYWORD_UINT8","KEYWORD_UINT16","KEYWORD_UINT32","KEYWORD_UINT64","KEYWORD_AND","KEYWORD_OR","KEYWORD_NOT","KEYWORD_XOR","KEYWORD_FLOAT","KEYWORD_DOUBLE","KEYWORD_PUBLIC","PERIOD","SEMI_COLON","PLUS","MINUS","ASTERISK","SLASH","AMPERSAND","POUND_SIGN","LESS_THAN","EQUALS","GREATER_THAN","COMMA","VERTICAL_BAR","PERCENT","EXCLAMATION_POINT","AT","CARROT", "COLON","general","in_class","klass","method_declaration","method_declaration_and_implementation","member_variable_declaration","t_arg_d","method_args","block","type_name","while_loop","for_in_loop","if_statement","literal","if_else_statement","block_components","assignment_expression","assignment_set","or_expression","and_expression","xor_expression","equality_expression","equality_set","relational_expression","relational_set","shift_expression","shift_set","plus_expression","plus_set","times_expression","times_set","postfix_expression","pointer_value","access_value","pointer_access","function_call","function_arg","t_args","ta","t_arg","line","program","expression","template_parameter","template_parameter_child","function_parameter","type","unary_expression","access_modifier_set", "postfix_expression_two", "array", "dictionary", "dictionary_arg"};
        return arr[int(t)];
    }
    
    size_t _number_of_leaves;
    std::vector<ParseTree*> _children;
    ParseTree* parent;
};


#endif /* ParseTree_h */
