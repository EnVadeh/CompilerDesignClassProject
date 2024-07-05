#pragma once
#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <set>

class PARSING_SOURCE
{  
public:
    char* source;
    char c;
    unsigned int i;
    unsigned int src_size;
    //I have to change the datastructure completely from the first grammar code...
    //Now I want Every Variable to map to eachother... 
    //Because using simple 1d array is complilacated, and 2d arrays make thigns more complicated ot think of
    //map already allows easy lookups
    std::map<char, std::vector<std::string>> grammar_rules; 
    std::map<char, std::set<char>> first_sets;
    std::map<char, std::set<char>> follow_sets;

    PARSING_SOURCE(char* source);

    void GRAMMAR_NEXT();
    void SKIP_SPACE();
    void PARSE_GRAMMAR();
    void ADD_RULE(char non_terminal, const std::string& production);
    void PRINT_GRAMMAR();

    void COMPUTE_FIRST_SETS();
    void COMPUTE_FOLLOW_SETS();
    std::set<char> FIRST(const std::string& s);
    void PRINT_FIRST_SETS();
    void PRINT_FOLLOW_SETS();
};

#endif