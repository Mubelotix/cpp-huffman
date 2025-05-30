#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

struct Node {
    unsigned char byte;
    int freq;
    Node* left;
    Node* right;
    int id;
    Node(unsigned char b, int f, int id, Node* l = nullptr, Node* r = nullptr);
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
    void buildHuffmanTree();
    void buildCodes(Node* node, const std::string& code);
    
    void printFrequencyTable() const;
    void printHuffmanTree() const;
    void printCodes() const;

    void deleteTree(Node* node);
    std::string encode(const std::vector<unsigned char>& data);
    std::vector<unsigned char> decode(const std::string& bitString);

    std::map<unsigned char, int> freqTable;
    std::unordered_map<unsigned char, std::string> codes;
    std::unordered_map<std::string, unsigned char> reverseCodes;
    Node* root = nullptr;
};

#endif
