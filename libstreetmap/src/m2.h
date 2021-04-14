/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2.h
 * Author: mazizhen
 *
 * Created on February 19, 2018, 4:49 PM
 */

#pragma once //protects against multiple inclusions of this header file
#include "StreetsDatabaseAPI.h"
#include <string>
#include <vector>
#include "graphics.h"
#define EDT -4
#define EET +2
#define IST +5
#define CST -5

void draw_map(); //main draw_map function
void drawSubway();
void display_POI(void(*drawscreen_ptr)(void));
void display_feat(void(*drawscreen_ptr)(void));
void display_Black(void(*drawscreen_ptr)(void));
void draw_screen(); //initialize the screen
void Search_intersections(void(*drawscreen_ptr)(void));// create search button, input and output found in terminal
void act_on_mouse_move(float x, float y);
void act_on_button_press(float x, float y, t_event_buttonPressed event);
void drawSegByID(unsigned street_segment_id,t_bound_box x); //draw each street segment
void drawAllInt(t_bound_box x);// drawing intersections 
void drawStreetByID(StreetSegmentIndex street_segment_id);

void drawAllSegs(t_bound_box x);
void checkHistory();// takes the number to display the most recently search streets
void search_History(void(*drawscreen_ptr)(void));
void drawFeatureByID(FeatureIndex ID, t_bound_box x);
void draw_found_feature();

void drawAllFeatures(t_bound_box x);
void load_another_map(void(*drawscreen_ptr)(void));


void drawLineTPoint(t_point A, t_point B,StreetSegmentInfo x);
void drawLineTPoint_f(t_point A, t_point B, FeatureIndex ID);
void draw_seg(StreetSegmentInfo thisSegment, StreetSegmentIndex street_segment_id);

void drawAllPoi(t_bound_box x);
void drawSpecificPoi(t_bound_box x);
LatLon Search_POI();
void Search_POI(void(*drawscreen_ptr)(void));
void help(void(*drawscreen_ptr)(void));
void drawAllMyFav();
void add_favourite(void(*drawscreen_ptr)(void));
void ask_fav();
void drawTextTpoint(t_point A, t_point B, StreetSegmentInfo x);
void drawFeature_name(FeatureIndex ID);

void setClr();
void act_on_key_press(char x, int keysym);
float LatToCar(double latY);
float LonToCar(double lonX);
float CarxToLon(float X);
float CaryToLat(float Y);
void draw_closest_POI_and_Int(t_bound_box x);
LatLon Search();
 void draw_int();
 void drawTextTpoint_f(t_point A, t_point B, FeatureIndex ID);
void draw_name_closed_poly(t_point polypts, unsigned num, FeatureIndex ID);
void Search_Feature(void(*drawscreen_ptr)(void));
void drawInstruction();
void drawFoundPath(const std::vector<unsigned>& segID);
void display_legend(void(*drawscreen_ptr)(void));
extern t_color SeaClr;
extern t_color BuildingClr;
extern t_color StreamClr;
extern t_color GreenClr;
extern t_color ParkClr;
extern t_color BeachClr;
extern t_color HWClr;
extern t_color RoadClr;
void draw_found_POI();
void Search_street_b(void(*drawscreen_ptr)(void));
LatLon Search_street();
 void draw_found_street();
 void drawWalkingDistance();
 void drawclock();
 std::string timeZone(std::string name);
 void google(std::string first);
// void google_street(std::string first) 
 void draw_found_intersection_path();



