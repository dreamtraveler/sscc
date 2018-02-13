#include "range_tree.h"

void RangeTree::parse(Parser *parser) {
	std::shared_ptr<Tree> tree;
	std::shared_ptr<Token> token = parser->cur();
	if (token->is_iden()) {
		parser->eat();

		tree = parser->symbols().get(token->text());
		if (!tree) {
			log_error(token->loc(), "unknown define name '%s'", token->text());
		}
		if (tree->type() != TREE_DEFINE) {
			log_error(token->loc(), "'%s' is not a define", token->text());
		}

		_min = std::dynamic_pointer_cast<DefineTree>(tree);
		token = parser->cur();
	}

	if (token->type() == TOKEN_RANGE) {
		parser->eat();
		token = parser->cur();
	}
	else {
		_max = _min;
		return;
	}

	if (token->is_iden()) {
		parser->eat();

		tree = parser->symbols().get(token->text());
		if (!tree) {
			log_error(token->loc(), "unknown define name '%s'", token->text());
		}
		if (tree->type() != TREE_DEFINE) {
			log_error(token->loc(), "'%s' is not a define", token->text());
		}

		_max = std::dynamic_pointer_cast<DefineTree>(tree);
	}
}


