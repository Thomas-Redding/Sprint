#include <stack>
#include <vector>
#include <list>
#include <unordered_map>

#include "../include/PostTokenizer.hpp"
#include "../include/Class.hpp"

void postTokenize(std::list<Token>& list) {

	std::unordered_map<std::string, Class> currentClasses;
	std::stack<uint64_t> numberOfClassesAddedAtCurrentDepth;
	std::stack<uint64_t> numberOfClassesOverwritten;
	std::stack<std::string> classesAdded;
	std::stack<Class> classesOverwritten;

	numberOfClassesAddedAtCurrentDepth.push(0);
	numberOfClassesOverwritten.push(0);
	int64_t template_depth = 0;
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (it->type == OPEN_CURLY_BRACE) {
			numberOfClassesAddedAtCurrentDepth.push(0);
			numberOfClassesOverwritten.push(0);
		}
		else if (it->type == CLOSE_CURLY_BRACE) {
			// remove added classes
			for (uint64_t i = 0; i < numberOfClassesAddedAtCurrentDepth.top(); ++i) {
				currentClasses.erase(classesAdded.top());
				classesAdded.pop();
			}
			numberOfClassesAddedAtCurrentDepth.pop();

			// re-overwrite overwritten classes
			for (uint64_t i = 0; i < numberOfClassesOverwritten.top(); ++i) {
				currentClasses[classesOverwritten.top().base_name] = classesOverwritten.top();
				classesOverwritten.pop();
			}
			numberOfClassesOverwritten.pop();
		}
		else if (it->type == KEYWORD_CLASS) {

			// add a class to the stack

			auto startIt = it;
			Class c(list, it);
			++startIt;
			startIt->type = CLASS_IDENTIFIER;
			--startIt;
			if (currentClasses.count(c.base_name) > 0) {
				classesOverwritten.push(currentClasses.find(c.base_name)->second);
				currentClasses[c.base_name] = c;
				numberOfClassesOverwritten.top()++;
			}
			else {
				currentClasses.insert(std::pair<std::string, Class>(c.base_name, c));
				numberOfClassesAddedAtCurrentDepth.top()++;
				classesAdded.push(c.base_name);
			}
			if (it->type != OPEN_CURLY_BRACE) ++it;
			if (it->type == OPEN_CURLY_BRACE) {
				// add template parameters to the class-stack
				numberOfClassesAddedAtCurrentDepth.push(0);
				numberOfClassesOverwritten.push(0);
				for (uint64_t i = 0; i < c.template_parameters.size(); ++i) {
					std::string name = c.template_parameters[i].second;
					if (c.template_parameters[i].first == KEYWORD_CLASS) {
						if (currentClasses.count(name) > 0) {
							classesOverwritten.push(currentClasses.find(c.base_name)->second);
							currentClasses[name] = Class(name);
							numberOfClassesOverwritten.top()++;
						}
						else {
							currentClasses.insert(std::pair<std::string, Class>(name, Class(name)));
							numberOfClassesAddedAtCurrentDepth.top()++;
							classesAdded.push(name);
						}
					}
				}
			}
		}
		else if (it->type == ARROW) {

			// convert function name to type 'FUNC_DECL_IDENTIFIER'
			// add any template-classes declared by a functio to the stack

			std::string lineNum = std::to_string(it->lineNum);
			auto it_arrow = it;
			--it;
			if (it->type != CLOSE_PARENTHESIS) {
				std::runtime_error("Error: expected ')' before '->' on line " + lineNum + " but found '" + it->str + "'");
			}

			// move iterator to '('
			while (--it != list.begin()) {
				if (it->type == OPEN_PARENTHESIS) {
					break;
				}
			}
			if (it == list.begin()) {
				std::runtime_error("Error: expected arguments '( ... )' before '->' on line " + lineNum + " but couldn't find '('");
			}

			auto it_openP = it;
			
			--it;
			bool isTemplated = (it->type == GREATER_THAN);
			if (isTemplated) {
				it->type = CLOSE_TEMPLATE;
				// add classes in the template to 'currentClasses'
				while (--it != list.begin()) {
					if (it->type == LESS_THAN) {
						it->type = OPEN_TEMPLATE;
						break;
					}
					else if (it->type == IDENTIFIER) {
						if (currentClasses.count(it->str) > 0) {
							classesOverwritten.push(currentClasses.find(it->str)->second);
							currentClasses[it->str] = Class(it->str);
							numberOfClassesOverwritten.top()++;
						}
						else {
							currentClasses.insert(std::pair<std::string, Class>(it->str, Class(it->str)));
							numberOfClassesAddedAtCurrentDepth.top()++;
							classesAdded.push(it->str);
						}
					}
				}
				if (it == list.begin()) {
					throw std::runtime_error("Error: expected function's template arguments to begin with '<' on line " + lineNum);
				}
				--it;
			}
			it->type = FUNC_DECL_IDENTIFIER;


			// convert 'IDENTIFIER's between the parentheses into 'CLASS_IDENTIFIER'
			it = it_openP;
			while (++it != list.end()) {
				if (it->type == CLOSE_PARENTHESIS) {
					break;
				}
				if (it->type == IDENTIFIER && currentClasses.count(it->str) > 0) {
					it->type = CLASS_IDENTIFIER;
				}
			}
			if (it == list.end()) {
				throw std::runtime_error("Error: expected arguments '( ... )' before '->' on line " + lineNum + " but couldn't find ')'");
			}

			it = it_arrow;
			while (++it != list.end()) {
				if (it->type == IDENTIFIER && currentClasses.count(it->str) > 0) {
					it->type = CLASS_IDENTIFIER;
				}
				else if (it->type == OPEN_CURLY_BRACE || it->type == SEMI_COLON) {
					break;
				}
				else if (it->type == GREATER_THAN) {
					it->type = CLOSE_TEMPLATE;
				}
				else if (it->type == LESS_THAN) {
					it->type = OPEN_TEMPLATE;
				}
				else if (it->type == SHIFT_RIGHT) {
					it->type = CLOSE_TEMPLATE;
					it->str = ">";
					list.insert(it, *it);
				}
			}
			if (it == list.end()) {
				throw std::runtime_error("Error: expected function on line " + lineNum + " to end with either a '{' or a ';'");
			}

			if (isTemplated && it->type == SEMI_COLON) {
				// remove classes added by the template
				for (uint64_t i = 0; i < numberOfClassesAddedAtCurrentDepth.top(); ++i) {
					currentClasses.erase(classesAdded.top());
					classesAdded.pop();
				}
				numberOfClassesAddedAtCurrentDepth.pop();

				// re-overwrite overwritten classes
				for (uint64_t i = 0; i < numberOfClassesOverwritten.top(); ++i) {
					currentClasses[classesOverwritten.top().base_name] = classesOverwritten.top();
					classesOverwritten.pop();
				}
				numberOfClassesOverwritten.pop();
			}
		}
		else if (it->type == IDENTIFIER && currentClasses.count(it->str) > 0) {
			it->type = CLASS_IDENTIFIER;
			++it;
			if (it->type != LESS_THAN) {
				--it;
				continue;
			}
			it->type = OPEN_TEMPLATE;
			++template_depth;
		}
		else if (template_depth > 0 && it->type == GREATER_THAN) {
			it->type = CLOSE_TEMPLATE;
			--template_depth;
		}
		else if (template_depth > 1 && it->type == SHIFT_RIGHT) {
			it->type = CLOSE_TEMPLATE;
			it->str = ">";
			list.insert(it, *it);
			it->charNum++;
			template_depth -= 2;
		}
		else if (it->type == ASTERISK) {
			--it;
			if (it->isPrimitive() || it->type == CLOSE_TEMPLATE || currentClasses.count(it->str) > 0) {
				++it;
				it->type = PTR;
			}
			else {
				++it;
			}
		}
	}
}