#include "LRsimulation.h"
#include <stack>
#include <algorithm>

//6. Write a program to simulate the parsing process of LR grammar. Take necessary measure to use parsing table.


LRParser::LRParser(const std::map<char, std::vector<std::string>>& g) : grammar(g) {
    construct_canonical_collection();
    construct_parsing_table();
}

void LRParser::construct_canonical_collection() {
    std::set<Item> initial_item = {{grammar.begin()->first, grammar.begin()->second[0], 0}};
    canonical_collection.push_back(closure(initial_item));

    bool changed;
    do {
        changed = false;
        std::vector<std::set<Item>> new_states;

        for (const auto& I : canonical_collection) {
            std::set<char> symbols;
            for (const auto& item : I) {
                if (item.dot_position < item.right.length()) {
                    symbols.insert(item.right[item.dot_position]);
                }
            }

            for (char X : symbols) {
                std::set<Item> goto_set = go_to(I, X);
                if (!goto_set.empty() && 
                    std::find(canonical_collection.begin(), canonical_collection.end(), goto_set) == canonical_collection.end() &&
                    std::find(new_states.begin(), new_states.end(), goto_set) == new_states.end()) {
                    new_states.push_back(goto_set);
                    changed = true;
                }
            }
        }

        canonical_collection.insert(canonical_collection.end(), new_states.begin(), new_states.end());
    } while (changed);
}

std::set<Item> LRParser::closure(const std::set<Item>& I) {
    std::set<Item> result = I;
    bool changed;
    do {
        changed = false;
        std::set<Item> new_items;
        for (const auto& item : result) {
            if (item.dot_position < item.right.length() && isupper(item.right[item.dot_position])) {
                char B = item.right[item.dot_position];
                for (const auto& production : grammar[B]) {
                    Item new_item = {B, production, 0};
                    if (result.find(new_item) == result.end()) {
                        new_items.insert(new_item);
                        changed = true;
                    }
                }
            }
        }
        result.insert(new_items.begin(), new_items.end());
    } while (changed);
    return result;
}

std::set<Item> LRParser::go_to(const std::set<Item>& I, char X) {
    std::set<Item> J;
    for (const auto& item : I) {
        if (item.dot_position < item.right.length() && item.right[item.dot_position] == X) {
            J.insert({item.left, item.right, item.dot_position + 1});
        }
    }
    return closure(J);
}

void LRParser::construct_parsing_table() {
    goto_table.resize(canonical_collection.size());
    action_table.resize(canonical_collection.size());

    for (size_t i = 0; i < canonical_collection.size(); ++i) {
        for (const auto& item : canonical_collection[i]) {
            if (item.dot_position < item.right.length()) {
                char symbol = item.right[item.dot_position];
                std::set<Item> goto_set = go_to(canonical_collection[i], symbol);
                auto it = std::find(canonical_collection.begin(), canonical_collection.end(), goto_set);
                if (it != canonical_collection.end()) {
                    int j = std::distance(canonical_collection.begin(), it);
                    if (isupper(symbol)) {
                        goto_table[i][symbol] = j;
                    } else {
                        action_table[i][symbol] = "s" + std::to_string(j);
                    }
                }
            } else {
                if (item.left == grammar.begin()->first && item.dot_position == item.right.length()) {
                    action_table[i]['$'] = "acc";
                } else {
                    for (const auto& rule : grammar) {
                        auto it = std::find(rule.second.begin(), rule.second.end(), item.right);
                        if (it != rule.second.end()) {
                            int rule_number = std::distance(grammar.begin(), std::find_if(grammar.begin(), grammar.end(),
                                [&](const auto& p) { return &rule == &p; }));
                            for (char c : {'a', 'b', 'c', 'd', '+', '*', '(', ')', '$'}) {
                                action_table[i][c] = "r" + std::to_string(rule_number);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

void LRParser::print_items() {
    for (size_t i = 0; i < canonical_collection.size(); ++i) {
        std::cout << "I" << i << ":" << std::endl;
        for (const auto& item : canonical_collection[i]) {
            std::cout << "  " << item.left << " -> ";
            for (size_t j = 0; j < item.right.length(); ++j) {
                if (j == item.dot_position) std::cout << ".";
                std::cout << item.right[j];
            }
            if (item.dot_position == item.right.length()) std::cout << ".";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void LRParser::print_parsing_table() {
    std::cout << "Action Table:" << std::endl;
    for (size_t i = 0; i < action_table.size(); ++i) {
        std::cout << i << ": ";
        for (const auto& action : action_table[i]) {
            std::cout << action.first << ":" << action.second << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nGoto Table:" << std::endl;
    for (size_t i = 0; i < goto_table.size(); ++i) {
        std::cout << i << ": ";
        for (const auto& goto_action : goto_table[i]) {
            std::cout << goto_action.first << ":" << goto_action.second << " ";
        }
        std::cout << std::endl;
    }
}

bool LRParser::parse(const std::string& input) {
    std::stack<int> state_stack;
    state_stack.push(0);  // Initial state
    size_t input_pos = 0;

    std::cout << "Input string: " << input << std::endl;

    while (true) {
        int s = state_stack.top();
        char a = (input_pos < input.length()) ? input[input_pos] : '$';

        std::cout << "Current state: " << s << ", Next input: " << a 
                  << " (position " << input_pos << ")" << std::endl;
        std::cout << "Stack: ";
        std::stack<int> temp = state_stack;
        while (!temp.empty()) {
            std::cout << temp.top() << " ";
            temp.pop();
        }
        std::cout << std::endl;

        if (action_table[s].find(a) == action_table[s].end()) {
            std::cout << "Error: No action defined for state " << s << " and input " << a << std::endl;
            return false;
        }

        std::string action = action_table[s][a];
        std::cout << "Action: " << action << std::endl;
        
        if (action[0] == 's') {
            int next_state = std::stoi(action.substr(1));
            state_stack.push(next_state);
            input_pos++;
            std::cout << "Shifted to state " << next_state << ", advanced input to position " << input_pos << std::endl;
        } else if (action[0] == 'r') {
            int rule = std::stoi(action.substr(1));
            auto it = grammar.begin();
            std::advance(it, rule);
            int pop_count = it->second[0].length();
            
            std::cout << "Reducing by rule: " << it->first << " -> " << it->second[0] << std::endl;
            std::cout << "Pop count: " << pop_count << ", Stack size: " << state_stack.size() << std::endl;

            if (state_stack.size() <= pop_count) {
                std::cout << "Warning: Attempted to pop more elements than exist in the stack. Adjusting pop count." << std::endl;
                pop_count = state_stack.size() - 1;  // Ensure we don't pop the initial state
            }

            for (int i = 0; i < pop_count; ++i) {
                state_stack.pop();
            }

            int t = state_stack.top();
            if (goto_table[t].find(it->first) == goto_table[t].end()) {
                std::cout << "Error: No goto action for state " << t << " and non-terminal " << it->first << std::endl;
                return false;
            }
            int goto_state = goto_table[t][it->first];
            state_stack.push(goto_state);
            std::cout << "After reduction, pushed state " << goto_state << " onto stack" << std::endl;
        } else if (action == "acc") {
            std::cout << "Input accepted" << std::endl;
            return true;
        } else {
            std::cout << "Error: Invalid action " << action << std::endl;
            return false;
        }
    }
}


int main() {
    std::map<char, std::vector<std::string>> grammar = {
        {'S', {"E"}},
        {'E', {"E+T", "T"}},
        {'T', {"T*F", "F"}},
        {'F', {"(E)", "d"}}
    };

    LRParser parser(grammar);

    std::cout << "LR(0) Items:" << std::endl;
    parser.print_items();

    std::cout << "Parsing Table:" << std::endl;
    parser.print_parsing_table();

    std::string input = "dd";
    std::cout << "Parsing input: " << input << std::endl;
    bool result = parser.parse(input);
    std::cout << "Parsing result: " << (result ? "Success" : "Failure") << std::endl;

    return 0;
}