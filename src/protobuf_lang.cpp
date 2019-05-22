#include "protobuf_lang.h"
#include <inttypes.h>
#include "pool.h"

ProtobufLang::ProtobufLang() : Language("pb") {
}

void ProtobufLang::option(const char *name, std::shared_ptr<Token> value) {

}

const char *ProtobufLang::type_decl(std::shared_ptr<TypeTree> tree) {
    switch (tree->type()) {
    case TYPE_INT8:
        return "int32";
    case TYPE_UINT8:
        return "uint32";
    case TYPE_INT16:
        return "int32";
    case TYPE_UINT16:
        return "uint32";
    case TYPE_INT32:
        return "int32";
    case TYPE_UINT32:
        return "uint32";
    case TYPE_INT64:
        return "int64";
    case TYPE_UINT64:
        return "uint64";
    case TYPE_FLOAT:
        return "float";
    case TYPE_DOUBLE:
        return "double";
    case TYPE_STRING:
        return "string";
    case TYPE_STRUCT:
        return tree->decl()->name()->text();
    default:
        assert(0);
        return nullptr;
    }
}

const char *ProtobufLang::type_default_value(std::shared_ptr<TypeTree> tree) {
    switch (tree->type()) {
    case TYPE_INT8:
        return "0";
    case TYPE_UINT8:
        return "0";
    case TYPE_INT16:
        return "0";
    case TYPE_UINT16:
        return "0";
    case TYPE_INT32:
        return "0";
    case TYPE_UINT32:
        return "0";
    case TYPE_INT64:
        return "0";
    case TYPE_UINT64:
        return "0";
    case TYPE_FLOAT:
        return "0.0f";
    case TYPE_DOUBLE:
        return "0.0f";
    case TYPE_STRING:
        return "\"\"";
    case TYPE_STRUCT:
        return Pool::instance()->printf("new %s()", tree->decl()->name()->text());
    default:
        assert(0);
        return nullptr;
    }
}

const char *ProtobufLang::type_decl(std::shared_ptr<StructItemTree> tree) {
    const char *type;
    if (tree->array()) {
        type = type_decl(tree->type());
		if (tree->type()->type() == TYPE_UINT8 || tree->type()->type() == TYPE_INT8) {
			type = Pool::instance()->printf("bytes");
		}
		else {
			type = Pool::instance()->printf("repeated %s", type);
		}
    }
    else {
        type = type_decl(tree->type());
    }
    return type;
}

void ProtobufLang::print_define(ProtobufPrinter &printer, std::shared_ptr<DefineTree> tree) {
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

void ProtobufLang::print_var(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, int idx) {
	auto dv = tree->defaultValue();
	if (dv == nullptr) {
		log_info("[warning] no tag number.");
		printer.s("%s %s = %d", type_decl(tree), tree->name()->text(), idx);
		return;
	}
	else {
		int v = (int)dv->vint();
		printer.s("%s %s = %d", type_decl(tree), tree->name()->text(), v);
	}
}

void ProtobufLang::print_base_var_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "buf.WriteSbyte";
        break;
    case TYPE_UINT8:
        method = "buf.WriteByte";
        break;
    case TYPE_INT16:
        method = "buf.WriteShort";
        break;
    case TYPE_UINT16:
        method = "buf.WriteUshort";
        break;
    case TYPE_INT32:
        method = "buf.WriteInt";
        break;
    case TYPE_UINT32:
        method = "buf.WriteUint";
        break;
    case TYPE_INT64:
        method = "buf.WriteLong";
        break;
    case TYPE_UINT64:
        method = "buf.WriteUlong";
        break;
    case TYPE_FLOAT:
        method = "buf.WriteFloat";
        break;
    case TYPE_DOUBLE:
        method = "buf.WriteDouble";
        break;
    case TYPE_STRING:
        method = "buf.WriteString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s(%s)", method, name);
}

void ProtobufLang::print_base_var_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "buf.ReadSbyte";
        break;
    case TYPE_UINT8:
        method = "buf.ReadByte";
        break;
    case TYPE_INT16:
        method = "buf.ReadShort";
        break;
    case TYPE_UINT16:
        method = "buf.ReadUshort";
        break;
    case TYPE_INT32:
        method = "buf.ReadInt";
        break;
    case TYPE_UINT32:
        method = "buf.ReadUint";
        break;
    case TYPE_INT64:
        method = "buf.ReadLong";
        break;
    case TYPE_UINT64:
        method = "buf.ReadUlong";
        break;
    case TYPE_FLOAT:
        method = "buf.ReadFloat";
        break;
    case TYPE_DOUBLE:
        method = "buf.ReadDouble";
        break;
    case TYPE_STRING:
        method = "buf.ReadString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s = %s()", name, method);
}

void ProtobufLang::print_base_var_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
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

void ProtobufLang::print_struct_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.s("%s.Serial(buf)", name);
    }
    else {
        printer.s("%s.Serial(buf)", name);
    }
}

void ProtobufLang::print_struct_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.s("s.Unserial(buf)", name);
    }
    else {
        printer.s("%s.Unserial(buf)", name);
    }
}

void ProtobufLang::print_struct_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
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

void ProtobufLang::print_array_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
	if (tree->type()->type() == TYPE_UINT8 || tree->type()->type() == TYPE_INT8) {
		printer.s("buf.WriteBytes(%s)", name);
		return;
	}
    printer.s("buf.WriteUint((uint)%s.Count)", name);

	const char* tname;
	if (tree->type()->type() == TYPE_STRUCT) {
		tname = tree->type()->decl()->name()->text();
	}
	else {
        tname = type_decl(tree->type());
	}
    printer.foreach_("%s sscc_i in %s", tname, name); {
        if (tree->type()->type() == TYPE_STRUCT) {
            print_struct_serial(printer, tree, "sscc_i", false);
        }
        else {
            print_base_var_serial(printer, tree, "sscc_i");
        }
    }
    printer.end();
}

void ProtobufLang::print_array_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
	if (tree->type()->type() == TYPE_UINT8 || tree->type()->type() == TYPE_INT8) {
		printer.s("%s = buf.ReadBytes()", name);
		return;
	}

	const char* tname;
	if (tree->type()->type() == TYPE_STRUCT) {
		tname = tree->type()->decl()->name()->text();
	}
	else {
        tname = type_decl(tree->type());
	}
    printer.do_(); {
		printer.s("%s = new List<%s>()", name, tname);
        printer.s("uint sscc_size = buf.ReadUint()");
        printer.for_("uint sscc_i = 0 ; sscc_i < sscc_size; ++sscc_i"); {
            if (tree->type()->type() == TYPE_STRUCT) {
				printer.s("%s item = new %s()", tname, tname);
                print_struct_unserial(printer, tree, "item", false);
            }
            else {
				const char *expstr = Pool::instance()->printf("%s item", tname);
                print_base_var_unserial(printer, tree, expstr);
            }
            printer.s("%s.Add(item)", name);
        }
        printer.end();
    }
    printer.end();
}

void ProtobufLang::print_array_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
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

void ProtobufLang::print_var_serial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    const char *name;
    if (prefix) {
        name = Pool::instance()->printf("this.%s.%s", prefix, tree->name()->text());
    }
    else {
        name = Pool::instance()->printf("this.%s", tree->name()->text());
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

void ProtobufLang::print_var_unserial(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    const char *name;
    if (prefix) {
        name = Pool::instance()->printf("%s.%s", prefix, tree->name()->text());
    }
    else {
        name = Pool::instance()->printf("this.%s", tree->name()->text());
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

void ProtobufLang::print_var_dump(ProtobufPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    const char *name = tree->name()->text();
    print_indent(printer);
    printer.s("SSCC_PRINT(\"%s = \")", name);
    if (prefix) {
        name = Pool::instance()->printf("this.%s.%s", prefix, name);
    }
    else {
        name = Pool::instance()->printf("this.%s", name);
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

void ProtobufLang::print_serial(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.function_("public void Serial(ByteBuffer buf)"); {
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

void ProtobufLang::print_unserial(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.function_("public void Unserial(ByteBuffer buf)"); {
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

void ProtobufLang::print_constructor(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree) {
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
                printer.print("  %s()", item->name()->text(), type_decl(item));
                break;
            }
            if (item->type()->type() == TYPE_STRUCT) {
                printer.print("  %s()", item->name()->text());
                break;
            }
            if (item->type()->type() == TYPE_STRING) {
                printer.print("  %s()", item->name()->text(), type_decl(item));
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

void ProtobufLang::print_indent(ProtobufPrinter &printer) {
    printer.s("SSCC_PRINT_INDENT(sscc_indent)");
}

void ProtobufLang::print_dump(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree) {
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

void ProtobufLang::print_struct(ProtobufPrinter &printer, std::shared_ptr<StructTree> tree, bool useClass) {
    const char *inherited = tree->inherited() ? tree->inherited()->name()->text() : nullptr;

	if (useClass || tree->message || tree->size() > 2) {
		useClass = true;
		printer.class_(tree->name()->text(), inherited);
	} 
	else {
		printer.struct_(tree->name()->text(), inherited);
	}
	{
		int i = 0;
        for (auto sym : *tree) {
			i++;
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var(printer, std::dynamic_pointer_cast<StructItemTree>(sym), i);
                break;
            case TREE_CLASS_ITEM:
                print_var(printer, std::dynamic_pointer_cast<StructItemTree>(sym), i);
                break;
            default:
                assert(0);
                break;
            }
        }
    }
    printer.end();
}

void ProtobufLang::print_message(ProtobufPrinter &printer, std::shared_ptr<MessageTree> tree) {
    print_struct(printer, tree->req());
    if (tree->rsp()) {
        print_struct(printer, tree->rsp());
    }
}

void ProtobufLang::print_include(ProtobufPrinter &printer, std::shared_ptr<IncludeTree> tree) {
    //printer.p("import \"%s\";", (tree->path()->directory() + tree->path()->basename()).c_str());
}

void ProtobufLang::print(SymbolTable &symbols, FILE *file) {
    ProtobufPrinter printer;
    printer.open(file);

	printer.p("syntax = \"proto3\";");
    printer.p(head().str().c_str());
	printer.p("package pbgen;");
	printer.println("");

    for (auto sym : symbols) {
        switch (sym->type()) {
        case TREE_DEFINE:
            print_define(printer, std::dynamic_pointer_cast<DefineTree>(sym));
            break;
        case TREE_STRUCT:
            print_struct(printer, std::dynamic_pointer_cast<StructTree>(sym));
            break;
        case TREE_CLASS:
            print_struct(printer, std::dynamic_pointer_cast<StructTree>(sym), true);
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
