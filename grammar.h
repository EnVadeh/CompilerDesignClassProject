#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H


class GRAMMAR_SOURCE
{   
    public:
	char* source;
	char c;
	unsigned int i;
	unsigned int src_size;
    bool fromArrow; //0 for left, 1 for right

    GRAMMAR_SOURCE(char* source);

    void GRAMMAR_NEXT();

    void CHECK_DIR();

    bool PEEK_FUNCTION();

    void GRAMMAR_LEFT();

    void GRAMMAR_RIGHT();


};



#endif