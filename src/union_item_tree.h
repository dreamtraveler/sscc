#ifndef __UNION_ITEM_TREE_H__
#define __UNION_ITEM_TREE_H__

#include "parser.h"
#include "tree.h"
#include "define_tree.h"
#include "struct_item_tree.h"

class UnionItemTree : public Tree {
public:
	UnionItemTree() : Tree(TREE_UNION_ITEM) {
		_decl = std::make_shared<StructItemTree>();
	}
	void parse(Parser *parser) override;

	std::shared_ptr<DefineTree> index() const {
		return _index;
	}

	std::shared_ptr<StructItemTree> decl() const {
		return _decl;
	}
private:
	std::shared_ptr<DefineTree> _index;
	std::shared_ptr<StructItemTree> _decl;
};


#endif

