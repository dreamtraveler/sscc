#include "symtab.h"
#include "log.h"
#include "define_tree.h"
#include "struct_tree.h"


std::shared_ptr<Tree> SymbolTable::probe(const char *name, std::shared_ptr<Tree> tree, bool export_it) {
    auto em = _map.emplace(name, tree);
    if (export_it && em.second) {
        exportSymbol(tree);
    }
    return em.first->second; 
}

std::shared_ptr<Tree> SymbolTable::get(const char *name) {
    auto it = _map.find(name);
    if (it != _map.end()) {
        return it->second;
    }
    return nullptr;
}

void SymbolTable::exportSymbol(std::shared_ptr<Tree> tree) {
    _list.push_back(tree);
}


