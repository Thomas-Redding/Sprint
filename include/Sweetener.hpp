#pragma once

#include <vector>
#include <iostream>

#include "Token.hpp"

// take a slice of a vector and move it to a new location
// runs in O(to - from + length) time and takes O(length) space
template<class T>
void move(std::vector<T>& A, size_t from, size_t length, size_t to);

// takes member functionc alls of the form "A.B(C)" and makes them "B(A, C)"
// note: "A.B()" will have a trailing comma: "B(A,)"
bool addFunctionSugar(std::vector<Token>& tokens);