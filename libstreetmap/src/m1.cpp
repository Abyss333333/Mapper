/* 
 * Copyright 2018 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "m1.h"
#include "m2.h"
#include "StreetsDatabaseAPI.h"
#include <unordered_map>
#include <math.h>
#include "Map.h"
#include "easygl/graphics.h"
#include "mapConnect.h"
Map * map;


bool load_map(std::string map_name) {
    bool load_successful = false;

    load_successful = loadStreetsDatabaseBIN(map_name); //Indicates whether the map has loaded 
    if (load_successful == true) {
        
        map= new Map;
        return true;
    }

    return false;
}

void close_map() {
    //Clean-up your map related data structures here
    
    delete map;
    closeStreetDatabase();

}





//Returns the street segments for the given intersection 

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {

    return map->get_intersection_street_segment(intersection_id);
}
//Returns the street names for the given intersection 
std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {
    std::vector<unsigned> street_segments;
    street_segments = map->get_intersection_street_segment(intersection_id);
    StreetSegmentInfo Seg_Info;
    std::vector<std::string> names;
    for (std::size_t i = 0; i < street_segments.size(); i++) {
        Seg_Info = map->get_Seg_Info_v(street_segments[i]);
        unsigned x = Seg_Info.streetID;
        names.push_back(getStreetName(x));
    }
    return names;
}

/*std::vector<unsigned> find_intersection_street_ids(unsigned intersection_id) {
    std::vector<unsigned> street_segments;
    street_segments = map->get_intersection_street_segment(intersection_id);
    StreetSegmentInfo Seg_Info;
    std::vector<unsigned > ids;
    for (std::size_t i = 0; i < street_segments.size(); i++) {
        Seg_Info = map->get_Seg_Info_v(street_segments[i]);
        unsigned x = Seg_Info.streetID;
        ids.push_back(x);
    }
    return ids;
}*/
//Returns street id(s) for the given street name
//If no street with this name exists, returns a 0-length vector.
std::vector<unsigned> find_street_ids_from_name(std::string street_name) {

    std::vector<unsigned> street_ids = map->get_street_ids_from_names(street_name);



    return street_ids;
}





//Returns true if you can get from intersection1 to intersection2 using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {
    std::vector<unsigned> streets_connected_to_id1;
    unsigned i;
    streets_connected_to_id1 = map->get_intersection_street_segment(intersection_id1);
    StreetSegmentInfo Seg_Info;


    if (intersection_id1 == intersection_id2) { // if the two intersections are the same
        return true;
    }
    for (i = 0; i < streets_connected_to_id1.size(); i++) {
        Seg_Info = map->get_Seg_Info_v(streets_connected_to_id1[i]);
        if (Seg_Info.from == intersection_id1 && Seg_Info.to == intersection_id2) // checking if they have similar intersections from and to
            return true;

        else if (Seg_Info.to == intersection_id1 && Seg_Info.from == intersection_id2 && Seg_Info.oneWay == false) // checking the opposite order and wheether its one way or not
            return true;

    }
    return false;
}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections

std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    
    std::vector<unsigned> streets_connected_to_id;
    unsigned i;
    streets_connected_to_id = map->get_intersection_street_segment(intersection_id);
    StreetSegmentInfo Seg_Info;
    std::vector<unsigned> adj_int_id;
    
    for (i = 0; i < streets_connected_to_id.size(); ++i) {
        Seg_Info = map->get_Seg_Info_v(streets_connected_to_id[i]);
        if (Seg_Info.oneWay == false) { // if its not one way
            if (Seg_Info.from == intersection_id) {
                adj_int_id.push_back(Seg_Info.to); 
            } else {
                adj_int_id.push_back(Seg_Info.from);
            }
        } else if (Seg_Info.oneWay == true) { // if it is one way

            if (Seg_Info.from == intersection_id) {
                adj_int_id. push_back(Seg_Info.to);
            }
        }
    }

    // get unique vector
    std::vector < unsigned> unique = map->unique_vector(adj_int_id);
    
    return unique;
    
}


//Returns all street segments for the given street

std::vector<unsigned> find_street_street_segments(unsigned street_id) {

    return map->get_street_street_segment(street_id);
    
    

}


//Returns all intersections along the a given street

std::vector<unsigned> find_all_street_intersections(unsigned street_id) {

    std::vector<unsigned> street_segments = map->get_street_street_segment(street_id); // using direct access to container in class
    unsigned i;
    StreetSegmentInfo Seg_Info;
    std::vector<unsigned> street_intersections;
    
    for (i = 0; i < street_segments.size(); i++) {
        Seg_Info = map->get_Seg_Info_v(street_segments[i]);
        street_intersections.push_back(Seg_Info.from);
        street_intersections.push_back(Seg_Info.to);
    }
    std::vector < unsigned> unique= map->unique_vector(street_intersections); // getting rid of duplicates in the vector
   
    return unique;


}





//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guarenteed to be unique, so more than 1 intersection id
//may exist

std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1,
        std::string street_name2) {
    
    // getting all intersection for both street names
    std::vector<unsigned> street_id_1 = map->get_street_inter_id(street_name1); 
    std::vector<unsigned> street_id_2 = map->get_street_inter_id(street_name2);
    // getting the unique ids that exist in both
    std::vector<unsigned> ret = map->unique_vector_from_two_vectors(street_id_1,street_id_2);
    return ret;
}





//Returns the distance between two coordinates in meters

double find_distance_between_two_points(LatLon point1, LatLon point2) {

    
    double x = map->distance_calc(point1, point2); //distance
    return x;
}

//Returns the length of the given street segment in meters

double find_street_segment_length(unsigned street_segment_id) {
   
    
    double x = map->get_street_seg_length(street_segment_id);
    return x;
}

//Returns the length of the specified street in meters

double find_street_length(unsigned street_id) {

    std::vector<unsigned> AllSegmentsOnStreet;
    double LengthTotal = 0;
    AllSegmentsOnStreet = map->get_street_street_segment(street_id);
    for (size_t i = 0; i < AllSegmentsOnStreet.size(); i++) {
        LengthTotal += find_street_segment_length(AllSegmentsOnStreet[i]);

    }


    return LengthTotal; 

}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)

double find_street_segment_travel_time(unsigned street_segment_id) {
  
    
    return map->get_travel (street_segment_id);
}

//Returns the nearest point of interest to the given position

unsigned find_closest_point_of_interest(LatLon my_position) {


    double MinDist = 1114000;// a large distance to begin with
    POIIndex minID = 0;
    unsigned POI = map->get_number_of_POI();
    double curDis;
    //initial area of searching
    double leftLon = my_position.lon() - 8; 
    double rightLon = my_position.lon() + 8;
    double upLat = my_position.lat() + 10;
    double downLat = my_position.lat() - 10;
    //change distance into lat/lon
    double sub = my_position.lat() * DEG_TO_RAD;
    double cosL = cos(sub) * DEG_TO_RAD;

    //int much = 0;

    for (unsigned int i = 0; i < POI ; i++) {

        LatLon p = map->get_LatLon_P(i);

        //skip distance calculation if it`s out of range
        if (p.lat() < upLat && p.lat() > downLat && p.lon() < rightLon && p.lon() > leftLon) {


            //make new region based on new min distance
            curDis = map->distance_calc(p, my_position);
            if (curDis <= MinDist) {
                MinDist = curDis;
                minID = i;
                //make a square of curDis using in degree
                curDis = curDis / EARTH_RADIUS_IN_METERS;

                leftLon = my_position.lon() - (curDis / cosL);
                rightLon = my_position.lon() + (curDis / cosL);
                upLat = my_position.lat() + curDis / DEG_TO_RAD;
                downLat = my_position.lat() - curDis / DEG_TO_RAD;

            }
        }//Lat satisfy

    }

    return minID;
}


    
//Returns the the nearest intersection to the given position

unsigned find_closest_intersection(LatLon my_position) {
    
    
     double MinDist = 111400000;
    unsigned  minID = 0;
    unsigned intersections = map->get_number_of_intersections();
    double curDis;
    //for calculation
    double sub = my_position.lat() * DEG_TO_RAD;
    double cosL = cos(sub) * DEG_TO_RAD;

    double noIntFound = intersections + 100; //no int found if this returns
    minID = noIntFound;
    //+-5KM, Search first
    double searchRange = 50000/EARTH_RADIUS_IN_METERS;
    double leftLon = my_position.lon() - (searchRange / cosL);
    double rightLon = my_position.lon() + (searchRange/ cosL);
    double upLat = my_position.lat() + searchRange / DEG_TO_RAD;
    double downLat = my_position.lat() - searchRange / DEG_TO_RAD;

    int range = 0; //0=+-5km,1=degree
    while (range < 2) {


        for (unsigned int i = 0; i < intersections ; i++) {

            LatLon p = map->get_Lat_Lon_i(i);


            //skip distance calculation if it`s out of range
            if (p.lat() < upLat && p.lat() > downLat ) {
                if(p.lon() < rightLon && p.lon() > leftLon){
                //set new distance if closer intersection found
                curDis = map->distance_calc(p, my_position);
                if (curDis <= MinDist) {
                    MinDist = curDis;
                    minID = i;
                    //make a square of curDis using in degree
                    curDis = curDis / EARTH_RADIUS_IN_METERS;

                    leftLon = my_position.lon() - (curDis / cosL);
                    rightLon = my_position.lon() + (curDis / cosL);
                    upLat = my_position.lat() + curDis / DEG_TO_RAD;
                    downLat = my_position.lat() - curDis / DEG_TO_RAD;

                }
            }
            }//Lat satisfy

        }//end of for loop

        if (minID == noIntFound) { //expand range if no intersect found
            leftLon = my_position.lon() - 8; //expand to larger range
            rightLon = my_position.lon() + 8;
            upLat = my_position.lat() + 10;
            downLat = my_position.lat() - 10;
            range++;
        } else {
            break;
        }

    }//end of while loop

    return minID;
}

