#include <iostream>
#include <math.h>

void vector_result(float x, float y, float z){
    float div_value = pow(pow(x, 2)+pow(y, 2)+pow(z,2), 0.5);
    std::cout<<"x: "<<x/div_value<<" y: "<<y/div_value<<" z: "<<z/div_value;
    }

int main(){
  vector_result(-600, 200, -400);  
}