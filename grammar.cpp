#include <iostream>
#include <math.h>
#include "grammar.h"
#include <cstring>

GRAMMAR_SOURCE::GRAMMAR_SOURCE(char* source){
  this->source = source;
  i = 0;
  src_size = strlen(source);
  productions = 0;
  c = source[0];
}

void GRAMMAR_SOURCE::GRAMMAR_NEXT(){
  if(i<src_size /*&& c!='\0'*/){
    i++;
    c = source[i];
  }
}

void GRAMMAR_SOURCE::GRAMMAR_LEFT(){ // A -> B
  while(true){
  SKIP_SPACE();
  if(c=='-'&&PEEK_FUNCTION()){
  GRAMMAR_NEXT();
  GRAMMAR_NEXT();
  GRAMMAR_RIGHT();
  return;
  }
  left_grammar += c;
  GRAMMAR_NEXT();
  }
}


void GRAMMAR_SOURCE::GRAMMAR_RIGHT(){ //A->BC|C
  while(c!='\0'){
  SKIP_SPACE();
  if(c=='|'){//right now even if there's |, the different rules are being merged
  GRAMMAR_NEXT();
  productions++;
  }
  right_grammar[productions] += c; //ofc this doesn't work if the grammar has error like this: A->B|... 
  GRAMMAR_NEXT();
  }
}
void GRAMMAR_SOURCE::SKIP_SPACE() {
    while (c == ' ' || c == '\t' || c == 13 || c == 10) {
        GRAMMAR_NEXT();
    }
}

bool GRAMMAR_SOURCE::PEEK_FUNCTION(){
  if(source[i+1]=='>'){
    return true;
  }
  else
    return false;
}

void GRAMMAR_SOURCE::CHECK_LEFT_RECURCSION(){
  GRAMMAR_LEFT();
  std::cout<<"Original production is: "<<std::endl;
  std::cout<<left_grammar[0]<<" -> ";
  for(int z = 0; z <productions; z++)
  std::cout<<right_grammar[z]<<" |";
  std::cout<<right_grammar[productions]<<std::endl;
  if(left_grammar[0]==right_grammar[0][0])
  REMOVE_RECURSION();
}

void GRAMMAR_SOURCE::REMOVE_RECURSION(){
  int x = 1;
  int temp = 0;
  std::string temp_grammar;
  while(right_grammar[0][x]!='\0'){
  temp++;
  temp_grammar +=right_grammar[0][x];
  x++;
  }
  temp_grammar = temp_grammar + left_grammar[0] + "'";
  std::cout<<"Left recursion removed productions are : "<<std::endl<<left_grammar[0]<<" -> ";
  for(int z = 1; z <productions; z++)
  std::cout<<right_grammar[z]<<left_grammar[0]<<"' |";
  std::cout<<right_grammar[productions]<<left_grammar[0]<<"'"<<std::endl;
  std::cout<<left_grammar[0]<<"' -> "<<temp_grammar<<" | "<<"e"<<std::endl;
}

//we make the user input the grammars in a way. 
// A -> A|B
// A -> A'B
//B -> A

//will need to store lefts, and first rights, and second and third rights.

int main(){
  std::string test = "A->Axyz|C|D";
  char* source = &test[0];
  GRAMMAR_SOURCE GS(source);
  GS.CHECK_LEFT_RECURCSION();
}