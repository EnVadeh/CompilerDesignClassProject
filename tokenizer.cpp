#include <cstring>
#include "tokenizer.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>


//initialising the static variable
int TOKEN_COUNTER_STRUCT::counter = 0;
int TOKEN_COUNTER_STRUCT::max_token = 0;


//wanted to tokenize for an entire line, but I think I shoudl tokenize until there's a semicolon because then the smicolon has an actual usage

TOKEN_T* tokenizer(char* src, int itype) {
    TOKEN_T* token = new(struct TOKEN_STRUCT);
    if (src == 0 ) {
        token->types = token->TOKEN_EOF;
        return nullptr;
    }
    else {
        size_t len = strlen(src);
        token->value = new char[len + 1];
        strncpy(token->value, src, len); //try using strncpy_s to prevent bfufer problems
        token->value[len] = '\0';
        token->types = static_cast<TOKEN_STRUCT::type>(itype);
    }
    return token;
}

LEXER_L* init_lexer(char* src) {
    LEXER_L* lexer = new(struct LEXER_STRUCT);
    lexer->src = src;
    lexer->i = 0;
    lexer->src_size = strlen(src);
    lexer->c = src[lexer->i];
    return lexer;
}

void lexer_advance(LEXER_L* lexer) { //string of letter or numbers or alphanums are skipped
    if (lexer->i < lexer->src_size && lexer->c != '\0') { //skipping semicolon because if not then it'll treat semicolon as normal string
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

TOKEN_T* lexer_advance_with(LEXER_L* lexer, TOKEN_T* token) { // emtpy because when i use the lexer_advance inside lexer_advance_with, i had to then use the lexer_special_character function which is obviously very slow
    return token;
}

TOKEN_T* lexer_advance_current(LEXER_L* lexer, int itype) {
    std::string value;
    value = lexer->c;
    value = value + '\0';

    char* valuesrc = &value[0];
    TOKEN_T* token = tokenizer(valuesrc, itype);
    lexer_advance(lexer);
    return token;
}

void lexer_skip_space(LEXER_L* lexer) {
    while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == 13 || lexer->c == 10) {
        lexer_advance(lexer);
    }
}


TOKEN_T* parse_id(LEXER_L* lexer) {
    std::string value;
    while (isalnum(lexer->c)) {
        value += lexer->c;
        lexer_advance(lexer);
    }
    char* valuestr = &value[0];
    //lexer_special_character(lexer);
    return tokenizer(valuestr, TOKEN_STRUCT::TOKEN_ID);
}

TOKEN_T* parse_num(LEXER_L* lexer) {
    std::string value;
    while (isdigit(lexer->c)) {
        value += lexer->c;
        lexer_advance(lexer);
    }
    char* valuestr = &value[0];
    //lexer_special_character(lexer);
    return tokenizer(valuestr, TOKEN_STRUCT::TOKEN_INT);
}

// btw the eof function isn't printed because the while loop ends when it's eof... cause of nullptr
TOKEN_T* lexer_next_token(LEXER_L* lexer) {
    while (lexer->c != '\0') {
        lexer_skip_space(lexer);
        if (isalpha(lexer->c))
            return lexer_advance_with(lexer, parse_id(lexer));
        if (isdigit(lexer->c)) 
            return lexer_advance_with(lexer, parse_num(lexer));
        switch (lexer->c) {
        case '=':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_EQ);
            break;
        case '(':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_LB);
            break;
        case '{':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_LC);
            break;
        case ')':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_RB);
            break;
        case '}':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_RC);
            break;
        case ';':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_SCOLON);
            break;
        case '+':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_ADD);
            break;
        case '-':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_SUB);
            break;
        case '*':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_MUL);
            break;
        case '/':
            return lexer_advance_current(lexer, TOKEN_STRUCT::TOKEN_DIV);
            break;

        case '\0': break;
        default: std::cout << "unexpected character" << std::endl; break;
        }
    }
    return tokenizer(0, TOKEN_STRUCT::TOKEN_EOF);
}

TOKEN_COUNTER_STRUCT* store_token() {
    TOKEN_COUNTER_STRUCT* token_storage = new(struct TOKEN_COUNTER_STRUCT[100]);
    return token_storage;
}

void store_add(TOKEN_T* tok, TOKEN_COUNTER_STRUCT* store) {
    TOKEN_T newToken;
    newToken = *tok;
    store[store->counter].token = newToken;
    store->counter++;
    store->max_token++;
}