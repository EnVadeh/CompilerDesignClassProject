#include "firstfollow.h"
#include <iostream>
#include <algorithm>

PARSING_SOURCE::PARSING_SOURCE(char* source) {
    this->source = source;
    i = 0;
    src_size = strlen(source);
    c = source[0];
    PARSE_GRAMMAR();
}


void PARSING_SOURCE::GRAMMAR_NEXT() {
    if (i < src_size) {
        i++;
        c = source[i];
    }
}

void PARSING_SOURCE::SKIP_SPACE() {
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        GRAMMAR_NEXT();
    }
}

void PARSING_SOURCE::PARSE_GRAMMAR() {
    while (c != '\0') {
        SKIP_SPACE();
        char non_terminal = c;
        GRAMMAR_NEXT();
        SKIP_SPACE();
        
        if (c == '-' && source[i+1] == '>') {
            GRAMMAR_NEXT();
            GRAMMAR_NEXT();
            SKIP_SPACE();
            
            std::string production;
            while (c != '\0' && c != '\n') {
                production += c;
                GRAMMAR_NEXT();
            }
            
            ADD_RULE(non_terminal, production);
        }
    }
}

void PARSING_SOURCE::ADD_RULE(char non_terminal, const std::string& production) {
    grammar_rules[non_terminal].push_back(production); //just what the map expects, a key value
}

void PARSING_SOURCE::PRINT_GRAMMAR() {
    for (const auto& rule : grammar_rules) {
        std::cout << rule.first << " -> ";
        for (size_t i = 0; i < rule.second.size(); ++i) {
            std::cout << rule.second[i];
            if (i < rule.second.size() - 1) {
                std::cout << " | ";
            }
        }
        std::cout << std::endl;
    }
}

void PARSING_SOURCE::COMPUTE_FIRST_SETS() {
    bool changed;
    do {
        changed = false;
        for (const auto& rule : grammar_rules) {
            char non_terminal = rule.first;
            for (const auto& production : rule.second) {
                std::set<char> first = FIRST(production);
                size_t original_size = first_sets[non_terminal].size();
                first_sets[non_terminal].insert(first.begin(), first.end());
                if (first_sets[non_terminal].size() > original_size) {
                    changed = true;
                }
            }
        }
    } while (changed);
}

std::set<char> PARSING_SOURCE::FIRST(const std::string& s) {
    std::set<char> result;
    if (s.empty()) {
        result.insert('#');  // # represents ε (epsilon)
        return result;
    }
    if (islower(s[0]) || s[0] == '#') {
        result.insert(s[0]);
        return result;
    }
    bool all_have_epsilon = true;
    for (char symbol : s) {
        if (islower(symbol) || symbol == '#') {
            result.insert(symbol);
            break;
        }
        std::set<char> first = first_sets[symbol];
        result.insert(first.begin(), first.end());
        if (first.find('#') == first.end()) {
            all_have_epsilon = false;
            break;
        }
        result.erase('#');
    }
    if (all_have_epsilon) {
        result.insert('#');
    }
    return result;
}

void PARSING_SOURCE::COMPUTE_FOLLOW_SETS() {
    follow_sets[grammar_rules.begin()->first].insert('$');  // $ represents end of input

    bool changed;
    do {
        changed = false;
        for (const auto& rule : grammar_rules) {
            char non_terminal = rule.first;
            for (const auto& production : rule.second) {
                for (size_t i = 0; i < production.size(); ++i) {
                    if (isupper(production[i])) {
                        std::set<char> first_of_beta = FIRST(production.substr(i + 1));
                        size_t original_size = follow_sets[production[i]].size();
                        follow_sets[production[i]].insert(first_of_beta.begin(), first_of_beta.end());
                        follow_sets[production[i]].erase('#');
                        if (first_of_beta.find('#') != first_of_beta.end() || i == production.size() - 1) {
                            follow_sets[production[i]].insert(follow_sets[non_terminal].begin(), follow_sets[non_terminal].end());
                        }
                        if (follow_sets[production[i]].size() > original_size) {
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
}

void PARSING_SOURCE::PRINT_FIRST_SETS() {
    std::cout << "FIRST sets:" << std::endl;
    for (const auto& set : first_sets) {
        std::cout << set.first << ": { ";
        for (char c : set.second) {
            std::cout << c << " ";
        }
        std::cout << "}" << std::endl;
    }
}

void PARSING_SOURCE::PRINT_FOLLOW_SETS() {
    std::cout << "FOLLOW sets:" << std::endl;
    for (const auto& set : follow_sets) {
        std::cout << set.first << ": { ";
        for (char c : set.second) {
            std::cout << c << " ";
        }
        std::cout << "}" << std::endl;
    }
}

int main() {
    std::string test = "S->AB\nS->bC\nA->b\nA->#\nB->aD\nC->AD\nD->aS\nD->c";
    char* source = &test[0];
    PARSING_SOURCE ps(source);
    
    std::cout << "Grammar:" << std::endl;
    ps.PRINT_GRAMMAR();
    
    ps.COMPUTE_FIRST_SETS();
    ps.PRINT_FIRST_SETS();
    
    ps.COMPUTE_FOLLOW_SETS(); //Both printed in ascending order
    ps.PRINT_FOLLOW_SETS();//because defauly property of map, it's more of a hassel to just chyange the order

    return 0;
}