#include "stack.h"

stack::stack(){

}

stack::stack (Node * x, double travel, Node * prev, unsigned prev_edge){
    curr_node = x;
    
    travel_time = travel;
    
    prev_node = prev;
    
    prev_seg = prev_edge;
    
    
    
}

stack::stack ( unsigned xx, double travelx){
  curr_n = xx;
trave= travelx;  
}

stack::stack(unsigned xs, double min, double r){
    xsz=xs;
    minz=min;
    xa=r;
}
stack::~stack(){
    
}


