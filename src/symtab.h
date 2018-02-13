#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include <map>
#include <list>
#include "tree.h"

class DefineTree;
class StructTree;

class SymbolTable {
public:
    typedef std::list<std::shared_ptr<Tree>>::iterator iterator;
public:
    std::shared_ptr<Tree> probe(const char *name, std::shared_ptr<Tree> tree, bool export_it = true);
    std::shared_ptr<Tree> get(const char *name);
    void exportSymbol(std::shared_ptr<Tree> tree);

    iterator begin() {
        return _list.begin();
    }

    iterator end() {
        return _list.end();
    }

private:
    std::map<const char*, std::shared_ptr<Tree>> _map;
    std::list<std::shared_ptr<Tree>> _list;
};

#endif

