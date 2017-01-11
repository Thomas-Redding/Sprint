#pragma once

#include <iostream>
#include "Token.hpp"

struct Class {
	Class() {};
	Class(std::string base_name) : base_name(base_name) {};
	Class(std::list<Token> tokens, std::list<Token>::iterator& it) {
		base_name = (++it)->str;
		Token base_name_token = *it;
		if ((++it)->type != LESS_THAN) {
			return;
		}
		it->type = OPEN_TEMPLATE;

		while (it->type != GREATER_THAN && ++it != tokens.end()) {
			TokenType templateType = KEYWORD_CLASS;
			std::string templateName;
			if ((*it).isIntKeyword()) {
				templateType = it->type;
				++it;
				if (it == tokens.end()) {
					throw std::runtime_error("Error: unexpected token on line " + std::to_string(base_name_token.lineNum) + "; expected an IDENTIFIER following a template parameter intger-type, but found EOF");
				}
				if (it->type != IDENTIFIER) {
					throw std::runtime_error("Error: unexpected token on line " + std::to_string(base_name_token.lineNum) + "; expected an IDENTIFIER following a template parameter integer-type, but found '" + it->str + "'");
				}
				templateName = it->str;
				++it;
				if (it == tokens.end()) {
					throw std::runtime_error("Error: unexpected token on line " + std::to_string(base_name_token.lineNum) + "; expected a ',' or '>' following a template parameter, but found EOF");
				}
				if (it->type != COMMA && it->type != GREATER_THAN) {
					throw std::runtime_error("Error: unexpected token on line " + std::to_string(base_name_token.lineNum) + "; expected a ',' or '>' following a template parameter, but found '" + it->str + "'");
				}
			}
			else if ((*it).type == IDENTIFIER) {
				templateName = (it++)->str;
				if (it == tokens.end()) {
					throw std::runtime_error("Error: unexpected token on line " + std::to_string(base_name_token.lineNum) + "; expected a ',' or '>' following a template parameter, but found EOF");
				}
				if (it->type != COMMA && it->type != GREATER_THAN) {
					throw std::runtime_error("Error: unexpected token on line " + std::to_string(base_name_token.lineNum) + "; expected a ',' or '>' following a template parameter, but found '" + it->str + "'");
				}
			}
			else {
				throw std::runtime_error("Error: unexpected token on line " + std::to_string(base_name_token.lineNum) + "; expected a template parameter (IDENTIFIER or an integer class), but found '" + it->str + "'");
			}
			template_parameters.push_back(std::pair<TokenType, std::string>(templateType, templateName));
		}
		if (it == tokens.end()) {
			throw std::runtime_error("Error: could not close templated variable on line " + std::to_string(base_name_token.lineNum));
		}
		if (it->type == GREATER_THAN) {
			it->type = CLOSE_TEMPLATE;
		}
	}
	friend std::ostream& operator<<(std::ostream& stream, const Class& c) {
		if (c.template_parameters.size() == 0) {
			return stream << c.base_name;
		}
		stream << c.base_name;

		if (c.template_parameters.size() > 0) {
			stream << "<" << Token(c.template_parameters[0].first) << " " << c.template_parameters[0].second;
			for (uint64_t i = 1; i < c.template_parameters.size(); ++i) {
				stream << ", " << Token(c.template_parameters[i].first) << " " << c.template_parameters[i].second;
			}
			stream << ">";
		}
		return stream;
	}
	std::string base_name;
	std::vector<std::pair<TokenType, std::string>> template_parameters;
};