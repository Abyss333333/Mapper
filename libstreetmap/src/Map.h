/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.h
 * Author: siddi558
 *
 * Created on February 4, 2018, 5:26 AM
 */

#ifndef MAP_H
#define MAP_H

#include "OSMDatabaseAPI.h"
#include "StreetsDatabaseAPI.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "m1.h"
#include "math.h"
#include "graphics.h"
#include <map>
#include "OSMDatabaseAPI.h"
#include  <utility>



struct Node_{
    Node_ * parent_node ; // holds parent node.
    double travel_time_from_start; // holds distance from start
    unsigned parent_street_id;
    unsigned id;
    unsigned prev_edge;
    
    
    } ;
    typedef Node_ Node;
    
    struct Edge_{
        unsigned street_seg_id;
        Node * prev_node;
        Node * to_node;
        double travel_time;
    };
    typedef Edge_ Edge;
    
class Map {
private:
    std::vector<LatLon>Lat_Lon_p; // vector holding LatLons of all points of Interest
    std::vector< double> street_seg_length; // indexed with street segment id, holds all length for said segment 
    std::vector<double> travel; // vector indexed with street segment and holds travel time for said segment
    std::vector< std::vector<unsigned> > intersection_street_segment; // vector indexed with intersection. holds all street segments to that intersection
    std::vector<LatLon>Lat_Lon_i; // vector holding LatLons of all intersections
    std::vector<StreetSegmentInfo> Seg_Info_v; // vector holding  Structure StreetSegmentInfo for all street segments
    // unordered map indexed with street id, holds all street segment of that street
    std::unordered_map< unsigned, std::vector<unsigned> > street_street_segment;
    std::unordered_map <std::string, std::vector<unsigned>> street_name_street_segment; // access all street segments with names
    unsigned number_of_intersections, number_of_features;
    unsigned number_of_POI;
    
    // unordered map with key "Street Name". holds street ids related to the name
    std::unordered_map< std::string, std::vector<unsigned> >street_ids_from_names;
    // unordered map indexed with street name, holds vector of all intersection in street name.
    std::unordered_map< std::string, std::vector<unsigned> > street_inter_id;
    //unordered map to save the users favorite locations
    std::unordered_map <std::string, unsigned> feature_id_from_name;
    
    std::vector<std::string> search_history; //stores the what the user inputs 
    
    std::string found_intersection;
   // std::vector<std::string> intersection_name;
    std::vector<std::string> feature_name;
    std::vector<double>street_length;
    std::vector<unsigned> feature_pointnum;
    std::vector<std::vector<LatLon>> feature_points;
    std::vector<std::vector<t_point>> curve_seg_points; //curved points on 
    std::vector<t_point> seg_from;
    std::vector<t_point> seg_to;
    std::vector<unsigned>seg_curve_num;
    
    std::vector<std::string>POI_type;
    std::vector<unsigned> type_education;
    std::vector<unsigned> type_food;
    std::vector<unsigned> type_transport;
    std::vector<unsigned> type_med;
    std::vector<unsigned> type_emergency;
    std::vector<unsigned> type_fun;
    
    bool POI_Click;
    bool feat_Click;
    bool int_Click;
    bool mouse_click;
    bool mouse_click2;
    bool search_click;
    bool legend_click;
    bool help_click;
    bool ed_click;
    bool food_click;
    bool trans_click;
    bool med_click;
    bool emergency_click;
    bool fun_click;
    bool  Fav_click;
    bool search_history_click;
    bool load_another_map;//
    bool sub_click;
    bool search_f;
    
    float mouse_click_x;
    float mouse_click_y;

    float mouse_click_x2;
    float mouse_click_y2;

    float mouse_move_x;
    float mouse_move_y;


    LatLon temporary;
    bool draw_int;
    bool draw_found_POI;
    bool draw_found_street;
    bool search_POI;
    bool search_street;

    std::vector <unsigned> found_POI_id;
    std::vector <unsigned> found_street_segment_ids;
    
    
    
    std::vector<LatLon> OSM_vector;
    
    
    
public: 
    
    Map();
    ~Map();
    unsigned max_speed;
    void OSM_database();
      std::vector<std::string> ask_fav_name;
    std::vector<t_point> ask_fav_loc;
    std::multimap <std::pair<unsigned,unsigned>, double> from_to_int_travel_time; // store the from intersection and to interseection as a pair and returns the travel time between
    std::multimap <std::pair<unsigned,unsigned>, unsigned>from_to_int_street_id;
    std::multimap <std::pair<unsigned,unsigned>, unsigned>from_to_int_street_seg_id;
     std::map <std::pair<unsigned,unsigned>, unsigned>from_to_int_street_seg_id1;
    void make_from_to();
    
    std::vector<unsigned> point;
    std::vector<unsigned> pathll;
    
    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> edges;
    
    void make_nodes_and_edges();
    std::vector<std::string> l;
    LatLon path_poi;
    unsigned number_of_segs;
    bool second_bar = false;
    std::string k;
    std::vector<std::string> POI_names;
   std::unordered_map<std::string, std::vector<unsigned>> POI_id_from_name;
    std::vector<std::vector<unsigned> > adj_int;
    std::vector<std::vector<unsigned> > adj_int_opp;
    void draw_screen();
    void draw_map();
    std::unordered_map<std::string, unsigned> intersection_id_from_names;
    std::vector<std::string> intersection_name;
    void clear_structure();
    void make_adjacent_intersections();
    
    bool get_POI_click();
    void set_POI_click(bool x);
    unsigned number_of_intersectionss;
    std::vector<bool>closed_list ;
    std::vector<bool> open_list ;
    std::vector<unsigned> parent_node;  // holds parent node.
    std::vector<unsigned> travel_time_from_start; // holds distance from start
    std::vector<unsigned> parent_street_id ;
    std::vector<unsigned>f_node;

    std::vector<std::string> get_ask_fav_name();
    void set_ask_fav_name(std::string x);
    
    std::vector<t_point> get_ask_fav_loc();
    void set_ask_fav_loc(t_point x);
    
    void set_found_POI_id(std::vector <unsigned> x);
    std::vector<unsigned> get_found_POI_id();

    void set_found_street_id(std::vector <unsigned> x);
    std::vector<unsigned> get_found_street_id();

    bool get_legend_click();
    void set_legend_click(bool x);
    
    bool get_help_click();
    void set_help_click(bool x);

    bool get_ed_click();
    void set_ed_click(bool x);

    bool get_fun_click();
    void set_fun_click(bool x);

    bool get_sub_click();
    void set_sub_click(bool x);
    
    bool get_trans_click();
    void set_trans_click(bool x);

    bool get_food_click();
    void set_food_click(bool x);

    bool get_med_click();
    void set_med_click(bool x);

    bool get_emergency_click();
    void set_emergency_click(bool x);

    bool get_Fav_click();
    void set_Fav_click(bool x);

    void set_draw_int(bool x);
    bool get_draw_int();

    bool get_search_click();
    void set_search_click(bool x);
    
    bool get_search_f();
    void set_search_f(bool x);

    bool get_search_history_click();
    void set_search_history_click(bool x);
    
    unsigned get_feature_id_from_name(std::string s);


    bool get_search_POI();
    void set_search_POI(bool x);

    bool get_search_street();
    void set_search_street(bool x);

    bool get_mouse_click();
    void set_mouse_click(bool x);

    void set_draw_found_POI(bool x);
    bool get_draw_found_POI();

    void set_draw_found_street(bool x);
    bool get_draw_found_street();

    void set_temp_LatLon(LatLon x);
    LatLon get_temp_Latlon();

    bool get_mouse_click2();
    void set_mouse_click2(bool x);

    bool get_int_click();
    void set_int_click(bool x);

     
    
    bool get_feat_click();
    void set_feat_click(bool x);

    float get_mouse_click_x();
    void set_mouse_click_x(float x);

    float get_mouse_move_x();
    void set_mouse_move_x(float x);

    float get_mouse_move_y();
    void set_mouse_move_y(float y);


    float get_mouse_click_y();
    void set_mouse_click_y(float y);

    float get_mouse_click_x2();
    void set_mouse_click_x2(float x);

    float get_mouse_click_y2();
    void set_mouse_click_y2(float y);

    void set_found_intersection(std::string x);
    std::string get_found_intersection();


    double distance_calc(LatLon x, LatLon y);

    void make_number_of_intersections_and_POI();
    void make_street_length_fromID();
    unsigned get_number_of_intersections();
    unsigned get_number_of_POI();
    unsigned get_number_of_features();

    std::vector<unsigned> unique_vector(std::vector<unsigned> temp);
    std::vector<unsigned> unique_vector_from_two_vectors(std::vector<unsigned> temp, std::vector<unsigned> temp2);

    void make_LatLon_P();
    LatLon get_LatLon_P(unsigned i);

    void make_street_seg_length_and_travel();
    double get_street_seg_length(unsigned i);
    double get_travel(unsigned i);

    void make_intersection_street_segment_and_Lat_Lon_i();
    LatLon get_Lat_Lon_i(unsigned i); //intersection LATLON
    std::vector<unsigned> get_intersection_street_segment(unsigned i);

    void make_street_segment_and_Seg_Info();
    StreetSegmentInfo get_Seg_Info_v(unsigned i);
    std::vector<unsigned> get_street_street_segment(unsigned i);
    std::vector<unsigned> get_street_name_street_segment(std::string i);
    void make_street_ids_from_names();
    std::vector<unsigned> get_street_ids_from_names(std::string name);
    
    
   
    void make_intersection_names();
    std::string get_intersection_name(unsigned i);
    std::vector <std::string> get_search_histor();
    void set_search_histor(std::string data);
    void set_clear_history();
    void make_feature_name();
    std::string get_feature_name(unsigned i);
    std::vector<std::string> get_feature_name_vector();



    void make_POI_name();
    std::string get_POI_name(unsigned i);
    std::vector<unsigned> get_POI_id_from_name(std::string x);

    void make_POI_type();
    std::string get_POI_type(unsigned i);
    std::vector<unsigned> get_type_education();
    std::vector<unsigned> get_type_food();
    std::vector<unsigned> get_type_trans();
    std::vector<unsigned> get_type_med();
    std::vector<unsigned> get_type_emergency();
    std::vector<unsigned> get_type_fun();
    void drawSpecificPoi(t_bound_box x);


    void make_street_inter_id();
    std::vector<unsigned> get_street_inter_id(std::string name);

    void make_feature_num_and_points();
    LatLon get_feature_points(unsigned i, unsigned j);
    unsigned get_feature_pointnum(unsigned j);

    void make_streetseg_position();
    t_point get_seg_curve_points(StreetSegmentIndex ID, unsigned c);
    unsigned get_seg_curve_num(unsigned c);
    t_point get_seg_from(unsigned id);
    t_point get_seg_to(unsigned id);
    
    
    
    void make_street_length();
    double get_street_length(unsigned i);
    
    bool get_load_another_map();
    void set_load_another_map(bool x);

    std::vector<LatLon> get_OSM_vector();
    
    std::string s;
    std::vector<std::string> part;

};

extern float LatAvgMap; //defined in M2.cpp  draw_map

#endif /* MAP_H */

