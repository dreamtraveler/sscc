#ifndef __PROTOBUF_LANG_H__
#define __PROTOBUF_LANG_H__

#include "lang.h"
#include "protobuf_printer.h"
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

class ProtobufLang : public Language {
public:
    ProtobufLang();
    void option(const char *name, std::shared_ptr<Token> value) override;
    void print(SymbolTable &symbols, FILE *file) override;

protected:
    void print_define(ProtobufPrinter &printer, std::shared_ptr<DefineTree> tree);
    void print_struct(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree, bool useClass = false);
    void print_message(ProtobufPrinter &printer, std::shared_ptr<MessageTree> tree);
    void print_include(ProtobufPrinter &printer, std::shared_ptr<IncludeTree> tree);

    void print_dump(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_serial(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree);
    void print_unserial(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree);

    void print_indent(ProtobufPrinter &printer);

    void print_var(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, int idx);

    const char *type_decl(std::shared_ptr<TypeTree> tree);
    const char *type_decl(std::shared_ptr<StructItemTree> tree);
	const char *type_default_value(std::shared_ptr<TypeTree> tree);

    void print_base_var_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_base_var_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_base_var_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);

    void print_struct_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer);

    void print_array_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_array_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);
    void print_array_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name);

    void print_var_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer);

    void print_constructor(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree);
protected:
    const char *_serial_name;
    const char *_unserial_name;
};

#endif