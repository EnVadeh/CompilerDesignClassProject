#pragma once
#ifndef LRSIMULATION_H
#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <unordered_map>

enum class TokenType {
    NUMBER,
    PLUS,
    MULTIPLY,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    std::string value;
};

class Parser {
private:
    std::vector<Token> tokens;
    std::stack<int> stateStack;
    std::stack<std::string> valueStack;
    int currentToken;

    std::unordered_map<int, std::unordered_map<TokenType, std::string>> actionTable;
    std::unordered_map<int, std::unordered_map<std::string, int>> gotoTable;

    void initializeTables();
    void shift(int state);
    void reduce(int rule);

public:
    Parser(const std::vector<Token>& tokens);
    bool parse();
    void printParsingTable();
};

std::vector<Token> tokenize(const std::string& input);
#endif