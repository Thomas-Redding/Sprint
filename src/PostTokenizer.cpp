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
			auto startIt = it;
			Class c(list, it);
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
			uint64_t lineNum = it->lineNum;
			auto it_copy = it;
			while (--it != list.begin()) {
				if (it->type == OPEN_PARENTHESIS) {
					break;
				}
			}

			if (it->type != OPEN_PARENTHESIS) {
				throw std::runtime_error("Error: found '->' on line " + std::to_string(lineNum) + " but could not find accompanying arguments for the function");
			}
			--it;
			if (it->type == GREATER_THAN) {
				it->type = CLOSE_TEMPLATE;
				numberOfClassesAddedAtCurrentDepth.push(0);
				numberOfClassesOverwritten.push(0);
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
					throw std::runtime_error("Error: found '...>(...) ->' on line " + std::to_string(lineNum) + " but could not find accompanying '<' for the templates of the function");
				}
				--it;
			}
			if (it->type != IDENTIFIER) {
				throw std::runtime_error("Error: expected IDENTIFIER at the start of a function declaration on line " + std::to_string(lineNum) + " but found '" + it->str + "'");
			}
			it->type = FUNC_DECL_IDENTIFIER;
			it = it_copy;
			while (++it != list.end()) {
				if (it->type == SEMI_COLON) {
					// remove added classes
					for (uint64_t i = 0; i < numberOfClassesAddedAtCurrentDepth.top(); ++i) {
						currentClasses.erase(classesAdded.top());
						classesAdded.pop();
					}
					std::cout << "#un-add " << numberOfClassesAddedAtCurrentDepth.top() << std::endl;
					numberOfClassesAddedAtCurrentDepth.pop();

					// re-overwrite overwritten classes
					for (uint64_t i = 0; i < numberOfClassesOverwritten.top(); ++i) {
						currentClasses[classesOverwritten.top().base_name] = classesOverwritten.top();
						classesOverwritten.pop();
					}
					std::cout << "#un-overwrite" << numberOfClassesOverwritten.top() << std::endl;
					numberOfClassesOverwritten.pop();

					std::cout << "function decl (';')" << std::endl;
					break;
				}
				else if (it->type == OPEN_CURLY_BRACE) {
					break;
				}
			}
			if (it == list.end()) {
				throw std::runtime_error("Error: function declared at line " + std::to_string(lineNum) + " does not end in a semicolon or a '{'");
			}
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
			if (it->isPrimitive() || currentClasses.count(it->str) > 0) {
				++it;
				it->type = PTR;
			}
			else {
				++it;
			}
		}
	}
}