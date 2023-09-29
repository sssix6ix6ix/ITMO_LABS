#include "lib\HAFfiles.h"

int main(int argc, char* argv[]) {
    Parser parser;
    parser.Parsing(argc, argv);
    DoAction(parser);

    return 0;
}
