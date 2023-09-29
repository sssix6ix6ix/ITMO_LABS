#include "lib/parser.h"

int main(int argc, char** argv) {
    Parser parser;
    parser.Parsing(argc, argv);

    SandPile sandPile;
    sandPile.init(parser.height, parser.width);

    BMPImage image;
    sandPile.SandPileSimulation(image, parser.input, parser.output, parser.max_iter, parser.freq);

    return 0;
}
