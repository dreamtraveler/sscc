#ifndef __INCLUDE_TREE_H__
#define __INCLUDE_TREE_H__

#include "parser.h"
#include "tree.h"

class IncludeTree : public Tree {
public:
	IncludeTree(std::shared_ptr<Path> path) : Tree(TREE_INCLUDE), _path(path) { }

	void parse(Parser *parser) override;
	
	std::shared_ptr<Path> path() const {
		return _path;
	}
protected:
	std::shared_ptr<Path> _path;
};

#endif

