/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stack.h
 * Author: siddi558
 *
 * Created on March 18, 2018, 2:46 PM
 */

#ifndef STACK_H
#define STACK_H
#include "Map.h"





class stack{
    
public:


    stack();
   stack (Node *x, double travel, Node *prev, unsigned prev_edge);
   stack ( unsigned xx, double travelx);
   
   stack (unsigned xs, double min, double r);
    Node * curr_node;
    
    double travel_time;
    
    Node * prev_node;
    
    unsigned prev_seg;
    
    unsigned curr_n;
    
    double trave;
   
    unsigned xsz;
    double minz;
    double xa;
    ~stack();
    
   
    
};  
   




#endif /* STACK_H */

