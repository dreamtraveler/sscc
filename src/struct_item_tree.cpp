#include "struct_item_tree.h"
#include "log.h"

void StructItemTree::parse(Parser *parser) {
	_type->parse(parser);

	if (parser->cur()->type() == '<') {
		parser->eat();
		_range->parse(parser);
		if (parser->cur()->type() != '>') {
			log_expect(parser->cur()->loc(), "'>'");
		}
		parser->eat();
	}

	_name = parser->cur();
	if (!_name->is_iden()) {
		log_expect(_name->loc(), "identifier");
	}
	parser->eat();

	if (parser->cur()->type() == '[') {
		parser->eat();
		_array = std::make_shared<RangeTree>();
		_array->parse(parser);

		if (parser->cur()->type() != ']') {
			log_expect(parser->cur()->loc(), "']'");
		}
		parser->eat();
	}

	if (parser->cur()->type() == '=') {
		parser->eat();
		_default = std::make_shared<ExprTree>();
		_default->parse(parser);
	}

	if (parser->cur()->type() != ';') {
		log_expect(parser->cur()->loc(), "';'");
	}
	parser->eat();
}



