#ifndef __RANGE_TREE_H__
#define __RANGE_TREE_H__


#include "parser.h"
#include "tree.h"
#include "define_tree.h"

class RangeTree : public Tree {
public:
	RangeTree() : Tree(TREE_RANGE) { }

	void parse(Parser *parser) override;
	void parse(Parser *parser, int end);

	std::shared_ptr<DefineTree> min() const {
		return _min;
	}

	std::shared_ptr<DefineTree> max() const {
		return _max;
	}

private:
	std::shared_ptr<DefineTree> _min;
	std::shared_ptr<DefineTree> _max;
};


#endif

