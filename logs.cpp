#include "huffman.hpp"
#include <fstream>
#include <bitset>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <algorithm>

void Huffman::printFrequencyTable() const {
    std::vector<std::pair<unsigned char, int>> sortedFreq(freqTable.begin(), freqTable.end());

    std::sort(sortedFreq.begin(), sortedFreq.end(),
            [](const auto& a, const auto& b) {
                if (a.second != b.second)
                    return a.second > b.second;  // frequency descending
                return a.first < b.first;        // tie-breaker: byte ascending
            });

    std::cout << "Byte Frequency Table (sorted by frequency):\n";
    for (const auto& [byte, freq] : sortedFreq) {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)byte
                  << " (" << std::dec << std::setw(3) << (int)byte << "): " << freq;
                  
        // Show printable ASCII characters if applicable
        if (std::isprint(byte)) {
            std::cout << " '" << byte << "'";
        }

        std::cout << '\n';
    }
}

void printNode(const Node* node, std::string indent = "", bool isLeft = true) {
    if (!node) return;

    std::cout << indent;

    // Branch prefix
    std::cout << (isLeft ? "├──" : "└──");

    if (!node->left && !node->right) {
        // Leaf node: print byte and frequency
        std::cout << " '" ;
        // Print printable characters as is, others as hex
        if (std::isprint(node->byte)) 
            std::cout << node->byte;
        else
            std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)node->byte << std::dec;

        std::cout << "' (" << node->freq << ")\n";
    } else {
        // Internal node
        std::cout << "* (" << node->freq << ")\n";
    }

    // Recurse for children with increased indent
    printNode(node->left, indent + (isLeft ? "│   " : "    "), true);
    printNode(node->right, indent + (isLeft ? "│   " : "    "), false);
}

void Huffman::printHuffmanTree() const {
    if (!root) {
        std::cout << "Huffman tree is empty.\n";
        return;
    }
    std::cout << "Huffman Tree:\n";
    printNode(root, "", false);
}

void Huffman::printCodes() const {
    if (codes.empty()) {
        std::cout << "No Huffman codes available. Build the tree first.\n";
        return;
    }

    // Copy map into vector
    std::vector<std::pair<unsigned char, std::string>> sortedCodes(codes.begin(), codes.end());

    // Sort by byte ascending
    std::sort(sortedCodes.begin(), sortedCodes.end(),
              [](const auto& a, const auto& b) {
                  return a.first < b.first;
              });

    std::cout << "Huffman Codes (sorted by byte ascending):\n";
    for (const auto& [byte, code] : sortedCodes) {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)byte
                  << " (" << std::dec << std::setw(3) << (int)byte << "): " << code;

        if (std::isprint(byte)) {
            std::cout << " '" << byte << "'";
        }

        std::cout << '\n';
    }
}
