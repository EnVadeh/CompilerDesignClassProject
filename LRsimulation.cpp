#include "LRsimulation.h"
#include <stack>
#include <algorithm>

//6. Write a program to simulate the parsing process of LR grammar. Take necessary measure to use parsing table.

void Parser::initializeTables() {
    actionTable[0][TokenType::NUMBER] = "S5";
    actionTable[0][TokenType::LPAREN] = "S4";
    actionTable[1][TokenType::PLUS] = "S6";
    actionTable[1][TokenType::END] = "ACC";
    actionTable[2][TokenType::PLUS] = "R2";
    actionTable[2][TokenType::MULTIPLY] = "S7";
    actionTable[2][TokenType::RPAREN] = "R2";
    actionTable[2][TokenType::END] = "R2";
    actionTable[3][TokenType::PLUS] = "R4";
    actionTable[3][TokenType::MULTIPLY] = "R4";
    actionTable[3][TokenType::RPAREN] = "R4";
    actionTable[3][TokenType::END] = "R4";
    actionTable[4][TokenType::NUMBER] = "S5";
    actionTable[4][TokenType::LPAREN] = "S4";
    actionTable[5][TokenType::PLUS] = "R6";
    actionTable[5][TokenType::MULTIPLY] = "R6";
    actionTable[5][TokenType::RPAREN] = "R6";
    actionTable[5][TokenType::END] = "R6";
    actionTable[6][TokenType::NUMBER] = "S5";
    actionTable[6][TokenType::LPAREN] = "S4";
    actionTable[7][TokenType::NUMBER] = "S5";
    actionTable[7][TokenType::LPAREN] = "S4";
    actionTable[8][TokenType::PLUS] = "S6";
    actionTable[8][TokenType::RPAREN] = "S11";
    actionTable[9][TokenType::PLUS] = "R1";
    actionTable[9][TokenType::MULTIPLY] = "S7";
    actionTable[9][TokenType::RPAREN] = "R1";
    actionTable[9][TokenType::END] = "R1";
    actionTable[10][TokenType::PLUS] = "R3";
    actionTable[10][TokenType::MULTIPLY] = "R3";
    actionTable[10][TokenType::RPAREN] = "R3";
    actionTable[10][TokenType::END] = "R3";
    actionTable[11][TokenType::PLUS] = "R5";
    actionTable[11][TokenType::MULTIPLY] = "R5";
    actionTable[11][TokenType::RPAREN] = "R5";
    actionTable[11][TokenType::END] = "R5";

    gotoTable[0]["E"] = 1;
    gotoTable[0]["T"] = 2;
    gotoTable[0]["F"] = 3;
    gotoTable[4]["E"] = 8;
    gotoTable[4]["T"] = 2;
    gotoTable[4]["F"] = 3;
    gotoTable[6]["T"] = 9;
    gotoTable[6]["F"] = 3;
    gotoTable[7]["F"] = 10;
}

void Parser::shift(int state) {
    stateStack.push(state);
    valueStack.push(tokens[currentToken].value);
    currentToken++;
}

void Parser::reduce(int rule) {
    std::string lhs;
    int popCount;

    switch (rule) {
        case 1: lhs = "E"; popCount = 3; break; // E -> E + T
        case 2: lhs = "E"; popCount = 1; break; // E -> T
        case 3: lhs = "T"; popCount = 3; break; // T -> T * F
        case 4: lhs = "T"; popCount = 1; break; // T -> F
        case 5: lhs = "F"; popCount = 3; break; // F -> ( E )
        case 6: lhs = "F"; popCount = 1; break; // F -> number
        default: throw std::runtime_error("Invalid rule");
    }

    for (int i = 0; i < popCount; i++) {
        stateStack.pop();
        valueStack.pop();
    }

    int state = stateStack.top();
    stateStack.push(gotoTable[state][lhs]);
    valueStack.push(lhs);
}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), currentToken(0) {
    initializeTables();
}

bool Parser::parse() {
    stateStack.push(0);

    while (true) {
        int state = stateStack.top();
        TokenType token = tokens[currentToken].type;

        if (actionTable[state].find(token) == actionTable[state].end()) {
            return false; // Error: No action defined
        }

        std::string action = actionTable[state][token];

        if (action[0] == 'S') {
            shift(std::stoi(action.substr(1)));
        } else if (action[0] == 'R') {
            reduce(std::stoi(action.substr(1)));
        } else if (action == "ACC") {
            return true; 
        } else {
            return false; 
        }
    }
}

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string current;

    for (char c : input) {
        if (std::isdigit(c)) {
            current += c;
        } else {
            if (!current.empty()) {
                tokens.push_back({TokenType::NUMBER, current});
                current.clear();
            }
            switch (c) {
                case '+': tokens.push_back({TokenType::PLUS, "+"}); break;
                case '*': tokens.push_back({TokenType::MULTIPLY, "*"}); break;
                case '(': tokens.push_back({TokenType::LPAREN, "("}); break;
                case ')': tokens.push_back({TokenType::RPAREN, ")"}); break;
                case ' ': break;
                default: throw std::runtime_error(std::string("Invalid character: ") + c);
            }
        }
    }

    if (!current.empty()) {
        tokens.push_back({TokenType::NUMBER, current});
    }

    tokens.push_back({TokenType::END, "$"});
    return tokens;
}

int main() {
    std::string input;
    std::cout << "Enter an arithmetic expression: ";
    std::getline(std::cin, input);

    try {
        std::vector<Token> tokens = tokenize(input);
        Parser parser(tokens);

        if (parser.parse()) {
            std::cout << "Parsing successful!" << std::endl;
        } else {
            std::cout << "Parsing failed." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}