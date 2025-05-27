#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

struct Node {
    unsigned char byte;
    int freq;
    Node* left;
    Node* right;
    Node(unsigned char b, int f, Node* l = nullptr, Node* r = nullptr);
};

struct Compare {
    bool operator()(Node* a, Node* b);
};

class Huffman {
public:
    void compress(const std::string& inputFile, const std::string& outputFile, bool verbose = false);
    void decompress(const std::string& inputFile, const std::string& outputFile, bool verbose = false);

private:
    void buildFrequencyTable(const std::vector<unsigned char>& data);
    void printFrequencyTable() const;

    void buildHuffmanTree();
    void printHuffmanTree() const;

    void buildCodes(Node* node, const std::string& code);
    void printCodes() const;

    void deleteTree(Node* node);
    std::string encode(const std::vector<unsigned char>& data);
    std::vector<unsigned char> decode(const std::string& bitString);

    std::unordered_map<unsigned char, int> freqTable;
    std::unordered_map<unsigned char, std::string> codes;
    std::unordered_map<std::string, unsigned char> reverseCodes;
    Node* root = nullptr;
};

#endif
