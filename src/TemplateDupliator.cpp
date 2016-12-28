#include "../TemplateDuplicator.hpp"

TemplateDuplicator::expand_tree(ParseNode* root) {

	switch (root.type) {
		case function_implementation:
			
		break;
		case function_declaration:
		break;
		case enum_implementation:
		break;
		case namespace_implementation:
		break;
		case class_declaration:
		break;
		case class_implementation:
		break;
		case variable_dec:
		break;
		default:
			std::cout << "Error expanding templates" << std::endl;
			exit(0);
	}
}