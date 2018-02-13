#ifndef __STRUCT_TREE_H__
#define __STRUCT_TREE_H__

#include <list>
#include "tree.h"
#include "parser.h"

class MessageTree;

class StructTree : public Tree {
public:
    StructTree() : Tree(TREE_STRUCT), message() { }
    StructTree(std::shared_ptr<Token> name) : Tree(TREE_STRUCT), _name(name), message() { }

    void parse(Parser *parser) override;

    std::shared_ptr<Token> name() const {
        return _name;
    }

	void name(std::shared_ptr<Token> value) {
		_name = value;
	}

    std::shared_ptr<StructTree> inherited() const {
		return std::dynamic_pointer_cast<StructTree>(_inherited);
    }

    void parseInherited(Parser *parser, std::shared_ptr<StructTree> inherited);
    void parseBody(Parser *parser);
    void parseItems(Parser *parser);
    std::shared_ptr<Tree> getItem(const char *name, bool find_union);
    
    typedef std::list<std::shared_ptr<Tree>>::iterator iterator;

    iterator begin() {
        return _symbols.begin();
    }

    iterator end() {
        return _symbols.end();
    }

	size_t size() const {
		return _symbols.size();
	}
protected:
    void parseUnion(Parser *parser);
    void parseUnionPtr(Parser *parser);
protected:
    std::shared_ptr<Token> _name;
    std::shared_ptr<Tree> _inherited;
    std::list<std::shared_ptr<Tree>> _symbols;
public:
	MessageTree *message;
};

#endif

