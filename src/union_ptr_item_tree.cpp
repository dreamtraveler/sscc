#include "union_ptr_item_tree.h"

void UnionPtrItemTree::parse(Parser *parser) {
	std::shared_ptr<Token> token = parser->cur();
	if (!token->is_iden()) {
		log_expect(token->loc(), "identifier");
	}
	parser->eat();

	std::shared_ptr<Tree> tree;
	tree = parser->symbols().get(token->text());
	if (!tree) {
		log_error(token->loc(), "unknown index name '%s'", token->text());
	}
	if (tree->type() != TREE_DEFINE) {
		log_error(token->loc(), "'%s' is not a define", token->text());
	}
	_index = std::dynamic_pointer_cast<DefineTree>(tree);

	if (_index->value()->exprType() != EXPR_INT) {
		log_error(_index->loc, "union index must is a integer");
	}
	if (parser->cur()->type() != ':') {
		log_expect(parser->cur()->loc(), "':'");
	}
	parser->eat();

	token = parser->cur();
	if (!token->is_iden()) {
		log_expect(token->loc(), "identifier");
	}
	parser->eat();

	tree = parser->symbols().get(token->text());
	if (!tree) {
		log_error(token->loc(), "unknown type name '%s'", token->text());
	}
	if (tree->type() != TREE_STRUCT) {
		log_error(token->loc(), "'%s' is not a struct", token->text());
	}
	_decl = std::dynamic_pointer_cast<StructTree>(tree);

	token = parser->cur();
	if (token->type() != ';') {
		log_expect(token->loc(), "';'");
	}
	parser->eat();
}

