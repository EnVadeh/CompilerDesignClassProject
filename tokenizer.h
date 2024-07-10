#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct TOKEN_STRUCT
{
	char* value;
	enum type {
		TOKEN_ID,
		TOKEN_INT,
		TOKEN_EQ,
		TOKEN_LB, //left bracket
		TOKEN_RB,
		TOKEN_LC, //left curly
		TOKEN_RC,
		TOKEN_SCOLON,
		TOKEN_ADD,
		TOKEN_SUB,
		TOKEN_MUL,
		TOKEN_DIV,
		TOKEN_CS, //control statement such as if, else
		TOKEN_LP, //Loops
		TOKEN_EOF
	}types;     // stores the value of the enum for that instance
};

struct LEXER_STRUCT
{
	char* src;
	char c;
	unsigned int i;
	unsigned int src_size;
};  // i could have typedefed here but for some reason the language procesor was acting weird and giving errors



typedef LEXER_STRUCT LEXER_L;
typedef TOKEN_STRUCT TOKEN_T;

struct TOKEN_COUNTER_STRUCT {
	static int counter;
	TOKEN_T token;
	static int max_token;
	//basically after end of line, I want the counter value to be copied here.. that way the token counter will know to stop going through arrays
};


LEXER_L* init_lexer(char* src);
void lexer_advance(LEXER_L* lexer);
TOKEN_T* lexer_advance_with(LEXER_L* lexer, TOKEN_T* token);
TOKEN_T* lexer_advance_current(LEXER_L* lexer, int itype);
void lexer_skip_space(LEXER_L* lexer);
TOKEN_T* parse_id(LEXER_L* lexer);
TOKEN_T* parse_num(LEXER_L* lexer);
TOKEN_T* lexer_next_token(LEXER_L* lexer);
TOKEN_T* tokenizer(char* src, int itype);
TOKEN_COUNTER_STRUCT* store_token();
void store_add(TOKEN_T* token, TOKEN_COUNTER_STRUCT* store);
//LEXER_L* lexer_special_character(LEXER_L* lexer);
//std::unique_ptr<TOKEN_STRUCT> tokenizer(char* src, int itype);
#endif
