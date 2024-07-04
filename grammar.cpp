#include <iostream>
#include <math.h>
#include "grammar.h"
#include <cstring>

GRAMMAR_SOURCE::GRAMMAR_SOURCE(char* source){
  this->source = source;
  i = 0;
  src_size = strlen(source);
}

void GRAMMAR_SOURCE::GRAMMAR_NEXT(){
  if(i<src_size && c!='\0'){
    i++;
    c = source[i];
  }
}

void GRAMMAR_SOURCE::GRAMMAR_LEFT(){
  left_grammar += c;
}


void GRAMMAR_SOURCE::GRAMMAR_RIGHT(){
  right_grammar += c;
}

bool GRAMMAR_SOURCE::PEEK_FUNCTION(){
  if(source[i+1]='>'){
    return true;
    i = i+2;
  }
  else
    return false;
}

void GRAMMAR_SOURCE::CHECK_LEFT_RECURCSION(){
  
  GRAMMAR_NEXT();

}
//we make the user input the grammars in a way. 
// A -> A/B
// A -> A'B
//B -> A

//will need to store lefts, and first rights, and second and third rights.

int main(){
  
}