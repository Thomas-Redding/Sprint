#include "../include/Sweetener.hpp"

template<class T>
void move(std::vector<T>& A, size_t from, size_t length, size_t to) {
    T* store = new T[length];
    for (size_t i = 0; i < length; ++i) {
        store[i] = A[from + i];
    }
    for (size_t i = from; i < to; ++i) {
        A[i] = A[i + length];
    }
    for (size_t i = 0; i < length; ++i) {
        A[to + i] = store[i];
    }
    delete [] store;
}

bool addFunctionSugar(std::vector<Token>& tokens) {
    for (size_t i = 1; i < tokens.size() - 2; ++i) {
        if (tokens[i].type != PERIOD || tokens[i + 1].type != IDENTIFIER) {
            continue;
        }
        
        // calculate 'beginningOfArguments', which is the index of the first argument
        size_t beginningOfArguments;
        if (tokens[i + 2].type == GREATER_THAN) {
            size_t depth = 1;
            size_t j = i + 3;
            while (depth > 0) {
                if (tokens[j].type == GREATER_THAN) {
                    depth++;
                }
                else if (tokens[j].type == LESS_THAN) {
                    depth--;
                }
                j++;
            }
            if (tokens[j].type == OPEN_PARENTHESIS) {
                beginningOfArguments = j + 1;
            }
            else {
                std::cout << "Expected a function call at line " << tokens[j].lineNum << " but there was not a parenthesis ( found a '" << Token::toString(tokens[j].type) << "' instead" << std::endl;
                return false;
            }
        }
        else if (tokens[i + 2].type == OPEN_PARENTHESIS) {
            beginningOfArguments = i + 2;
        }
        else {
            std::cout << "Expected a function call at line " << tokens[i + 2].lineNum << " but there was not a parenthesis ( found a '" << Token::toString(tokens[i + 2].type) << "' instead" << std::endl;
            return false;
        }
        
        // calculate the beginning of the expression that is calling the function.  E.g. a.foo(), (a + b).foo(), a()[5].foo(), etc.
        size_t startingIndex = i - 1;
        size_t endingIndex = i;
        size_t depth = (tokens[i - 1].type == CLOSE_PARENTHESIS || tokens[i - 1].type == CLOSE_BRACKET || tokens[i - 1].type == GREATER_THAN) ? 1 : 0;
        while (depth > 0) {
            startingIndex--;
            if (tokens[startingIndex].type == CLOSE_PARENTHESIS || tokens[startingIndex].type == CLOSE_BRACKET || tokens[startingIndex].type == GREATER_THAN) {
                depth++;
            }
            else if (tokens[startingIndex].type == OPEN_PARENTHESIS || tokens[startingIndex].type == OPEN_BRACKET || tokens[startingIndex].type == LESS_THAN) {
                depth--;
            }
            if (depth == 0 && (tokens[startingIndex - 1].type == CLOSE_PARENTHESIS || tokens[startingIndex - 1].type == CLOSE_BRACKET || tokens[startingIndex - 1].type == GREATER_THAN)) {
                depth++;
                startingIndex--;
            }
        }
        
        // if it is *not* of the form (...).foo(...), then there is probably an identifier that begins the sequence of function calls, accesses, etc.
        if (tokens[startingIndex - 1].type == IDENTIFIER) {
            startingIndex--;
        }
        
        // before we cut-and-paste, replace the period with a comma
        tokens[i] = Token(COMMA, ",", tokens[i].lineNum, tokens[i].charNum);
        
        // perform cut-paste
        move(tokens, startingIndex, endingIndex - startingIndex + 1, startingIndex + beginningOfArguments - endingIndex);
    }
    return true;
}