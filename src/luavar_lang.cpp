#include "luavar_lang.h"
#include "printer.h"
#include "define_tree.h"

LuaVarLang::LuaVarLang() : Language("luavar") { 
}

void LuaVarLang::option(const char *name, std::shared_ptr<Token> value) {
}

void LuaVarLang::print(SymbolTable &symbols, FILE *file) {
    Printer printer;
    printer.open(file);

    for (auto sym : symbols) {
        if (sym->type() != TREE_DEFINE) {
            continue;
        }
		std::shared_ptr<DefineTree> tree = std::dynamic_pointer_cast<DefineTree>(sym);
        switch (tree->value()->exprType()) {
        case EXPR_INT:
            printer.println("%s = %lli;", tree->name()->text(), tree->value()->vint());
            break;
        case EXPR_STRING:
            printer.println("%s = \"%s\";", tree->name()->text(), tree->value()->vstr());
            break;
        default:
            assert(0);
            break;
        }
    }
}


