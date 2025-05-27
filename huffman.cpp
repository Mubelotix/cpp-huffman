#include "huffman.hpp"
#include <fstream>
#include <bitset>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <algorithm>

Node::Node(unsigned char b, int f, Node* l, Node* r)
    : byte(b), freq(f), left(l), right(r) {}

bool Compare::operator()(Node* a, Node* b) {
    if (a->freq != b->freq)
        return a->freq > b->freq;
    return a->byte > b->byte;
}

void Huffman::buildFrequencyTable(const std::vector<unsigned char>& data) {
    freqTable.clear();
    for (unsigned char b : data) freqTable[b]++;
}

void Huffman::buildHuffmanTree() {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto& [b, f] : freqTable) {
        pq.push(new Node(b, f));
    }

    while (pq.size() > 1) {
        Node* l = pq.top(); pq.pop();
        Node* r = pq.top(); pq.pop();
        pq.push(new Node(0, l->freq + r->freq, l, r));
    }

    root = pq.top();
    buildCodes(root, "");
}

void Huffman::buildCodes(Node* node, const std::string& code) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->byte] = code;
        reverseCodes[code] = node->byte;
        return;
    }
    buildCodes(node->left, code + "0");
    buildCodes(node->right, code + "1");
}

std::string Huffman::encode(const std::vector<unsigned char>& data) {
    std::string encoded;
    for (unsigned char b : data)
        encoded += codes[b];
    return encoded;
}

std::vector<unsigned char> Huffman::decode(const std::string& bitString) {
    std::vector<unsigned char> result;
    std::string current;
    for (char bit : bitString) {
        current += bit;
        if (reverseCodes.count(current)) {
            result.push_back(reverseCodes[current]);
            current.clear();
        }
    }
    return result;
}

void Huffman::compress(const std::string& inputFile, const std::string& outputFile, bool verbose) {
    // Read binary input
    std::ifstream in(inputFile, std::ios::binary);
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    buildFrequencyTable(data);
    buildHuffmanTree();

    std::string encoded = encode(data);

    // Add padding to ensure multiple of 8
    int padding = (8 - encoded.size() % 8) % 8;
    encoded.append(padding, '0');

    // Write header
    std::ofstream out(outputFile, std::ios::binary);
    uint32_t tableSize = freqTable.size();
    out.write(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));
    for (const auto& [b, f] : freqTable) {
        out.put(static_cast<char>(b));
        out.write(reinterpret_cast<const char*>(&f), sizeof(f));
    }
    out.put(static_cast<char>(padding));

    if (verbose) {
        std::cout << "Padding size: " << padding << " bits\n";
        std::cout << "Table size: " << tableSize << " entries\n";
        printFrequencyTable();
        printHuffmanTree();
        printCodes();
    }

    // Write encoded binary data
    for (size_t i = 0; i < encoded.size(); i += 8) {
        std::bitset<8> byte(encoded.substr(i, 8));
        out.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    out.close();
    deleteTree(root);
}

void Huffman::decompress(const std::string& inputFile, const std::string& outputFile, bool verbose) {
    std::ifstream in(inputFile, std::ios::binary);

    // Read header
    uint32_t tableSize;
    in.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));
    freqTable.clear();

    for (uint32_t i = 0; i < tableSize; ++i) {
        unsigned char b = in.get();
        int freq;
        in.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        freqTable[b] = freq;
    }

    int padding = in.get();

    // Read encoded content
    std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(in)),
                                      std::istreambuf_iterator<char>());
    in.close();

    std::string bitString;
    for (unsigned char byte : bytes)
        bitString += std::bitset<8>(byte).to_string();

    if (padding > 0)
        bitString = bitString.substr(0, bitString.size() - padding);

    buildHuffmanTree();

    if (verbose) {
        std::cout << "Padding size: " << padding << " bits\n";
        std::cout << "Table size: " << tableSize << " entries\n";

        printFrequencyTable();
        printHuffmanTree();
        printCodes();
    }

    std::vector<unsigned char> decoded = decode(bitString);

    std::ofstream out(outputFile, std::ios::binary);
    out.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
    out.close();
    deleteTree(root);
}

void Huffman::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}
