/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include <unordered_map>
#include <math.h>
#include <bits/random.h>
#include "Map.h"
#include "m2.h"
#include "easygl/graphics.h"
#include "graphics.h"
#include "mapConnect.h"
#include <sstream>
#include <algorithm>
#include <string>
#include <cctype>
#include <list>
#include <queue>
#include "m3.h"
#include "stack.h"
#include <stack>



std::vector<unsigned> heuristic; //heuristic value for each intersection
std::vector<unsigned> TimeFromStart;
std::vector<unsigned> QueueDone; //queue for id that are done 
std::vector<unsigned> travelFrom; //which intersection did it come from
std::list<struct WeightedInt>QueueToSearch; //sort by weight
double angle (t_point A, t_point B);
//--------------   <Weight  ,NodeID  > so smallest weight first 
double make_Heuristic_calc(unsigned curr_vertex, unsigned id_end, int SpdStandard);
std::vector<unsigned> find_path_between_intersections2(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);
std::vector<unsigned> find_path_between_intersections3(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);

double compute_path_travel_time(const std::vector<unsigned>& path,
        const double turn_penalty) {
    //takes a vector of [Street Segment ID] and compute travel time.
    double totalTime = 0; //accumulate in for loop
    double numofSegs = path.size(); //total seg number in this path
    if (numofSegs == 0)
        return 0;
    //unsigned previousStreetID = getStreetSegmentInfo(path[0]).streetID;

    for (unsigned iSeg = 0; iSeg < numofSegs; iSeg++) { //traverse through all seg
        totalTime += map->get_travel(path[iSeg]);
        //unsigned currentStreetID = map->get_Seg_Info_v(path[iSeg]).streetID;

        if (iSeg != 0) { //means a turn

            if (map->get_Seg_Info_v(path[iSeg]).streetID != map->get_Seg_Info_v(path[iSeg - 1]).streetID) {
                //previousStreetID = currentStreetID;
                totalTime += turn_penalty; //add turn_penalty to time
            }
        }

    }

    return totalTime ;


}
std::vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty){
    
    //std::vector<unsigned> path1 = find_path_between_intersections2 (intersect_id_start,intersect_id_end, turn_penalty);
    std::vector<unsigned> path2 = find_path_between_intersections3 (intersect_id_start,intersect_id_end, turn_penalty);
    
    //double x= compute_path_travel_time(path1, turn_penalty);
    //double y = compute_path_travel_time (path2, turn_penalty);
    
    //if (x< y){
    //    return path1;
    //}
    
   // else{
        return path2;
   // }
    
}

//-------------------------the main FIND_PATH FUNCTION-------------------------

/*std::vector<unsigned> find_path_between_intersections2(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty) {
    
    //std::cout<<"from "<<intersect_id_start<<" to "<<intersect_id_end<<std::endl;//test!

    int SpeedStandard = 60; //to calculate heuristic, reference speed. May Change

    
    unsigned numb_of_int = map->get_number_of_intersections();
    std::vector<unsigned> final_path;
    bool closed_list [numb_of_int];  //Test This was not being used
    bool open_list [numb_of_int];
    unsigned parent_node [numb_of_int]; // holds parent node.
    unsigned travel_time_from_start [numb_of_int]; // holds distance from start
    unsigned parent_street_id [numb_of_int];
    unsigned f_node[numb_of_int];
    for (unsigned ii = 0; ii < numb_of_int; ii++) {
        closed_list[ii] = 0;
        open_list[ii] = 0;
        travel_time_from_start [ii] = 4094967295;//max
        parent_street_id [ii] = 4094967295;
        f_node[ii] = 4094967295;
        //parent_node[ii] = numb_of_int + 1;

    }

    struct compare {

        bool operator()(const stack& lhs, const stack& rhs) {
            return lhs.trave > rhs.trave;
        }

    };
    std::priority_queue<stack, std::vector<stack>, compare> open_list_queue;
    int pqi=0;

    //std::priority_queue<stack, std::vector<stack>, compare> opp_list_queue;


    unsigned current_vertex = intersect_id_start;
    double g = 0;
    travel_time_from_start[intersect_id_start] = 0;
    double h = make_Heuristic_calc(current_vertex, intersect_id_end, SpeedStandard);
    double f = g + h;
    f_node[intersect_id_start] = f;

    double best_f = INFINITY;

    bool found = 0;

    open_list_queue.push(stack(intersect_id_start, f));
    //opp_list_queue.push(stack(intersect_id_end,f));
    // if (are_directly_connected(intersect_id_start, intersect_id_end)) {
    //   final_path.push_back(map->from_to_int_street_seg_id[std::make_pair(intersect_id_start, intersect_id_end)]);
    // }
//    unsigned count = 0;
    
   
    //unsigned limit = numb_of_int / 4;
    if (intersect_id_start != intersect_id_end ) {


        while (!open_list_queue.empty() && !found ) {//main loop

            current_vertex = open_list_queue.top().curr_n;
            double current_time = open_list_queue.top().trave;
            closed_list[current_vertex] =1;
            
            
            if (current_vertex != intersect_id_start) {

                if (best_f < current_time ) {
                    found = 1;
                    
                }
            }



            if (!found) {
                
                current_vertex = open_list_queue.top().curr_n;
                double current_time = open_list_queue.top().trave;
                open_list_queue.pop();
                
               
                
                
                    
                
                    if (intersect_id_end == current_vertex ) {
                        double f_t = f_node[current_vertex];
                        //double o_t = f_node[opp_vertex];
                        
                        if(best_f > (f_t ))
                            best_f = f_t ;
                    }
                
                    



                    
                    std::vector <unsigned> adjacent = map->adj_int[current_vertex];
                    std::vector<unsigned> opp = map->adj_int[intersect_id_end];
                    
                    closed_list[current_vertex] = 1;//test initial the CurrentVertex

                    for (unsigned i = 0; i < adjacent.size(); i++) {// traverses adjacent intersections
                       
                        
                        std::vector <double> travel;
                        std::vector<unsigned> street_id;
                        std::vector<unsigned> street_seg_id;
                            double d = travel_time_from_start[current_vertex];
                            auto range = map->from_to_int_travel_time.equal_range(std::make_pair(current_vertex, adjacent[i])); // traveltime from curr vertex to new
                            
                            for (auto j = range.first; j != range.second; ++j){
                                travel.push_back(j->second);
                            }
                            
                            auto range1 = map->from_to_int_street_id.equal_range(std::make_pair(current_vertex, adjacent[i])); // traveltime from curr vertex to new
                            
                            for (auto j1 = range1.first; j1 != range1.second; ++j1){
                                street_id.push_back(j1->second);
                            }
                            
                            auto range11 = map->from_to_int_street_seg_id.equal_range(std::make_pair(current_vertex, adjacent[i])); // traveltime from curr vertex to new
                            
                            for (auto j11 = range11.first; j11 != range11.second; ++j11){
                                street_seg_id.push_back(j11->second);
                            }
                            
                            
                            double min_r= INFINITY;
                            unsigned count = 0;
                            //bool turn = false;
                            
                            for (unsigned l=0; l<travel.size(); l++){
                                double r = travel[l];
                                if (current_vertex != intersect_id_start) { //add turn penalty
                                    if (street_id[l] != parent_street_id [current_vertex]){
                                        double angle1 = angle(map->get_seg_from(map->from_to_int_street_seg_id1[std::make_pair(parent_node[current_vertex], current_vertex)]),map->get_seg_to(map->from_to_int_street_seg_id1[std::make_pair(parent_node[current_vertex], current_vertex)]));
                                        double angle2 = angle (map->get_seg_from(street_seg_id[l]), map->get_seg_to(street_seg_id[l]));
                                        if ((angle2 - angle1) <= 45){
                                            r += turn_penalty;
                                        }
                                        else if (((angle2 - angle1) > 45) && ((angle2 - angle1) <= 90)){
                                            r += 2*turn_penalty;
                                        }
                                        
                                        else if ((angle2 - angle1) > 90 ){
                                            r += 3*turn_penalty;
                                        }
                                        
                                        
                                    }
                                    
                                   
                                    
                                    
                                    
                                }
                                
                                if(r<min_r){
                                    min_r =r;
                                    count =l;
                                    
                                }
                            }
                            g = d + min_r;
                            map->from_to_int_street_seg_id1[std::make_pair(current_vertex, adjacent[i])] = street_seg_id[count];
                            parent_street_id[adjacent[i]] = street_id[count];
                            
                            




                            if (travel_time_from_start[adjacent[i]] > g) {
                                travel_time_from_start[adjacent[i]] = g;
                                h = make_Heuristic_calc(adjacent[i], intersect_id_end, map->max_speed);
                                double new_f;
                                
                                //if(make_Heuristic_calc(current_vertex, intersect_id_end, SpeedStandard) > g+h)
                                if(h>g){
                                     new_f = g+g;
                                     std::cout<< "Fail";
                                }
                                else if (h<g){
                                     new_f = g + h;
                                }
                                
                                if (f_node[adjacent[i] > new_f]) {
                                    parent_node[adjacent[i]] = current_vertex;
                                    f_node[adjacent[i]] = new_f;
                                    open_list_queue.push(stack(adjacent[i], new_f));
                                    
                                }
                                //h = make_Heuristic_calc(adjacent[i], intersect_id_end, SpeedStandard);
                                //double new_f = g + h;
                                //f_node[adjacent[i]] = new_f;

                            }
                        
                            

                    }//end of traverse adjacent loop
                    
                    adjacent.clear();
                     

                
                               // end of while loop
            }
        }
        unsigned prev_node = INFINITY;
        unsigned current_node = intersect_id_end;

        std::stack <unsigned> seg_id1;
        if (found) {
            // std::cout<< current_node<< " ";
            while (prev_node != intersect_id_start) {
                //std::cout<< current_node<< " ";

                prev_node = parent_node[current_node];
                seg_id1.push(map->from_to_int_street_seg_id1[std::make_pair(prev_node, current_node)]);
                current_node = prev_node;
            }

            while (!seg_id1.empty()) {
                unsigned id = seg_id1.top();

                final_path.push_back(id);
                seg_id1.pop();
            }



        } else {
            final_path = {};


        }
    } else {
        final_path = {};
    }


   // open_list_queue.clear();




    return final_path;










    //build the heuristic for all points
    //<vect>heuristic made


    //clear every data structure

    //QueueToSearch.clear();

}///======================end of the main FIND_PATH Function=====================*/
std::vector<unsigned> find_path_between_intersections3(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty) {

    
    // final path
    std::vector<unsigned> final_path;
    // current time
    double current_travel_time;
    
    
    //compare for queue
    struct compare {

        bool operator()(const stack& lhs, const stack& rhs) {
            return lhs.travel_time > rhs.travel_time;
        }

    };
    
    Node * start_vertex = &map->nodes[intersect_id_start]; // initial node
    
    
    std::priority_queue<stack, std::vector<stack>, compare> open_list_queue; // priority queue
    open_list_queue.push(stack(start_vertex, 0, NULL, INFINITY)); // initial node stored in queue
    
    
    
    double best_time = INFINITY; // default best time
    bool found = 0; // flag

    
    
   
    //std::cout<< " start" << std::endl;

        while (!open_list_queue.empty() && !found) {//main loop

            
            
            if (open_list_queue.top().curr_node != start_vertex) {

                if (best_time < open_list_queue.top().travel_time) {
                    found = true;
                    
                }
            }



            if (!found) {
                
                stack current_vertex = open_list_queue.top();
                open_list_queue.pop();
                
                Node  * present_node = current_vertex.curr_node;
                
                 
                if (current_vertex.travel_time < present_node->travel_time_from_start){
                   
                    if( present_node-> id != intersect_id_start){
                        present_node->prev_edge = current_vertex.prev_seg;
                        present_node->parent_node = current_vertex.prev_node;
                    }
                    present_node->travel_time_from_start = current_vertex.travel_time;
                    
                    if (present_node->id == intersect_id_end) {
                        double p = present_node->travel_time_from_start;
                        
                        if (p < best_time){    
                            best_time = p;
                        }
                        
                    }



                    
                    std::vector<Edge> adjacent = map->edges[present_node->id];

                    for (unsigned i = 0; i < adjacent.size(); i++) {// traverses adjacent intersections

                        if(present_node != adjacent[i].to_node){
                            current_travel_time = (present_node->travel_time_from_start + adjacent[i].travel_time);
                            
                            if (present_node->id != intersect_id_start){
                                if(map->get_Seg_Info_v(adjacent[i].street_seg_id).streetID != map->get_Seg_Info_v(current_vertex.prev_seg).streetID){
                                    current_travel_time = current_travel_time + turn_penalty;
                                }
                            }
                            
                            
                            //double h = make_Heuristic_calc(adjacent[i].to_node->id, intersect_id_end, map->max_speed);
                            //current_travel_time = current_travel_time + h;
                            //std::cout<< adjacent[i].to_node->id << " " << current_travel_time<< std::endl;
                            open_list_queue.push(stack(adjacent[i].to_node, current_travel_time, present_node, adjacent[i].street_seg_id));
                        }

                            


                        
                    }//end of traverse adjacent loop
                    

                }
            }

        } // end of while loop

        unsigned prev_node = INFINITY;
        unsigned current_node = intersect_id_end;
        Node * end_int = &(map->nodes[intersect_id_end]);
        std::stack <unsigned> seg_id;
        
            // std::cout<< current_node<< " ";
            while (end_int->parent_node!= NULL) {
                //std::cout<< current_node<< " ";

                
                seg_id.push(end_int->prev_edge);
                end_int = end_int->parent_node;
            }

            while (!seg_id.empty()) {
                unsigned id = seg_id.top();

                final_path.push_back(id);
                seg_id.pop();
            }



        
        for (unsigned ii=0; ii < map->number_of_intersectionss; ii++){
           map->nodes[ii].travel_time_from_start = INFINITY;
           map->nodes[ii].prev_edge = INFINITY;
           map->nodes[ii].parent_node =NULL;
       }

      


    

       // std::cout<< best_time<< std::endl;
        //std::cout<< " end" << std::endl;

    return final_path;










    

}//======================end of the main FIND_PATH Function=====================

double make_Heuristic_calc(unsigned curr_vertex, unsigned id_end, int SpdStandard) {
    //unsigned totalIntNum= map->get_number_of_intersections();
    LatLon IDLatLon;
    unsigned IDdistance;

    LatLon EndLatLon = map->get_Lat_Lon_i(id_end); //find the latlon of end
    
    //for(unsigned IDi=0; IDi<totalIntNum-1; IDi++){
    IDLatLon = map->get_Lat_Lon_i(curr_vertex);
    IDdistance = find_distance_between_two_points(IDLatLon, EndLatLon);
    double timeHeuristic = sqrt((IDdistance / SpdStandard)*3.6) ;
    return timeHeuristic;
    //store the time-to-end for all intersections

    //return 0;
}

std::vector<unsigned> find_path_to_point_of_interest2(const unsigned intersect_id_start,
        const std::string point_of_interest_name,
        const double turn_penalty) {

    std::vector<unsigned> final_path;
    double ss = 123;
    unsigned id;
    unsigned Int_end = 0;
    double min_d = INFINITY;
    double min_int_d = INFINITY;
    LatLon pos;
    
    struct compare {

        bool operator()(const stack& lhs, const stack& rhs) {
            return lhs.minz > rhs.minz;
        }

    };
    std::priority_queue<stack,std::vector<stack>, compare> pq;

    LatLon int_pos = map->get_Lat_Lon_i(intersect_id_start);
    std::vector<unsigned> poi_ids = map->get_POI_id_from_name(point_of_interest_name);

    for (unsigned i = 0; i < poi_ids.size(); i++) {
        LatLon current_POI_pos = map->get_LatLon_P(poi_ids[i]);
        
        unsigned int_id = find_closest_intersection(current_POI_pos);
        LatLon xx;
        xx= map->get_Lat_Lon_i(int_id);
        double dist = find_distance_between_two_points(int_pos, xx);
        
        
        if (dist < min_d) {
            min_d = dist;
            id = i;
            Int_end= int_id;
            pq.push(stack(Int_end,min_d,ss));
            pos = current_POI_pos;
        }
    }
    
   /* for (unsigned j = 0; j < map->get_number_of_intersections(); j++) {
        LatLon current_int_pos = map->get_Lat_Lon_i(j);
        double dist = find_distance_between_two_points(pos, current_int_pos);
        if (dist < min_int_d) {
            min_int_d = dist;
            Int_end = j;

        }
    }*/
    double mind= INFINITY;
    std::vector<unsigned> real_final_path;
    int count=0;
    while(!pq.empty() && count!=10){
    
        stack r = pq.top();
        pq.pop();
        final_path = find_path_between_intersections3(intersect_id_start, r.xsz, turn_penalty);
        double ma= compute_path_travel_time (final_path, turn_penalty);
        
        if (ma<mind){
            mind=ma;
            real_final_path = final_path;
        }
        count+=1;
    }
    map->path_poi = pos;
    return real_final_path;

}


void calc_Near_Intersect();

struct WeightedInt {
    unsigned weight;
    unsigned ID;
};

double angle (t_point A, t_point B) {
    double angle;
    angle = atan((B.y - A.y) / (B.x - A.x)) / DEG_TO_RAD;
    return angle;

}

std::vector<unsigned> find_path_to_point_of_interest(const unsigned intersect_id_start,
        const std::string point_of_interest_name,
        const double turn_penalty){
    
    std::vector<unsigned> final_path;
    unsigned p_id;
    double travel_p = INFINITY;
    unsigned i_id;
    double travel_i= INFINITY;
    
    LatLon poi;
    
    LatLon start_i = map->get_Lat_Lon_i(intersect_id_start);
    
    for (unsigned i= 0; i< map->get_number_of_POI(); i++){
        if (map->get_POI_name(i) == point_of_interest_name){
            LatLon temp_poi = map->get_LatLon_P(i);
            double x = find_distance_between_two_points (start_i, temp_poi);
            
            if(x<travel_p){
                travel_p = x;
                p_id =i;
                poi = temp_poi;
                
            }
            
        }
    }
    
    
    for (unsigned j=0; j< map->get_number_of_intersections(); j++){
        LatLon temp_i = map->get_Lat_Lon_i(j);
        double xx = find_distance_between_two_points (poi, temp_i);
        if (xx < travel_i){
            travel_i = xx;
            i_id = j;
        }
    }
    
    final_path = find_path_between_intersections3(intersect_id_start, i_id, turn_penalty);
    return final_path;
    
}


