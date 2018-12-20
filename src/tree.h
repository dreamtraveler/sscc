#ifndef __TREE_H__
#define __TREE_H__

#include <memory>
#include "token.h"

class Parser;

enum {
    TREE_DEFINE,
    TREE_EXPR,
    TREE_STRUCT,
	TREE_STRUCT_ITEM,
    TREE_CLASS,
    TREE_CLASS_ITEM,
	TREE_TYPE,
    TREE_VAR,
	TREE_UNION,
	TREE_UNION_ITEM,
	TREE_UNION_PTR,
	TREE_UNION_PTR_ITEM,
	TREE_RANGE,
	TREE_MESSAGE,
	TREE_INCLUDE,
};

class Tree {
public:
    Tree(int type) : _type(type) { }
    virtual void parse(Parser *parser) = 0;
public:
    int type() const {
        return _type;
    }
	void resetType(int v) {
		_type = v;
	}
    Location loc;
private:
    int _type;
};

#endif
