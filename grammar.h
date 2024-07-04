#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <string.h>

class GRAMMAR_SOURCE
{   
    public:
	char* source;
	char c;
	unsigned int i;
	unsigned int src_size;
    bool fromArrow; //0 for left, 1 for right
    std::string left_grammar;
    std::string right_grammar;

    GRAMMAR_SOURCE(char* source);

    void GRAMMAR_NEXT();

    void CHECK_DIR();

    bool PEEK_FUNCTION();

    void GRAMMAR_LEFT();

    void GRAMMAR_RIGHT();

    void CHECK_LEFT_RECURCSION();

};



#endif