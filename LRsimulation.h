#pragma once
#ifndef LRSIMULATION_H
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

struct Item {
    char left;
    std::string right;
    int dot_position;

    bool operator<(const Item& other) const {
        if (left != other.left) return left < other.left;
        if (right != other.right) return right < other.right;
        return dot_position < other.dot_position;
    }

    bool operator==(const Item& other) const {
        return left == other.left && right == other.right && dot_position == other.dot_position;
    }
};


class LRParser {
private:
    std::map<char, std::vector<std::string>> grammar;
    std::vector<std::set<Item>> canonical_collection;
    std::vector<std::map<char, int>> goto_table;
    std::vector<std::map<char, std::string>> action_table;

    void construct_canonical_collection();
    std::set<Item> closure(const std::set<Item>& I);
    std::set<Item> go_to(const std::set<Item>& I, char X);
    void construct_parsing_table();

public:
    LRParser(const std::map<char, std::vector<std::string>>& g);
    void print_items();
    void print_parsing_table();
    bool parse(const std::string& input);
};
#endif