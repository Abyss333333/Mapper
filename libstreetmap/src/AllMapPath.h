/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include <string>
/* 
 * File:   AllMapPath.h
 * Author: mazizhen
 *
 * Created on February 23, 2018, 3:56 PM
 */

#ifndef ALLMAPPATH_H
#define ALLMAPPATH_H

std::string default_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
std::string Toronto_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
std::string Beijing_map_path = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
std::string Cairo_map_path = "/cad2/ece297s/public/maps/cairo_egypt.streets.bin";
std::string Capetown_map_path = "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin";
std::string GoldenHorseshoe_map_path = "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin";
std::string Hamilton_map_path = "/cad2/ece297s/public/maps/hamilton_canada.streets.bin";
std::string Hongkong_map_path = "/cad2/ece297s/public/maps/hong-kong_china.streets.bin";
std::string Iceland_map_path = "/cad2/ece297s/public/maps/iceland.streets.bin";
std::string Interlaken_map_path = "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin";
std::string London_map_path = "/cad2/ece297s/public/maps/london_england.streets.bin";
std::string Moscow_map_path = "/cad2/ece297s/public/maps/moscow_russia.streets.bin";
std::string NewDelhi_map_path = "/cad2/ece297s/public/maps/new-delhi_india.streets.bin";
std::string NewYork_map_path = "/cad2/ece297s/public/maps/new-york_usa.streets.bin";
std::string RioDeJaneiro_map_path = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin";
std::string SaintHelena_map_path = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
std::string Singapore_map_path = "/cad2/ece297s/public/maps/singapore.streets.bin";
std::string Sydney_map_path = "/cad2/ece297s/public/maps/sydney_australia.streets.bin";
std::string Tehran_map_path = "/cad2/ece297s/public/maps/tehran_iran.streets.bin";
std::string Tokyo_map_path = "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";

std::string Toronto_osm_path = "/cad2/ece297s/public/maps/toronto_canada.osm.bin";
std::string Beijing_osm_path = "/cad2/ece297s/public/maps/beijing_china.osm.bin";
std::string Cairo_osm_path = "/cad2/ece297s/public/maps/cairo_egypt.osm.bin";
std::string Capetown_osm_path = "/cad2/ece297s/public/maps/cape-town_south-africa.osm.bin";
std::string GoldenHorseshoe_osm_path = "/cad2/ece297s/public/maps/golden-horseshoe_canada.osm.bin";
std::string Hamilton_osm_path = "/cad2/ece297s/public/maps/hamilton_canada.osm.bin";
std::string Hongkong_osm_path = "/cad2/ece297s/public/maps/hong-kong_china.osm.bin";
std::string Iceland_osm_path = "/cad2/ece297s/public/maps/iceland.osm.bin";
std::string Interlaken_osm_path = "/cad2/ece297s/public/maps/interlaken_switzerland.osm.bin";
std::string London_osm_path = "/cad2/ece297s/public/maps/london_england.osm.bin";
std::string Moscow_osm_path = "/cad2/ece297s/public/maps/moscow_russia.osm.bin";
std::string NewDelhi_osm_path = "/cad2/ece297s/public/maps/new-delhi_india.osm.bin";
std::string NewYork_osm_path = "/cad2/ece297s/public/maps/new-york_usa.osm.bin";
std::string RioDeJaneiro_osm_path = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.osm.bin";
std::string SaintHelena_osm_path = "/cad2/ece297s/public/maps/saint-helena.osm.bin";
std::string Singapore_osm_path = "/cad2/ece297s/public/maps/singapore.osm.bin";
std::string Sydney_osm_path = "/cad2/ece297s/public/maps/sydney_australia.osm.bin";
std::string Tehran_osm_path = "/cad2/ece297s/public/maps/tehran_iran.osm.bin";
std::string Tokyo_osm_path = "/cad2/ece297s/public/maps/tokyo_japan.osm.bin";

std::string mapnametopath(std::string * osmref) {
    std::string thename;
    
    std::string osmpath;
    std::cout << "please enter the region name or datapath" << std::endl;
    while (1) {

        getline(std::cin, thename);
      

        if (thename == "Toronto" ||thename == Toronto_map_path) {
            *osmref = Toronto_osm_path;
            return Toronto_map_path;
        }
        if (thename == "Beijing"||thename == Beijing_map_path) {
            *osmref = Beijing_osm_path;
            return Beijing_map_path;
        }
        if (thename == "Cairo"||thename ==Cairo_map_path) {
            *osmref =  Cairo_osm_path;          
            return Cairo_map_path;
        }
        if (thename == "Capetown"||thename ==Capetown_map_path) {
            *osmref = Capetown_osm_path;
            return Capetown_map_path;
        }
        if (thename == "GoldenHorseshoe"||thename ==GoldenHorseshoe_map_path) {
            *osmref = GoldenHorseshoe_osm_path;
            return GoldenHorseshoe_map_path;
        }
        if (thename == "Hamilton"||thename ==Hamilton_map_path) {
            *osmref =  Hamilton_osm_path;          
            return Hamilton_map_path;
        }
        if (thename == "Hongkong"||thename ==Hongkong_map_path) {
            *osmref = Hongkong_osm_path;
            return Hongkong_map_path;
        }
        if (thename == "Iceland"||thename ==Iceland_map_path) {
            *osmref = Iceland_osm_path;
            return Iceland_map_path;
        }
        if (thename == "Interlaken"||thename ==Interlaken_map_path) {
            *osmref = Interlaken_osm_path;
            return Interlaken_map_path;
        }
        if (thename == "London"||thename ==London_map_path) {
            *osmref = London_osm_path;
            return London_map_path;
        }
        if (thename == "Moscow"||thename ==Moscow_map_path) {
            *osmref = Moscow_osm_path;
            return Moscow_map_path;
        }
        if (thename == "NewDelhi"||thename ==NewDelhi_map_path) {
            *osmref = NewDelhi_osm_path;
            return NewDelhi_map_path;
        }
        if (thename == "NewYork"||thename ==NewYork_map_path) {
            *osmref = NewYork_osm_path;
            return NewYork_map_path;
        }
        if (thename == "RioDeJaneiro"||thename ==RioDeJaneiro_map_path) {
            *osmref = RioDeJaneiro_osm_path;
            return RioDeJaneiro_map_path;
        }
        if (thename == "SaintHelena"||thename ==SaintHelena_map_path) {
            *osmref = SaintHelena_osm_path;
            return SaintHelena_map_path;
        }
        if (thename == "Singapore"||thename ==Singapore_map_path) {
            *osmref = Singapore_osm_path;
            return Singapore_map_path;
        }
        if (thename == "Sydney"||thename ==Sydney_map_path) {
            *osmref = Sydney_osm_path;
            return Sydney_map_path;
        }
        if (thename == "Tehran"||thename ==Tehran_map_path) {
            *osmref = Tehran_osm_path;
            return Tehran_map_path;
        }
        if (thename == "Tokyo"||thename ==Tokyo_map_path) {
            *osmref = Tokyo_osm_path;
            return Tokyo_map_path;
        }
        std::cout<<"please enter the OSM library path as well"<<std::endl;
        std::cin.ignore();
        osmpath = std::cin.get();
        *osmref = osmpath;
        return thename;
        
    }

}


#endif /* ALLMAPPATH_H */

