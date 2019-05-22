#include <cassert>
#include "protobuf_printer.h"

void ProtobufPrinter::s(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprint(fmt, ap);
	println(";");
}

void ProtobufPrinter::p(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	unsigned old = indent;
	indent = 0;
	vprint(fmt, ap);
	println("");
	indent = old;
}

void ProtobufPrinter::d(const char *name, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	unsigned old = indent;
	print("public const int %s = ", name);
	vprint(fmt, ap);
	println(";");
}

void ProtobufPrinter::if_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	print("if (");
	vprint(fmt, ap);
	println(") {");

	_blocks.push(BLOCK_IF);
	++indent;
}

void ProtobufPrinter::else_() {
	assert(_blocks.top() == BLOCK_IF);
	end();
	println("else {");
	_blocks.push(BLOCK_ELSE);
	++indent;
}

void ProtobufPrinter::else_if_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	assert(_blocks.top() == BLOCK_IF);
	end();
	print("else if (");
	vprint(fmt, ap);
	println(") {");
	_blocks.push(BLOCK_IF);
	++indent;
}

void ProtobufPrinter::for_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	print("for (");
	vprint(fmt, ap);
	println(") {");
	_blocks.push(BLOCK_FOR);
	++indent;
}

void ProtobufPrinter::foreach_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	print("foreach (");
	vprint(fmt, ap);
	println(") {");
	_blocks.push(BLOCK_FOR);
	++indent;
}

void ProtobufPrinter::while_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	print("while (");
	vprint(fmt, ap);
	println(") {");
	_blocks.push(BLOCK_WHILE);
	++indent;
}

void ProtobufPrinter::do_() {
	println("do {");
	_blocks.push(BLOCK_DO);
	++indent;
}

void ProtobufPrinter::function_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	vprint(fmt, ap);
	println(" {");
	_blocks.push(BLOCK_FUNC);
	++indent;
}

void ProtobufPrinter::struct_(const char *name, const char *inherited) {
	println("message %s {", name);
	_blocks.push(BLOCK_STRUCT);
	++indent;
}

void ProtobufPrinter::class_(const char *name, const char *inherited) {
	println("message %s {", name);
	_blocks.push(BLOCK_CLASS);
	++indent;
}

void ProtobufPrinter::enum_() {
	println("enum {");
	_blocks.push(BLOCK_ENUM);
	++indent;
}

void ProtobufPrinter::end() {
	assert(_blocks.size() > 0);
	--indent;
	switch (_blocks.top()) {
	case BLOCK_IF:
	case BLOCK_ELSE:
	case BLOCK_FOR:
	case BLOCK_WHILE:
	case BLOCK_FUNC:
		println("}");
		break;
	case BLOCK_STRUCT:
	case BLOCK_UNION:
	case BLOCK_CLASS:
	case BLOCK_ENUM:
		println("}");
		break;
	case BLOCK_DO:
		println("} while (false);");
		break;
	default:
		assert(0);
	}
	_blocks.pop();
}

void ProtobufPrinter::end(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	assert(_blocks.size() > 0);
	assert(_blocks.top() == BLOCK_DO);

	--indent;
	print("} while (");
	vprint(fmt, ap);
	println(");");
	_blocks.pop();
}
