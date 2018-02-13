#ifndef __STRUCT_ITEM_TREE_H__
#define __STRUCT_ITEM_TREE_H__

#include "tree.h"
#include "parser.h"
#include "type_tree.h"
#include "range_tree.h"

class StructItemTree : public Tree {
public:
    StructItemTree() : Tree(TREE_STRUCT_ITEM) {
		_type = std::make_shared<TypeTree>();
		_range = std::make_shared<RangeTree>();
	}
    void parse(Parser *parser) override;

	std::shared_ptr<Token> name() const {
		return _name;
	}

	std::shared_ptr<TypeTree> type() const {
		return _type;
	}

	std::shared_ptr<RangeTree> array() const {
		return _array;
	}

	std::shared_ptr<RangeTree> range() const {
		return _range;
	}

	std::shared_ptr<ExprTree> defaultValue() const {
		return _default;
	}
public:
	std::shared_ptr<Token> _name;
	std::shared_ptr<TypeTree> _type;
	std::shared_ptr<RangeTree> _array;
	std::shared_ptr<RangeTree> _range;
	std::shared_ptr<ExprTree> _default;
};

#endif

