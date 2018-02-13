#include "message_tree.h"
#include "pool.h"
#include "log.h"

void MessageTree::parse(Parser *parser) {
    static auto request_base = std::make_shared<StructTree>(std::make_shared<IdenToken>(parser->input(), Unistr::get("ISerial")));
    static auto response_base = std::make_shared<StructTree>(std::make_shared<IdenToken>(parser->input(), Unistr::get("ISerial")));

    _name = parser->cur();
    if (!_name->is_iden()) {
        log_expect(_name->loc(), "identifier");
    }
    parser->eat();

    if (parser->cur()->type() != '<') {
        log_expect(parser->cur()->loc(), "'<'");
    }
    parser->eat();

    std::shared_ptr<Token> id_token = parser->cur();
    if (!id_token->is_iden()) {
        log_expect(_name->loc(), "identifier");
    }
    parser->eat();

    std::shared_ptr<Tree> id_tree = parser->symbols().get(id_token->text());
    if (!id_tree) {
        log_error(id_token->loc(), "unknown id name '%s'", id_token->text());
    }
    if (id_tree->type() != TREE_DEFINE) {
        log_error(id_token->loc(), "id '%s' is not a define", id_token->text());
    }
    _id = std::dynamic_pointer_cast<DefineTree>(id_tree);
    
    if (parser->cur()->type() != '>') {
        log_expect(parser->cur()->loc(), "'>'");
    }
    parser->eat();

    if (parser->cur()->type() != '{') {
        log_expect(parser->cur()->loc(), "'{'");
    }
    parser->eat();

    while (1) {
        std::shared_ptr<Token> token = parser->cur();
        if (parser->cur()->type() == '}') {
            parser->eat();
            break;
        }

        if (token->type() != TOKEN_STRUCT) {
            log_expect(token->loc(), "struct");
        }
        parser->eat();
    
        token = parser->cur();
        parser->eat();
        switch (token->type()) {
        case TOKEN_REQUEST:
            if (_req) {
                log_error(token->loc(), "dup declare request");
            }
            _req = std::make_shared<StructTree>();
            _req->parseInherited(parser, request_base);
            token->text(Unistr::get(Pool::instance()->printf(the_request_name, _name->text())));
            if (_req != parser->symbols().probe(token->text(), _req, false)) {
                log_error(token->loc(), "dup struct name");
            }
            _req->name(token);
            _req->message = this;
            break;
        case TOKEN_RESPONSE:
            if (_rsp) {
                log_error(token->loc(), "dup declare response");
            }
            _rsp = std::make_shared<StructTree>();
            _rsp->parseInherited(parser, response_base);
            token->text(Unistr::get(Pool::instance()->printf(the_response_name, _name->text())));
            if (_rsp != parser->symbols().probe(token->text(), _rsp, false)) {
                log_error(token->loc(), "dup struct name");
            }
            _rsp->name(token);
            _rsp->message = this;
            break;
        default:
            log_expect(token->loc(), "request or response");
        }
    }

    if (!_req) {
        log_error(id_token->loc(), "message must declare request");
    }


    if (parser->cur()->type() != ';') {
        log_expect(parser->cur()->loc(), "';'");
    }
    parser->eat();
}



