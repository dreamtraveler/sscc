#include "cpp_lang.h"
#include <inttypes.h>
#include "pool.h"

CppLang::CppLang() : Language("cpp") {
}

void CppLang::option(const char *name, std::shared_ptr<Token> value) {

}

const char *CppLang::type_decl(std::shared_ptr<TypeTree> tree, bool union_item) {
    switch (tree->type()) {
    case TYPE_INT8:
        return "int8_t";
    case TYPE_UINT8:
        return "uint8_t";
    case TYPE_INT16:
        return "int16_t";
    case TYPE_UINT16:
        return "uint16_t";
    case TYPE_INT32:
        return "int32_t";
    case TYPE_UINT32:
        return "uint32_t";
    case TYPE_INT64:
        return "int64_t";
    case TYPE_UINT64:
        return "uint64_t";
    case TYPE_FLOAT:
        return "float";
    case TYPE_DOUBLE:
        return "double";
    case TYPE_STRING:
        return "std::string";
    case TYPE_STRUCT:
        return tree->decl()->name()->text();
    default:
        assert(0);
        return nullptr;
    }
}

const char *CppLang::type_decl(std::shared_ptr<StructItemTree> tree, bool union_item) {
    const char *type;
    if (tree->array()) {
        type = type_decl(tree->type(), false);
        type = Pool::instance()->printf("std::vector<%s>", type);
    }
    else {
        type = type_decl(tree->type(), union_item);
    }
    return type;
}

void CppLang::print_define(CppPrinter &printer, std::shared_ptr<DefineTree> tree) {
    switch (tree->value()->exprType()) {
    case EXPR_INT:
        printer.d(tree->name()->text(), "0x%llx", tree->value()->vint());
        break;
    case EXPR_STRING:
        printer.d(tree->name()->text(), "\"%s\"", tree->value()->vstr());
        break;
    default:
        assert(0);
        break;
    }
}

void CppLang::print_var(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, bool union_item) {
    printer.s("%s %s", type_decl(tree, union_item), tree->name()->text());
}

void CppLang::print_base_var_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "buf->AppendInt8";
        break;
    case TYPE_UINT8:
        method = "buf->AppendUint8";
        break;
    case TYPE_INT16:
        method = "buf->AppendInt16";
        break;
    case TYPE_UINT16:
        method = "buf->AppendUint16";
        break;
    case TYPE_INT32:
        method = "buf->AppendInt32";
        break;
    case TYPE_UINT32:
        method = "buf->AppendUint32";
        break;
    case TYPE_INT64:
        method = "buf->AppendInt64";
        break;
    case TYPE_UINT64:
        method = "buf->AppendUint64";
        break;
    case TYPE_FLOAT:
        method = "buf->AppendFloat";
        break;
    case TYPE_DOUBLE:
        method = "buf->AppendDouble";
        break;
    case TYPE_STRING:
        method = "buf->AppendString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s(%s)", method, name);
}

void CppLang::print_base_var_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "buf->ReadInt8";
        break;
    case TYPE_UINT8:
        method = "buf->ReadUint8";
        break;
    case TYPE_INT16:
        method = "buf->ReadInt16";
        break;
    case TYPE_UINT16:
        method = "buf->ReadUint16";
        break;
    case TYPE_INT32:
        method = "buf->ReadInt32";
        break;
    case TYPE_UINT32:
        method = "buf->ReadUint32";
        break;
    case TYPE_INT64:
        method = "buf->ReadInt64";
        break;
    case TYPE_UINT64:
        method = "buf->ReadUint64";
        break;
    case TYPE_FLOAT:
        method = "buf->ReadFloat";
        break;
    case TYPE_DOUBLE:
        method = "buf->ReadDouble";
        break;
    case TYPE_STRING:
        method = "buf->ReadString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s = %s()", name, method);
}

void CppLang::print_base_var_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    switch (tree->type()->type()) {
    case TYPE_INT8:
        printer.s("SSCC_PRINT(\"%%d(0x%%x)\", (int)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_UINT8:
        printer.s("SSCC_PRINT(\"%%u(0x%%x)\", (unsigned)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_INT16:
        printer.s("SSCC_PRINT(\"%%d(0x%%x)\", (int)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_UINT16:
        printer.s("SSCC_PRINT(\"%%u(0x%%x)\", (unsigned)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_INT32:
        printer.s("SSCC_PRINT(\"%%d(0x%%x)\", (int)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_UINT32:
        printer.s("SSCC_PRINT(\"%%u(0x%%x)\", (unsigned)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_INT64:
        printer.s("SSCC_PRINT(\"%%" PRId64 "(0x%%" PRIx64 ")\", %s, %s)", name, name);
        break;
    case TYPE_UINT64:
        printer.s("SSCC_PRINT(\"%%" PRIu64 "(0x%%" PRIx64 ")\", %s, %s)", name, name);
        break;
    case TYPE_FLOAT:
        printer.s("SSCC_PRINT(\"%%f\", (double)%s)", name);
        break;
    case TYPE_DOUBLE:
        printer.s("SSCC_PRINT(\"%%f\", %s)", name);
        break;
    case TYPE_STRING:
        printer.s("SSCC_PRINT(\"\\\"%%s\\\"\", SSCC_STRING_CSTR(%s))", name);
        break;
    default:
        assert(0);
        return;
    }
}

void CppLang::print_struct_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.s("%s->serial(buf)", name);
    }
    else {
        printer.s("%s.serial(buf)", name);
    }
}

void CppLang::print_struct_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.s("s->unserial(buf)", name);
    }
    else {
        printer.s("%s.unserial(buf)", name);
    }
}

void CppLang::print_struct_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    printer.s("SSCC_PRINT(\"{\\n\")");
    printer.s("++sscc_indent");
    if (is_pointer) {
        printer.s("SSCC_POINTER_GET(%s)->SSCC_DUMP_FUNC(sscc_indent SSCC_DUMP_PARAM)", name);
    }
    else {
        printer.s("%s.SSCC_DUMP_FUNC(sscc_indent SSCC_DUMP_PARAM)", name);
    }
    printer.s("--sscc_indent");
    print_indent(printer);
    printer.s("SSCC_PRINT(\"}\")");
}

void CppLang::print_array_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    printer.s("buf->AppendUint32(%s.size())", name);

    printer.for_("auto &sscc_i : %s", name); {
        if (tree->type()->type() == TYPE_STRUCT) {
            print_struct_serial(printer, tree, "sscc_i", false);
        }
        else {
            print_base_var_serial(printer, tree, "sscc_i");
        }
    }
    printer.end();
}

void CppLang::print_array_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    printer.do_(); {
        printer.s("uint32_t sscc_size = buf->ReadUint32()");
        printer.for_("size_t sscc_i = 0 ; sscc_i < sscc_size; ++sscc_i"); {
            printer.s("%s.emplace_back()", name);
            if (tree->type()->type() == TYPE_STRUCT) {
                print_struct_unserial(printer, tree, Pool::instance()->printf("%s.back()", name), false);
            }
            else {
                print_base_var_unserial(printer, tree, Pool::instance()->printf("%s.back()", name));
            }
        }
        printer.end();
    }
    printer.end();
}

void CppLang::print_array_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    printer.s("SSCC_PRINT(\"{\\n\")");
    printer.s("++sscc_indent");

    printer.do_(); {
        printer.s("size_t sscc_i = 0");
        printer.for_("auto &sscc_obj : %s", name); {
            print_indent(printer);
            printer.s("SSCC_PRINT(\"[%%lu] = \", sscc_i++)");
            name = "sscc_obj";
            if (tree->type()->type() == TYPE_STRUCT) {
                print_struct_dump(printer, tree, name, false);
            }
            else {
                print_base_var_dump(printer, tree, name);
            }
            printer.s("SSCC_PRINT(\",\\n\")");
        }
        printer.end();
    }
    printer.end();
    printer.s("--sscc_indent");
    print_indent(printer);
    printer.s("SSCC_PRINT(\"}\")", name);
}

void CppLang::print_var_serial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    const char *name;
    if (prefix) {
        name = Pool::instance()->printf("this->%s.%s", prefix, tree->name()->text());
    }
    else {
        name = Pool::instance()->printf("this->%s", tree->name()->text());
    }
    if (tree->array()) {
        print_array_serial(printer, tree, name);
        return;
    }
    if (tree->type()->type() == TYPE_STRUCT) {
        print_struct_serial(printer, tree, name, is_pointer);
        return;
    }
    print_base_var_serial(printer, tree, name);
}

void CppLang::print_var_unserial(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    const char *name;
    if (prefix) {
        name = Pool::instance()->printf("%s.%s", prefix, tree->name()->text());
    }
    else {
        name = Pool::instance()->printf("this->%s", tree->name()->text());
    }
    if (tree->array()) {
        print_array_unserial(printer, tree, name);
        return;
    }
    if (tree->type()->type() == TYPE_STRUCT) {
        print_struct_unserial(printer, tree, name, is_pointer);
        return;
    }
    print_base_var_unserial(printer, tree, name);
}

void CppLang::print_var_dump(CppPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    const char *name = tree->name()->text();
    print_indent(printer);
    printer.s("SSCC_PRINT(\"%s = \")", name);
    if (prefix) {
        name = Pool::instance()->printf("this->%s.%s", prefix, name);
    }
    else {
        name = Pool::instance()->printf("this->%s", name);
    }
    if (tree->array()) {
        print_array_dump(printer, tree, name);
    }
    else if (tree->type()->type() == TYPE_STRUCT) {
        print_struct_dump(printer, tree, name, is_pointer);
    }
    else {
        print_base_var_dump(printer, tree, name);
    }
}

void CppLang::print_serial(CppPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.function_("void serial(evpp::Buffer* buf) const override"); {
        if (tree->inherited()) {
            //printer.s("%s::serial(buf)", tree->inherited()->name()->text());
        }
        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var_serial(printer, std::dynamic_pointer_cast<StructItemTree>(sym), nullptr, false);
                break;
            default:
                assert(0);
                break;
            }
        }
    }
    printer.end();
};

void CppLang::print_unserial(CppPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.function_("void unserial(evpp::Buffer* buf) override"); {
        if (tree->inherited()) {
            //printer.s("%s::unserial(buf)", tree->inherited()->name()->text());
        }
        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var_unserial(printer, std::dynamic_pointer_cast<StructItemTree>(sym), nullptr, false);
                break;
            default:
                assert(0);
                break;
            }
        }
    }
    printer.end();
};

void CppLang::print_constructor(CppPrinter &printer, std::shared_ptr<StructTree> tree) {
    bool first = true;
    printer.println("%s()", tree->name()->text());
    if (tree->inherited()) {
        printer.print(": %s()", tree->inherited()->name()->text());
        first = false;
	} else {
        printer.print(":");
	}

    std::shared_ptr<StructItemTree> item;
    for (auto sym : *tree) {
        switch (sym->type()) {
        case TREE_STRUCT_ITEM:
            item = std::dynamic_pointer_cast<StructItemTree>(sym);
            if (first) {
                first = false;
            }
            else {
                printer.println(",");
            }

            if (item->array()) {
                printer.print("  %s()", item->name()->text(), type_decl(item, false));
                break;
            }
            if (item->type()->type() == TYPE_STRUCT) {
                printer.print("  %s()", item->name()->text());
                break;
            }
            if (item->type()->type() == TYPE_STRING) {
                printer.print("  %s()", item->name()->text(), type_decl(item, false));
                break;
            }
            printer.print("  %s()", item->name()->text());
            break;
        default:
            break;
        }
    }

    printer.println("");
    printer.println("{ }");
};

void CppLang::print_indent(CppPrinter &printer) {
    printer.s("SSCC_PRINT_INDENT(sscc_indent)");
}

void CppLang::print_dump(CppPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.p("#ifdef SSCC_USE_DUMP");
    printer.function_("void SSCC_DUMP_FUNC(unsigned sscc_indent SSCC_DUMP_PARAM_DECL) override"); {
        if (tree->inherited()) {
            printer.s("%s::SSCC_DUMP_FUNC(sscc_indent SSCC_DUMP_PARAM)", tree->inherited()->name()->text());
        }

        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var_dump(printer, std::dynamic_pointer_cast<StructItemTree>(sym), nullptr, false);
                break;
            default:
                assert(0);
                break;
            } 
            printer.s("SSCC_PRINT(\",\\n\")");
        }
    }
    printer.end();

    printer.function_("void SSCC_DUMP_FUNC(const char *sscc_name, unsigned sscc_indent SSCC_DUMP_PARAM_DECL) override"); {
        printer.if_("!sscc_name"); {
            printer.s("sscc_name = the_class_name");
        }
        printer.end();
        print_indent(printer);
        printer.s("SSCC_PRINT(\"%%s = {\\n\", sscc_name)");
        printer.s("SSCC_DUMP_FUNC(sscc_indent + 1 SSCC_DUMP_PARAM)");
        print_indent(printer);
        printer.s("SSCC_PRINT(\"}\\n\")");
    }
    printer.end();
    printer.p("#endif");
}

void CppLang::print_struct(CppPrinter &printer, std::shared_ptr<StructTree> tree) {
    const char *inherited = tree->inherited() ? tree->inherited()->name()->text() : nullptr;
    printer.struct_(tree->name()->text(), inherited); {
        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var(printer, std::dynamic_pointer_cast<StructItemTree>(sym), false);
                break;
            default:
                assert(0);
                break;
            }
        }

        printer.println("");
        print_constructor(printer, tree);
        printer.println("");
        if (tree->message) {
			printer.function_("uint16_t msgid() const override"); {
				printer.s("return %s", tree->message->id()->name()->text());
			}
			printer.end();

			printer.function_("std::string msgname() const"); {
				printer.s("return \"%s\"", tree->name()->text());
			}
			printer.end();
		}
        printer.println("");
        print_serial(printer, tree);
        printer.println("");
        print_unserial(printer, tree);
        printer.println("");
        //print_dump(printer, tree);
        //printer.println("");
    }
    printer.end();
}

void CppLang::print_message(CppPrinter &printer, std::shared_ptr<MessageTree> tree) {
    printer.s("struct %s", tree->name()->text());

    print_struct(printer, tree->req());
    if (tree->rsp()) {
        print_struct(printer, tree->rsp());
    }

    printer.struct_(tree->name()->text(), "Servlet"); {
		printer.s("%s req", tree->req()->name()->text());
		if (tree->rsp()) {
			printer.s("%s rsp", tree->rsp()->name()->text());
		}
		if (tree->rsp()) {
			printer.s("%s(bool b) : Servlet(b), req(), rsp() {}", tree->name()->text());
		}
		else {
			printer.s("%s(bool b) : Servlet(b), req() {}", tree->name()->text());
		}

		printer.function_("uint16_t msgid() const override"); {
			printer.s("return %s", tree->id()->name()->text());
		}
		printer.end();

		printer.function_("std::string msgname() const override"); {
			printer.s("return \"%s\"", tree->id()->name()->text());
		}
		printer.end();

		printer.function_("%s* get_request() override", tree->req()->name()->text()); {
			printer.s("return &req");
		}
		printer.end();

		printer.function_("void serial_request(evpp::Buffer* buf) override"); {
			printer.s("req.serial(buf)");
		}
		printer.end();

		printer.function_("void unserial_request(evpp::Buffer* buf) override"); {
			printer.s("req.unserial(buf)");
		}
		printer.end();

		if (tree->rsp()){
			printer.function_("%s* get_response() override", tree->rsp()->name()->text()); {
				printer.s("return &rsp");
			}
			printer.end();
		} else {
			printer.function_("%s* get_response() override", tree->req()->inherited()->name()->text()); {
				printer.s("return nullptr");
			}
			printer.end();
		}

		printer.function_("void serial_response(evpp::Buffer* buf) override"); {
			if (tree->rsp())
				printer.s("rsp.serial(buf)");
		}
		printer.end();

		printer.function_("void unserial_response(evpp::Buffer* buf) override"); {
			if (tree->rsp())
				printer.s("rsp.unserial(buf)");
		}
		printer.end();

		printer.function_("std::shared_ptr<Servlet> clone() override"); {
			printer.s("return std::make_shared<%s>(need_rpc)", tree->name()->text());
		}
		printer.end();

		if (tree->rsp()){
			printer.s("int execute() override");
		}
		else {
			printer.s("int execute() {return 0;}");
		}
    }
    printer.end();
}

void CppLang::print_include(CppPrinter &printer, std::shared_ptr<IncludeTree> tree) {
    printer.p("#include \"%s.h\"", (tree->path()->directory() + tree->path()->basename()).c_str());
}

void CppLang::print(SymbolTable &symbols, FILE *file) {
    CppPrinter printer;
    printer.open(file);

    printer.println("#pragma once");
    printer.p(head().str().c_str());
    for (auto sym : symbols) {
        switch (sym->type()) {
        case TREE_DEFINE:
            print_define(printer, std::dynamic_pointer_cast<DefineTree>(sym));
            break;
        case TREE_STRUCT:
            print_struct(printer, std::dynamic_pointer_cast<StructTree>(sym));
            break;
        case TREE_MESSAGE:
            print_message(printer, std::dynamic_pointer_cast<MessageTree>(sym));
            break;
        case TREE_INCLUDE:
            print_include(printer, std::dynamic_pointer_cast<IncludeTree>(sym));
            break;
        default:
            assert(0);
            break;
        }
    }
    printer.p(tail().str().c_str());
} 