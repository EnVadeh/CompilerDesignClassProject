#include "firstfollow.h"
#include <iostream>
#include <algorithm>
#include <stack>

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
    std::string trimmed_production = production;
    trimmed_production.erase(0, trimmed_production.find_first_not_of(" \t\n\r"));
    trimmed_production.erase(trimmed_production.find_last_not_of(" \t\n\r") + 1);
    grammar_rules[non_terminal].push_back(trimmed_production);
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

void PARSING_SOURCE::CREATE_PARSING_TABLE() {
    std::cout << "Creating parsing table..." << std::endl;
    for (const auto& rule : grammar_rules) {
        char non_terminal = rule.first;
        for (const auto& production : rule.second) {
            std::set<char> first = FIRST(production);
            std::cout << "FIRST(" << production << ") = { ";
            for (char c : first) std::cout << c << " ";
            std::cout << "}" << std::endl;
            
            for (char terminal : first) {
                if (terminal != '#' && terminal != ' ') {
                    parsing_table[{non_terminal, terminal}] = production;
                    std::cout << "Added to table: " << non_terminal << ", " << terminal << " -> " << production << std::endl;
                }
            }
            if (first.find('#') != first.end() || first.find(' ') != first.end()) {
                std::set<char> follow = follow_sets[non_terminal];
                std::cout << "FOLLOW(" << non_terminal << ") = { ";
                for (char c : follow) std::cout << c << " ";
                std::cout << "}" << std::endl;
                
                for (char terminal : follow) {
                    if (terminal != ' ') {
                        parsing_table[{non_terminal, terminal}] = production;
                        std::cout << "Added to table: " << non_terminal << ", " << terminal << " -> " << production << std::endl;
                    }
                }
            }
        }
    }
}

void PARSING_SOURCE::PRINT_PARSING_TABLE() {
    std::cout << "LL(1) Parsing Table:" << std::endl;
    for (const auto& entry : parsing_table) {
        std::cout << entry.first.first << ", " << entry.first.second << " -> " << entry.second << std::endl;
    }
}

bool PARSING_SOURCE::SIMULATE_LL1_PARSING(const std::string& input) {
    std::stack<char> stack;
    stack.push('$');
    stack.push('S');  // Start symbol
    size_t input_pos = 0;

    std::cout << "Simulating LL(1) parsing for input: " << input << std::endl;

    while (!stack.empty()) {
        char top = stack.top();
        char current_input = (input_pos < input.length()) ? input[input_pos] : '$';

        std::cout << "Stack: ";
        std::stack<char> temp_stack = stack;
        std::string stack_content;
        while (!temp_stack.empty()) {
            stack_content = temp_stack.top() + stack_content;
            temp_stack.pop();
        }
        std::cout << stack_content;
        std::cout << "\tInput: " << input.substr(input_pos) << "$" << std::endl;

        if (top == '$' && current_input == '$') {
            std::cout << "Parsing completed successfully" << std::endl;
            return true;
        }

        if (isupper(top)) {  // Non-terminal
            auto it = parsing_table.find({top, current_input});
            if (it == parsing_table.end()) {
                std::cout << "No production rule found for " << top << " and " << current_input << std::endl;
                return false;  
            }
            std::string production = it->second;
            std::cout << "Applied rule: " << top << " -> " << production << std::endl;
            stack.pop();
            if (production != "#") {  // '#' represents ε (epsilon)
                for (int i = production.length() - 1; i >= 0; i--) {
                    if (production[i] != ' ') {  // for some reason there was empty string? And it kept asking for an empty string after each match
                        stack.push(production[i]);
                    }
                }
            }
        } else if (top == current_input) {
            std::cout << "Matched " << top << std::endl;
            stack.pop();
            input_pos++;
        } else if (top == ' ') {  // same reason as above, just in case, even though removing this doesn't seem to cause any problems 
            stack.pop();
        } else {
            std::cout << "Mismatch: expected '" << top << "', got '" << current_input << "'" << std::endl;
            return false;  
        }
    }

    if (input_pos < input.length()) {
        std::cout << "Parsing failed: input not fully consumed" << std::endl;
        return false;
    }

    std::cout << "Parsing completed successfully" << std::endl;
    return true;
}

int main() {
    std::string test = "S -> AB \n A -> aA \n A-> b \n B -> CA \n C -> cC \n C-> #";
    char* source = &test[0];
    PARSING_SOURCE ps(source);
    
    std::cout << "Grammar:" << std::endl;
    ps.PRINT_GRAMMAR();
    
    ps.COMPUTE_FIRST_SETS();
    ps.PRINT_FIRST_SETS();
    
    ps.COMPUTE_FOLLOW_SETS(); //Both printed in ascending order
    ps.PRINT_FOLLOW_SETS();//because defauly property of map, it's more of a hassel to just chyange the order

    ps.CREATE_PARSING_TABLE();
    ps.PRINT_PARSING_TABLE();

    std::string input = "bb";
    
    bool result = ps.SIMULATE_LL1_PARSING(input);
    std::cout<<"The string was ";
    if(result)
    std::cout<<"parsed successfully";
    else
    std::cout<<"not parsed successfully";

    return 0;
}