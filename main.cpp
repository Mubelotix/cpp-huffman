#include "huffman.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage:\n"
                  << "  To compress:   " << argv[0] << " c input.txt output.huf [verbose]\n"
                  << "  To decompress: " << argv[0] << " d input.huf output.txt [verbose]\n";
        return 1;
    }

    bool verbose = (argc == 5 && (std::string(argv[4]) == "verbose" || std::string(argv[4]) == "--verbose" || std::string(argv[4]) == "-v"));

    Huffman huff;
    std::string mode = argv[1];
    if (mode == "c") {
        huff.compress(argv[2], argv[3], verbose);
        std::cout << "File compressed.\n";
    } else if (mode == "d") {
        huff.decompress(argv[2], argv[3], verbose);
        std::cout << "File decompressed.\n";
    } else {
        std::cerr << "Invalid mode. Use 'c' for compress, 'd' for decompress.\n";
        return 1;
    }

    return 0;
}
