#ifndef __UNION_TREE_H__
#define __UNION_TREE_H__

#include <list>
#include "parser.h"
#include "tree.h"
#include "union_item_tree.h"
#include "struct_item_tree.h"

class StructTree;

class UnionTree : public Tree {
public:
	UnionTree(StructTree *the_struct) : Tree(TREE_UNION), _struct(the_struct) { }
	void parse(Parser *parser) override;
	
	std::shared_ptr<Token> name() const {
		return _name;
	}

	std::shared_ptr<StructItemTree> key() const {
		return _key;
	}

	typedef std::list<std::shared_ptr<UnionItemTree>>::iterator iterator;

	iterator begin() {
		return _symbols.begin();
	}

	iterator end() {
		return _symbols.end();
	}

	size_t size() const {
		return _symbols.size();
	}
private:
	StructTree *_struct;
	std::shared_ptr<StructItemTree> _key;
	std::shared_ptr<Token> _name;
	std::list<std::shared_ptr<UnionItemTree>> _symbols;
};

#endif

