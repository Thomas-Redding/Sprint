#ifndef POST_TOKENIZER_HPP
#define POST_TOKENIZER_HPP

#include "Token.hpp"

// converts 'LESS_THAN' to 'OPEN_TEMPLATE'
// converts 'ASTERISK' to 'PTR'
// converts 'GREATER_THAN' to 'CLOSE_TEMPLATE'
// converts function identifiers (where they're declared/implemented) to 'FUNC_DECL_IDENTIFIER'
// converts all 'IDENTIFIER's to 'CLASS_IDENTIFIER' if they reference a class
void postTokenize(std::list<Token>& list);

#endif