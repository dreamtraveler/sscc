#include "js_lang.h"
#include <inttypes.h>
#include "pool.h"

JsLang::JsLang() : Language("js") {
}

void JsLang::option(const char *name, std::shared_ptr<Token> value) {

}

const char *JsLang::type_decl(std::shared_ptr<TypeTree> tree, bool union_item) {
    switch (tree->type()) {
    case TYPE_INT8:
    case TYPE_UINT8:
    case TYPE_INT16:
    case TYPE_UINT16:
    case TYPE_INT32:
    case TYPE_UINT32:
    case TYPE_INT64:
    case TYPE_UINT64:
    case TYPE_FLOAT:
    case TYPE_DOUBLE:
    case TYPE_STRING:
    case TYPE_STRUCT:
		return "var";
    default:
        assert(0);
        return nullptr;
    }
}

const char *JsLang::type_decl(std::shared_ptr<StructItemTree> tree, bool union_item) {
    return "var";
}

const char *JsLang::default_value(std::shared_ptr<TypeTree> tree) {
    switch (tree->type()) {
    case TYPE_INT8:
    case TYPE_UINT8:
    case TYPE_INT16:
    case TYPE_UINT16:
    case TYPE_INT32:
    case TYPE_UINT32:
    case TYPE_INT64:
    case TYPE_UINT64:
		return "0";
    case TYPE_FLOAT:
    case TYPE_DOUBLE:
		return "0.0";
    case TYPE_STRING:
		return "\"\"";
    case TYPE_STRUCT:
		return "null";
    default:
        assert(0);
        return nullptr;
    }
}

void JsLang::print_define(JsPrinter &printer, std::shared_ptr<DefineTree> tree) {
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

void JsLang::print_var(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, bool union_item) {
    printer.s("this.%s = %s", tree->name()->text(), default_value(tree->type()));
}

void JsLang::print_base_var_serial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "buf.writeInt8";
        break;
    case TYPE_UINT8:
        method = "buf.writeUInt8";
        break;
    case TYPE_INT16:
        method = "buf.writeInt16";
        break;
    case TYPE_UINT16:
        method = "buf.writeUInt16";
        break;
    case TYPE_INT32:
        method = "buf.writeInt32";
        break;
    case TYPE_UINT32:
        method = "buf.writeUInt32";
        break;
    case TYPE_INT64:
        method = "buf.writeInt64";
        break;
    case TYPE_UINT64:
        method = "buf.writeUInt64";
        break;
    case TYPE_FLOAT:
        method = "buf.writeFloat";
        break;
    case TYPE_DOUBLE:
        method = "buf.writeDouble";
        break;
    case TYPE_STRING:
        method = "buf.writeString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s(%s)", method, name);
}

void JsLang::print_base_var_unserial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "buf.readInt8";
        break;
    case TYPE_UINT8:
        method = "buf.readUInt8";
        break;
    case TYPE_INT16:
        method = "buf.readInt16";
        break;
    case TYPE_UINT16:
        method = "buf.readUInt16";
        break;
    case TYPE_INT32:
        method = "buf.readInt32";
        break;
    case TYPE_UINT32:
        method = "buf.readUInt32";
        break;
    case TYPE_INT64:
        method = "buf.readInt64";
        break;
    case TYPE_UINT64:
        method = "buf.readUInt64";
        break;
    case TYPE_FLOAT:
        method = "buf.readFloat";
        break;
    case TYPE_DOUBLE:
        method = "buf.readDouble";
        break;
    case TYPE_STRING:
        method = "buf.readString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s = %s()", name, method);
}

void JsLang::print_struct_serial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.s("%s->serial(buf)", name);
    }
    else {
        printer.s("%s.serial(buf)", name);
    }
}

void JsLang::print_struct_unserial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name, bool is_pointer) {
	const char* tname = tree->type()->decl()->name()->text();
	printer.s("%s = new %s()", name, tname);
	printer.s("%s.unserial(buf)", name);
}

void JsLang::print_array_serial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
    printer.s("buf.writeUInt32(%s.length)", name);

	const char* item = Pool::instance()->printf("%s[sscc_i]", name);
    printer.for_("var sscc_i = 0; sscc_i < %s.length; ++sscc_i", name); {
        if (tree->type()->type() == TYPE_STRUCT) {
            print_struct_serial(printer, tree, item, false);
        }
        else {
            print_base_var_serial(printer, tree, item);
        }
    }
    printer.end();
}

void JsLang::print_array_unserial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *name) {
	const char* tname;
	if (tree->type()->type() == TYPE_STRUCT) {
		tname = tree->type()->decl()->name()->text();
	}
	else {
		tname = type_decl(tree->type(), false);
	}

	const char* size_name = Pool::instance()->printf("sscc_%s_size", tree->name()->text());

	printer.s("%s = []", name);
	printer.s("var %s = buf.readUInt32()", size_name);

	printer.for_("var sscc_i = 0; sscc_i < %s; ++sscc_i", size_name); {
		if (tree->type()->type() == TYPE_STRUCT) {
			printer.s("var item = new %s()", tname);
			print_struct_unserial(printer, tree, "item", false);
		}
		else {
			const char *expstr = Pool::instance()->printf("%s item", tname);
			print_base_var_unserial(printer, tree, expstr);
		}
		printer.s("%s.push(item)", name);
	}
	printer.end();
}

void JsLang::print_var_serial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
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

void JsLang::print_var_unserial(JsPrinter &printer, std::shared_ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
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

void JsLang::print_serial(JsPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.function_("pb.%s.prototype.serial = function(buf)", tree->name()->text()); {
		printer.s("var pbbuf = Buffer.from(this.serializeBinary())");
		printer.s("buf.writeBytes(pbbuf)");
    }
    printer.end();
};

void JsLang::print_unserial(JsPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.function_("pb.%s.prototype.unserial = function(buf)", tree->name()->text()); {
		printer.s("return pb.%s.deserializeBinary(buf.bytes())", tree->name()->text());
    }
    printer.end();
};

void JsLang::print_constructor(JsPrinter &printer, std::shared_ptr<StructTree> tree) {
	printer.function_("function %s()", tree->name()->text()); {
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
	}
	printer.end();
    printer.println("");
};

void JsLang::print_indent(JsPrinter &printer) {
    printer.s("SSCC_PRINT_INDENT(sscc_indent)");
}

void JsLang::print_struct(JsPrinter &printer, std::shared_ptr<StructTree> tree) {
    printer.s("exports.%s=pb.%s", tree->name()->text(), tree->name()->text());
	printer.println("");

	//print_constructor(printer, tree);

	if (tree->message) {
		printer.function_("pb.%s.prototype.msgId = function()", tree->name()->text()); {
			printer.s("return Message.%s", tree->message->id()->name()->text());
		}
		printer.end();

		printer.println("");
		print_serial(printer, tree);
		printer.println("");
		print_unserial(printer, tree);
		printer.println("");
	}
}

void JsLang::print_message(JsPrinter &printer, std::shared_ptr<MessageTree> tree) {
    printer.s("exports.%s=%s", tree->name()->text(), tree->name()->text());

    print_struct(printer, tree->req());
    if (tree->rsp()) {
        print_struct(printer, tree->rsp());
    }

	printer.struct_(tree->name()->text(), nullptr); {
		printer.s("this.req = new pb.%s()", tree->req()->name()->text());
		if (tree->rsp()) {
			printer.s("this.rsp = new pb.%s()", tree->rsp()->name()->text());
		}
	}
	printer.end();
	printer.println("");

	printer.function_("%s.prototype.msgId = function()", tree->name()->text()); {
		printer.s("return Message.%s", tree->id()->name()->text());
	}
	printer.end();
	printer.println("");

	printer.function_("%s.prototype.onBack = function()", tree->name()->text()); {
		if (tree->rsp()) {
			printer.if_("this.cb != null"); {
				printer.s("this.cb(this)");
			}
			printer.end();
		}
		printer.s("return");
	}
	printer.end();
}

void JsLang::print_include(JsPrinter &printer, std::shared_ptr<IncludeTree> tree) {
    //printer.p("var %s=require(\"%s.js\")", (tree->path()->directory() + tree->path()->basename()).c_str(), (tree->path()->directory() + tree->path()->basename()).c_str());
}

void JsLang::print(SymbolTable &symbols, FILE *file) {
    JsPrinter printer;
    printer.open(file);

	printer.s("\"use strict\"");
    printer.p(head().str().c_str());

	int cnt = 0;
    for (auto sym : symbols) {
        switch (sym->type()) {
        case TREE_DEFINE:
            print_define(printer, std::dynamic_pointer_cast<DefineTree>(sym));
            break;
        case TREE_STRUCT:
        case TREE_CLASS:
			cnt++;
			if (cnt == 1) {
				printer.s("var pb = require(\'../../pbgen/%s_pb\')", this->filename().c_str());
			}
            print_struct(printer, std::dynamic_pointer_cast<StructTree>(sym));
            break;
        case TREE_MESSAGE:
			cnt++;
			if (cnt == 1) {
				printer.s("var pb = require(\'../../pbgen/%s_pb\')", this->filename().c_str());
			}
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