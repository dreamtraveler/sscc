#include "csharp_lang.h"
#include <inttypes.h>
#include "pool.h"

CsharpLang::CsharpLang() : Language("csharp") {
}

void CsharpLang::option(const char *name, std::shared_ptr<Token> value) {

}

const char *CsharpLang::type_decl(std::shared_ptr<TypeTree> tree, bool use_class) {
    switch (tree->type()) {
    case TYPE_INT8:
        return "sbyte";
    case TYPE_UINT8:
        return "byte";
    case TYPE_INT16:
        return "short";
    case TYPE_UINT16:
        return "ushort";
    case TYPE_INT32:
        return "int";
    case TYPE_UINT32:
        return "uint";
    case TYPE_INT64:
        return "long";
    case TYPE_UINT64:
        return "ulong";
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

const char *CsharpLang::type_default_value(std::shared_ptr<TypeTree> tree) {
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

const char *CsharpLang::type_decl(std::shared_ptr<StructItemTree> tree, bool union_item) {
    const char *type;
    if (tree->array()) {
        type = type_decl(tree->type(), false);
		if (tree->type()->type() == TYPE_UINT8 || tree->type()->type() == TYPE_INT8) {
			type = Pool::instance()->printf("%s[]", type);
		}
		else {
			type = Pool::instance()->printf("List<%s>", type);
		}
    }
    else {
        type = type_decl(tree->type(), union_item);
    }
    return type;
}

void CsharpLang::print_define(CsharpPrinter &printer, std::shared_ptr<DefineTree> tree) {
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

void CsharpLang::print_var(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, bool use_class) {
	if (use_class) {
		const char *rightres;
		if (tree->array()) {
			if (tree->type()->type() == TYPE_UINT8 || tree->type()->type() == TYPE_INT8) {
				rightres = "null";
			}
			else {
				rightres = Pool::instance()->printf("new %s()", type_decl(tree, false));
			}
		}
		else {
			rightres = Pool::instance()->printf("%s", type_default_value(tree->type()));
		}
		printer.s("public %s %s = %s", type_decl(tree, use_class), tree->name()->text(), rightres);
	}
	else {
		printer.s("public %s %s", type_decl(tree, use_class), tree->name()->text());
	}
}

void CsharpLang::print_base_var_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
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

void CsharpLang::print_base_var_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
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

void CsharpLang::print_base_var_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
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

void CsharpLang::print_struct_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.s("%s.Serial(buf)", name);
    }
    else {
        printer.s("%s.Serial(buf)", name);
    }
}

void CsharpLang::print_struct_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.s("s.Unserial(buf)", name);
    }
    else {
        printer.s("%s.Unserial(buf)", name);
    }
}

void CsharpLang::print_struct_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
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

void CsharpLang::print_array_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
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
        tname = type_decl(tree->type(), false);
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

void CsharpLang::print_array_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
	if (tree->type()->type() == TYPE_UINT8 || tree->type()->type() == TYPE_INT8) {
		printer.s("%s = buf.ReadBytes()", name);
		return;
	}

	const char* tname;
	if (tree->type()->type() == TYPE_STRUCT) {
		tname = tree->type()->decl()->name()->text();
	}
	else {
        tname = type_decl(tree->type(), false);
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

void CsharpLang::print_array_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
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

void CsharpLang::print_var_serial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
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

void CsharpLang::print_var_unserial(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
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

void CsharpLang::print_var_dump(CsharpPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
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

void CsharpLang::print_serial(CsharpPrinter &printer, std::shared_ptr<StructTree> tree) {
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

void CsharpLang::print_unserial(CsharpPrinter &printer, std::shared_ptr<StructTree> tree) {
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

void CsharpLang::print_constructor(CsharpPrinter &printer, std::shared_ptr<StructTree> tree) {
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

void CsharpLang::print_indent(CsharpPrinter &printer) {
    printer.s("SSCC_PRINT_INDENT(sscc_indent)");
}

void CsharpLang::print_dump(CsharpPrinter &printer, std::shared_ptr<StructTree> tree) {
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

void CsharpLang::print_struct(CsharpPrinter &printer, std::shared_ptr<StructTree> tree) {
    const char *inherited = tree->inherited() ? tree->inherited()->name()->text() : nullptr;

	bool useClass = false;
	if (tree->message || tree->size() > 4) {
		useClass = true;
		printer.class_(tree->name()->text(), inherited);
	} 
	else {
		printer.struct_(tree->name()->text(), inherited);
	}
	{
        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var(printer, std::dynamic_pointer_cast<StructItemTree>(sym), useClass);
                break;
            default:
                assert(0);
                break;
            }
        }

        //printer.println("");
        //print_constructor(printer, tree);
        printer.println("");
        if (tree->message) {
			printer.function_("public ushort MsgId()"); {
				printer.s("return Message.%s", tree->message->id()->name()->text());
			}
			printer.end();
		}
		else {
			printer.function_("public ushort MsgId()"); {
				printer.s("return 0");
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

void CsharpLang::print_message(CsharpPrinter &printer, std::shared_ptr<MessageTree> tree) {
    print_struct(printer, tree->req());
    if (tree->rsp()) {
        print_struct(printer, tree->rsp());
    }

    printer.class_(tree->name()->text(), "Servlet"); {
		printer.s("public %s req = new %s()", tree->req()->name()->text(), tree->req()->name()->text());
		if (tree->rsp()) {
			printer.s("public %s rsp = new %s()", tree->rsp()->name()->text(), tree->rsp()->name()->text());
			printer.s("private Action<%s> cb", tree->name()->text());
		}

		printer.function_("public %s(bool needRpc = false)", tree->name()->text()); {
			printer.s("this.needRpc = needRpc");
		}
		printer.end();

		printer.function_("public override ushort MsgId()"); {
			printer.s("return Message.%s", tree->id()->name()->text());
		}
		printer.end();

		printer.function_("public override ISerial Req()"); {
			printer.s("return req");
		}
		printer.end();

		printer.function_("public override void SerialRequest(ByteBuffer buf)"); {
			printer.s("req.Serial(buf)");
		}
		printer.end();

		printer.function_("public override void UnserialRequest(ByteBuffer buf)"); {
			printer.s("req.Unserial(buf)");
		}
		printer.end();

		if (tree->rsp()){
			printer.function_("public override ISerial Rsp()"); {
				printer.s("return rsp");
			}
			printer.end();

			printer.function_("public int Response(Conn conn)"); {
				printer.s("conn.Send(rsp, seq)");
				printer.s("return 0");
			}
			printer.end();
		} else {
			printer.function_("public override ISerial Rsp()"); {
				printer.s("return null");
			}
			printer.end();
		}

		printer.function_("public override void SerialResponse(ByteBuffer buf)"); {
			if (tree->rsp()) {
				printer.s("rsp.Serial(buf)");
			}
		}
		printer.end();

		printer.function_("public override void UnserialResponse(ByteBuffer buf)"); {
			if (tree->rsp()) {
				printer.s("rsp.Unserial(buf)");
			}
		}
		printer.end();

		printer.function_("public override int Execute(Conn conn)"); {
			printer.s("return 0");
		}
		printer.end();

		printer.function_("public override int OnBack()"); {
			if (tree->rsp()) {
				printer.if_("cb != null"); {
					printer.s("cb(this)");
				}
				printer.end();
			}
			printer.s("return 0");
		}
		printer.end();

		if (tree->rsp()) {
			printer.function_("public Action<%s> Cb", tree->name()->text()); {
				printer.function_("set"); {
					printer.s("cb = value");
				}
				printer.end();
			}
			printer.end();
		}
    }
    printer.end();
}

void CsharpLang::print_include(CsharpPrinter &printer, std::shared_ptr<IncludeTree> tree) {
    //printer.p("#include \"%s.h\"", (tree->path()->directory() + tree->path()->basename()).c_str());
}

void CsharpLang::print(SymbolTable &symbols, FILE *file) {
    CsharpPrinter printer;
    printer.open(file);

    printer.p(head().str().c_str());
	printer.function_("namespace Tnet");

	if (this->filename == "Message" || this->filename == "G_Defines") {
		printer.function_("public class %s", this->filename.c_str());
	}

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
	if (this->filename == "Message" || this->filename == "G_Defines") {
		printer.end();
	}
	printer.end();
    printer.p(tail().str().c_str());
} 
