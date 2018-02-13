#ifndef __MESSAGE_TREE_H__
#define __MESSAGE_TREE_H__

#include "struct_tree.h"
#include "define_tree.h"

class MessageTree : public Tree {
public:
	MessageTree() : Tree(TREE_MESSAGE) { }
	void parse(Parser *parser) override;
	
	std::shared_ptr<Token> name() const {
		return _name;	
	}

	std::shared_ptr<DefineTree> id() const {
		return _id;
	}

	std::shared_ptr<StructTree> req() const {
		return _req;
	}

	std::shared_ptr<StructTree> rsp() const {
		return _rsp;
	}
protected:
	void parseBody(Parser *parser);

protected:
	std::shared_ptr<Token> _name;
	std::shared_ptr<DefineTree> _id;
	std::shared_ptr<StructTree> _req;
	std::shared_ptr<StructTree> _rsp;
};

#endif

