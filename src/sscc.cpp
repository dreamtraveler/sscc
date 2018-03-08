
#include "input.h"
#include "parser.h"
#include "lex.h"
#include "cpp_lang.h"
#include "luavar_lang.h"
#include "go_lang.h"
#include "csharp_lang.h"
#include "js_lang.h"

void usage() {
     fprintf(stderr, "Usage: sscc -i input_file -o output_file -l language\n");
     exit(1);
}

int main(int argc, char *argv[])
{
    char *opt;
    const char *input_name = nullptr;
    const char *output_name = nullptr;
    const char *lang_name = nullptr;

    Input input;
	for (int i = 1; i < argc; i+=2) {
		opt = argv[i];
		if (strcmp(opt, "-i")==0) {
			input_name = argv[i + 1];
		}
		else if (strcmp(opt, "-o") == 0) {
			output_name = argv[i + 1];
		}
		else if (strcmp(opt, "-l") == 0) {
			lang_name = argv[i + 1];
		}
		else if (strcmp(opt, "-I") == 0) {
			input.addPath(std::make_shared<Path>(argv[i+1]));
		}
		else {
			usage();
		}
	}
    //while ((opt = getopt(argc, argv, "i:o:l:I:")) != -1) {
    //    switch (opt) {
    //    case 'i':
    //        input_name = optarg;
    //        break;
    //    case 'o':
    //        output_name = optarg;
    //        break;
    //    case 'l':
    //        lang_name = optarg;
    //        break;
    //    case 'I':
    //        input.addPath(object<Path>(optarg));
    //        break;
    //    default:
    //        usage();
    //    }
    //}

    if (!input_name || !output_name || !lang_name) {
        usage();
    }

    Token::init();
    Language::reg<CppLang>();
    Language::reg<LuaVarLang>();
    Language::reg<GoLang>();
	Language::reg<CsharpLang>();
	Language::reg<JsLang>();

    input.load(std::make_shared<Path>(input_name), false);

    SymbolTable symbols;

    Parser parser(input, symbols);
    parser.parse();

    Language *lang = Language::get(lang_name);
    if (!lang) {
        log_fail("unknown language name '%s'", lang_name);
    }

    FILE *output = fopen(output_name, "w");
    if (!output) {
        perror("open output file failed.");
        exit(1);
    }
	lang->set_filename(input_name);
    lang->print(symbols, output);
    fclose(output);
    return 0;
}

