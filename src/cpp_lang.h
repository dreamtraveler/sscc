#ifndef __CPP_LANG_H__
#define __CPP_LANG_H__

#include "lang.h"
#include "cpp_printer.h"
#include "define_tree.h"
#include "struct_tree.h"
#include "struct_item_tree.h"
#include "type_tree.h"
#include "union_tree.h"
#include "union_ptr_tree.h"
#include "message_tree.h"
#include "include_tree.h"
#include "platform.h"

using namespace GX_NS;

class CppLang : public Language {
public:
    CppLang();
    void option(const char *name, std::shared_ptr<Token> value) override;
    void print(SymbolTable &symbols, FILE *file) override;

protected:
    void print_define(CppPrinter &printer, std::shared_ptr<DefineTree> tree);
    void print_struct(CppPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_message(CppPrinter &printer, std::shared_ptr<MessageTree> tree);
    void print_include(CppPrinter &printer, std::shared_ptr<IncludeTree> tree);

    void print_dump(CppPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_serial(CppPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_unserial(CppPrinter &printer, std::shared_ptr<StructTree> tree);

    void print_indent(CppPrinter &printer);

    void print_var(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, bool union_item);

    const char *type_decl(std::shared_ptr<TypeTree> tree, bool union_item);
    const char *type_decl(std::shared_ptr<StructItemTree> tree, bool union_item);

    void print_base_var_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_base_var_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_base_var_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);

    void print_struct_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);

    void print_array_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_array_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_array_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);

    void print_var_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);

    void print_constructor(CppPrinter &printer, std::shared_ptr<StructTree> tree);
protected:
    const char *_serial_name;
    const char *_unserial_name;
};

#endif