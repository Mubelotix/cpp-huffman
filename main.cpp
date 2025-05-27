#include "huffman.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage:\n"
                  << "  To compress:   " << argv[0] << " c input.txt output.huf\n"
                  << "  To decompress: " << argv[0] << " d input.huf output.txt\n";
        return 1;
    }

    Huffman huff;
    std::string mode = argv[1];
    if (mode == "c") {
        huff.compress(argv[2], argv[3]);
        std::cout << "File compressed.\n";
    } else if (mode == "d") {
        huff.decompress(argv[2], argv[3]);
        std::cout << "File decompressed.\n";
    } else {
        std::cerr << "Invalid mode. Use 'c' for compress, 'd' for decompress.\n";
        return 1;
    }

    return 0;
}
