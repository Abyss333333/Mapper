/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <random>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"


std::vector<std::string>eachName;
std::vector<unsigned> retID;

using ece297test::relative_error;

struct MapFixture {

    MapFixture() {
        //Load the map
        load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
    }

    ~MapFixture() {
        //Clean-up
        close_map();
    }
};

TEST_FIXTURE(MapFixture, find_street_ids_from_name) {
    ECE297_TIME_CONSTRAINT(1);

    unsigned maxStreetID = getNumberOfStreets();


    bool passAll = 1;

    for (int i = 0; i < 4000; i++) {

        unsigned stID = rand() % maxStreetID; //street id for this cycle
        std::string relatedName = getStreetName(stID);

        bool pass = 0;

       retID = find_street_ids_from_name(relatedName);

        //for (std::vector<unsigned>::iterator it = retID.begin(); it != retID.end(); ++it) {
        for (unsigned i = 0; i<retID.size();i++){
            if (retID[i] == stID) {
                pass = 1;
                break;
            }

        }
        if (pass == 0) {
            std::cout << "error @ ID " << stID << std::endl;
            passAll = 0;
        }




    }
    //UnitTest::TimeHelpers::SleepMs(50);
    CHECK_EQUAL(passAll,1);
}
