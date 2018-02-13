#ifndef __DEFINE_TREE_H__
#define __DEFINE_TREE_H__

#include "tree.h"
#include "expr_tree.h"

class DefineTree : public Tree {
public:
    DefineTree() : Tree(TREE_DEFINE) { }
	DefineTree(std::shared_ptr<Token> name, std::shared_ptr<ExprTree> value)
	: Tree(TREE_DEFINE), _name(name), _value(value) { }

    void parse(Parser *parser) override;

    std::shared_ptr<Token> name() const {
        return _name;
    }
    std::shared_ptr<ExprTree> value() const {
        return _value;
    }
private:
    std::shared_ptr<Token> _name;
    std::shared_ptr<ExprTree> _value;
};

#endif

