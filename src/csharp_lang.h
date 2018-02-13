#ifndef __CSHARP_LANG_H__
#define __CSHARP_LANG_H__

#include "lang.h"
#include "csharp_printer.h"
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

class CsharpLang : public Language {
public:
    CsharpLang();
    void option(const char *name, std::shared_ptr<Token> value) override;
    void print(SymbolTable &symbols, FILE *file) override;

protected:
    void print_define(CsharpPrinter &printer, std::shared_ptr<DefineTree> tree);
    void print_struct(CsharpPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_message(CsharpPrinter &printer, std::shared_ptr<MessageTree> tree);
    void print_include(CsharpPrinter &printer, std::shared_ptr<IncludeTree> tree);

    void print_dump(CsharpPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_serial(CsharpPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_unserial(CsharpPrinter &printer, std::shared_ptr<StructTree> tree);

    void print_indent(CsharpPrinter &printer);

    void print_var(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, bool union_item);

    const char *type_decl(std::shared_ptr<TypeTree> tree, bool union_item);
    const char *type_decl(std::shared_ptr<StructItemTree> tree, bool union_item);

    void print_base_var_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_base_var_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_base_var_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);

    void print_struct_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);

    void print_array_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_array_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_array_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);

    void print_var_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);

    void print_constructor(CsharpPrinter &printer, std::shared_ptr<StructTree> tree);
protected:
    const char *_serial_name;
    const char *_unserial_name;
};

#endif