/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "m4.h"
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
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
#include <list>
#include <queue>
#include <thread>
#include "m3.h"
#include <time.h>

//declaration of sub-function
double make_Heuristic_calc(unsigned curr_vertex, unsigned id_end, int SpdStandard);
double make_Manhattan_calc(unsigned i, unsigned j);
std::vector<unsigned>pathSwap(std::vector<unsigned>&intersectionPath, const std::vector<DeliveryInfo>& deliveries, std::vector<unsigned >& related_del_id);


std::vector<unsigned> traveling_courier_given_depot(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty, const unsigned givenDepotIndex);

void two_opt(std::vector<unsigned>& list,
        std::unordered_map<unsigned, std::vector<unsigned> >& pickup_d,
        std::unordered_map<unsigned, unsigned> &list_d,
        std::vector<unsigned>&d_list, double current_min_time, const float turn_penalty,
        std::unordered_map <unsigned, std::vector<unsigned> > &deliveries_,
        std::vector<unsigned> p_list,
        std::unordered_map <unsigned, unsigned> &list_p);

void three_opt(std::vector<unsigned>& list,
        std::unordered_map<unsigned, std::vector<unsigned> >& pickup_d,
        std::unordered_map<unsigned, unsigned> &list_d,
        std::vector<unsigned>&d_list, double &current_min_time, const float turn_penalty,
        std::unordered_map <unsigned, std::vector<unsigned> > &deliveries_,
        std::vector<unsigned> p_list,
        std::unordered_map <unsigned, unsigned> &list_p);
//main 


std::vector<unsigned> traveling_courier5(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty, int timestart) {


    const unsigned NumDepots = depots.size(); //totalnum of depots
    std::vector<unsigned>currentBestPath;
    double currentBestTravelTime = INFINITY;

    //search for best Depot to start with
    for (unsigned DepotIndex = 0; DepotIndex < NumDepots; DepotIndex++) {
        std::vector<unsigned>returnedPath;
        returnedPath = traveling_courier_given_depot(deliveries, depots, turn_penalty, DepotIndex);
        if (returnedPath.size() == 0) {//if this path is invalid
            continue;
        }
        double returnedTravelTime = compute_path_travel_time(returnedPath, turn_penalty);
        if (returnedTravelTime < currentBestTravelTime) {
            currentBestTravelTime = returnedTravelTime;
            currentBestPath = returnedPath;

        }

        int timeEnd = clock();
        double timediff = (timeEnd - timestart) / CLOCKS_PER_SEC;

        if (timediff > 25) {
            break;
        }

    }
    return currentBestPath;
}

std::vector<unsigned> traveling_courier_given_depot(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty,
        const unsigned givenDepotIndex) { //sub-Main M4 FUNC

    std::vector<unsigned>travel_Path; //The Final Path to return
    const unsigned NumDeliv = deliveries.size(); //total num of Deliveries
    const unsigned NumDepots = depots.size(); //totalnum of depots


    //=============Greedy + PICKUP========================

    std::vector<unsigned>intersectionPath; //path of intersections to traverse
    std::vector<unsigned > related_del_id;

    unsigned delivStatus[NumDeliv]; //0=untouched, 1=pickedup, 2=dropped.
    for (unsigned i = 0; i < NumDeliv; i++) {
        delivStatus[i] = 0; //initialize array above to "untouched"
    }

    //now choose a depot to start with

    unsigned chosenDepot = depots[givenDepotIndex];
    unsigned bestStartpickup;
    double shortestPickupDist = 99999999;
    LatLon depotLL = map->get_Lat_Lon_i(chosenDepot);

    for (unsigned ipick = 0; ipick < NumDeliv; ipick++) {//find the starting pickup (closest to depot)
        LatLon intLL = map->get_Lat_Lon_i(deliveries[ipick].pickUp);
        double distToDepot = find_distance_between_two_points(depotLL, intLL);

        if (distToDepot < shortestPickupDist) {
            bestStartpickup = ipick;
            shortestPickupDist = distToDepot;

        }
    }
    //std::cout << "start with:" << bestStartpickup << std::endl;
    delivStatus[bestStartpickup] = 1; //best starting pickup found
    travel_Path = find_path_between_intersections(chosenDepot, deliveries[bestStartpickup].pickUp, turn_penalty); //push in the first path
    intersectionPath.push_back(deliveries[bestStartpickup].pickUp); //insert to first Int list
    //check validity of this depot
    if (travel_Path.size() == 0) {
        return travel_Path;
    }

    //now search a valid path of intersections
    while (1) {
        bool foundSth = 0; //set to one if a valid point was found, else brake out of loop
        double ClosestDist = 99999999;
        unsigned ClosestInt;
        unsigned ClosestIntArrayIndex;

        for (unsigned i = 0; i < NumDeliv; i++) {
            if (delivStatus[i] == 0) {//not picked up yet
                LatLon intLL = map->get_Lat_Lon_i(deliveries[i].pickUp);
                LatLon prevIntLL = map->get_Lat_Lon_i(intersectionPath.back());
                double currDist = find_distance_between_two_points(prevIntLL, intLL); //previous int to this int distance

                if (currDist < ClosestDist) {
                    ClosestDist = currDist;
                    ClosestInt = deliveries[i].pickUp;
                    ClosestIntArrayIndex = i;
                    foundSth = 1;
                }
            } else if (delivStatus[i] == 1) {//picked up but not delivered
                LatLon intLL = map->get_Lat_Lon_i(deliveries[i].dropOff);
                LatLon prevIntLL = map->get_Lat_Lon_i(intersectionPath.back());
                double currDist = find_distance_between_two_points(prevIntLL, intLL); //previous int to this int distance

                if (currDist < ClosestDist) {
                    ClosestDist = currDist;
                    ClosestInt = deliveries[i].dropOff;
                    ClosestIntArrayIndex = i;
                    foundSth = 1;
                }
            }



        }//end of traversing all deliveries for each

        if (foundSth == 0) {//nothing to be found, then get out of loop
            break;
        }

        intersectionPath.push_back(ClosestInt); //push it into the list
        related_del_id.push_back(ClosestIntArrayIndex); //its related index

        if (delivStatus[ClosestIntArrayIndex] == 0) {
            delivStatus[ClosestIntArrayIndex] = 1; //set to "picked up"
        } else {
            delivStatus[ClosestIntArrayIndex] = 2;
        }
    }//end of int path finding While loop



    //now a vector of intersection was made. Traverse through and make path
    unsigned totalIntNum = intersectionPath.size();

    //pathSwap(intersectionPath, deliveries, related_del_id);


    for (unsigned i = 0; i < (totalIntNum - 1); i++) {
        std::vector<unsigned>currentPath = find_path_between_intersections(intersectionPath[i], intersectionPath[i + 1], turn_penalty);
        travel_Path.insert(travel_Path.end(), currentPath.begin(), currentPath.end());
    }
    // std::vector<unsigned>::iterator; //path of intersections to traverse

    //return to depot
    unsigned nearestReturnIndex = 0;
    unsigned secondnearestReturnIndex;
    float nearestReturnDistance = 99999999;
    LatLon lastintLatLon = map->get_Lat_Lon_i(intersectionPath.back()); //latlon of last intersection
    //find the two closest depots incase one doesn't work.
    for (unsigned iret = 0; iret < NumDepots; iret++) {
        LatLon depotLLret = map->get_Lat_Lon_i(depots[iret]);
        double distfromdepot = find_distance_between_two_points(depotLLret, lastintLatLon);
        if (distfromdepot < nearestReturnDistance) {
            secondnearestReturnIndex = nearestReturnIndex;
            nearestReturnIndex = iret;
            nearestReturnDistance = distfromdepot;
        }


    }
    std::vector<unsigned>returnDepotPath = find_path_between_intersections(intersectionPath.back(), depots[nearestReturnIndex], turn_penalty);
    if (returnDepotPath.size() == 0) {//if closest one doesn`t work
        //std::cout << "into second" << std::endl;
        //std::cout << nearestReturnIndex << "<-nearest further->" << secondnearestReturnIndex << std::endl;
        returnDepotPath = find_path_between_intersections(intersectionPath.back(), depots[secondnearestReturnIndex], turn_penalty);
        if (returnDepotPath.size() == 0) {
            //std::cout << "into third" << std::endl;
            returnDepotPath = find_path_between_intersections(intersectionPath.back(), chosenDepot, turn_penalty);
        }
    }
    travel_Path.insert(travel_Path.end(), returnDepotPath.begin(), returnDepotPath.end());


    //std::iter_swap(travel_Path.begin()+1,travel_Path.end()+2);


    //=============END of Greedy + PICKUP===============

    return travel_Path;
}//end of sun-Main M4 FUNC [traveling_courier_given_depot]

//this function works. put the output on [Main]`s spot

double make_Manhattan_calc(unsigned i, unsigned j) {
    LatLon first = map->get_Lat_Lon_i(i);
    LatLon second = map->get_Lat_Lon_i(j);

    double distance = 0;
    float firsty = LatToCar(first.lat());
    float firstx = LonToCar(first.lon());
    float secondy = LatToCar(second.lat());
    float secondx = LonToCar(second.lon());

    distance = abs(firstx - secondx) + abs(firsty - secondy);
    return distance;
}

double make_Heuristic_calcc(unsigned curr_vertex, unsigned id_end, int SpdStandard) {
    //unsigned totalIntNum= map->get_number_of_intersections();
    LatLon IDLatLon;
    unsigned IDdistance;

    LatLon EndLatLon = map->get_Lat_Lon_i(id_end); //find the latlon of end

    //for(unsigned IDi=0; IDi<totalIntNum-1; IDi++){
    IDLatLon = map->get_Lat_Lon_i(curr_vertex);
    IDdistance = find_distance_between_two_points(IDLatLon, EndLatLon);
    double timeHeuristic = sqrt((IDdistance / SpdStandard)*3.6);
    return timeHeuristic;
    //store the time-to-end for all intersections

    //return 0;
}

std::vector<unsigned>pathSwap(std::vector<unsigned>&intersectionPath, const std::vector<DeliveryInfo>& deliveries, std::vector<unsigned >& related_del_id) {

    double minDis = 0.0;
    double distance;
    unsigned NumDeliv = deliveries.size();
    for (unsigned i = 0; i < (NumDeliv - 2); i++) {
        for (unsigned j = i + 2; j < (NumDeliv - 1); i++) {
            // check if next intersection item has been pick p
            //if (delivStatus[] == 0 

            distance = make_Manhattan_calc(i, j) + make_Manhattan_calc(i + 1, j + 1) - make_Manhattan_calc(i, i + 1) - make_Manhattan_calc(j, j + 1);

            if (0 > distance) {
                // minDis = distance;
                bool check = true; //valid path
                int flag[NumDeliv] = {0};
                for (unsigned k = i + 1; k <= j; k++) {

                    if (flag[related_del_id[k]] == 0) {
                        flag[related_del_id[k]] = 1;
                    } else {
                        check = false;
                        break;
                    }
                }
                if (check == false)
                    continue;
                //now swap

                std::reverse(std::begin(intersectionPath) + i, std::begin(intersectionPath) + j + 1);

            }
        }
    }

}

void add_path(std::vector<unsigned> seg, std::vector<unsigned> & main) {

    for (unsigned i = 0; i < seg.size(); i++) {
        main.push_back(seg[i]);
    }

    return;
}

std::vector<unsigned> traveling_courier3(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty, unsigned x) {
    std::vector<unsigned> list;
    std::unordered_map <unsigned, std::vector<unsigned> > deliveries_;
    std::vector<unsigned> pickup_;
    std::vector<unsigned> go_to_list;
    std::unordered_map <unsigned, bool> reached;

    std::unordered_map<unsigned, std::vector<unsigned>> list_pickup_position_of_d;
    std::unordered_map<unsigned, unsigned> list_position_delivery;
    std::unordered_map<unsigned, unsigned> list_position_pickup;

    std::vector<unsigned> d_list;
    std::vector<unsigned> p_list;


    double min_start_distance = INFINITY; // find depot to closest pickup.

    unsigned start_depot = depots[x];

    bool check = 0;
    unsigned hold_int = map->number_of_intersectionss + 1;
    unsigned start_pickup = map->number_of_intersectionss + 1;

    std::vector<unsigned> temp_path = find_path_between_intersections(start_depot, deliveries[0].pickUp, turn_penalty);

    double xy = compute_path_travel_time(temp_path, turn_penalty);

    if (xy == 0) {
        check = true;
        hold_int = start_depot;
        if (start_depot != depots[0]) {
            start_depot = depots[0];
        } else {
            start_depot = depots[1];
        }

    }

    // finding closest pickup to start depot
    //#pragma omp parallel for
    for (unsigned i = 0; i < deliveries.size(); i++) {
        unsigned temp1 = deliveries[i].pickUp;

        pickup_.push_back(deliveries[i].pickUp); // pickup vector
        go_to_list.push_back(deliveries[i].pickUp); // go to list
        // d_list.push_back(deliveries[i].dropOff); // deliveries vector

        auto it = deliveries_.find(deliveries[i].pickUp);
        if (it != deliveries_.end()) {

            std::vector <unsigned> temp2;
            deliveries_.emplace(deliveries[i].pickUp, temp2);
            deliveries_[deliveries[i].pickUp].push_back(deliveries[i].dropOff);

        } else {
            deliveries_[deliveries[i].pickUp].push_back(deliveries[i].dropOff);
        }



        

        //double xx = compute_path_travel_time(temp_path, turn_penalty);
        double xx = make_Heuristic_calcc(start_depot, temp1, 80);
        
        if (xx < min_start_distance) {
            min_start_distance = xx;
            start_pickup = temp1;
        }
    }
    list.push_back(start_depot);
    list.push_back(start_pickup);
    p_list.push_back(start_pickup);
    add_path(deliveries_[start_pickup], go_to_list);
    list_position_pickup.emplace(start_pickup, list.size() - 1);

    for (unsigned i = 0; i < deliveries_[start_pickup].size(); i++) {
        std::vector<unsigned> test;
        list_pickup_position_of_d.emplace(deliveries_[start_pickup][i], test);
        list_pickup_position_of_d[deliveries_[start_pickup][i]].push_back(list.size() - 1);
    }

    std::vector<unsigned> temp = go_to_list;
    go_to_list.clear();
    for (unsigned i = 0; i < temp.size(); i++) {
        if (temp[i] != start_pickup)
            go_to_list.push_back(temp[i]);
    }
    //go_to_list.push_back(deliveries_[start_pickup]);
    add_path(deliveries_[start_pickup], go_to_list);
    unsigned current_node = start_pickup;
    unsigned current_temp = map->number_of_intersectionss + 1;

    //std::cout<< "coo;" << std::endl;
    while (!go_to_list.empty()) {

        double min_path = INFINITY;

        //std::cout<< "ss" << std::endl;

        // #pragma omp parallel for
        for (unsigned i = 0; i < go_to_list.size(); i++) {

            LatLon ir = map->get_Lat_Lon_i(current_node);
            LatLon p = map->get_Lat_Lon_i(go_to_list[i]);
            double xx = find_distance_between_two_points(ir, p);
            if (xx < min_path) {
                min_path = xx;
                current_temp = go_to_list[i];
            }

        }



        // std::cout<< "ssas" << std::endl;
        for (unsigned i = 0; i < p_list.size(); i++) {
            //  std::cout<< p_list[i]<< "plist" << std::endl;
        }

        for (unsigned j = 0; j < list.size(); j++) {
            //  std::cout<< list[j]<< "LIST" << std::endl;
        }
        current_node = current_temp;

        auto search = find(pickup_.begin(), pickup_.end(), current_node);
        auto search2 = find(p_list.begin(), p_list.end(), current_node);
        if (search != pickup_.end() && search2 == p_list.end()) {
            add_path(deliveries_[current_node], go_to_list);
            for (unsigned i = 0; i < deliveries_[current_node].size(); i++) {
                auto search = list_pickup_position_of_d.find(deliveries_[current_node][i]);
                if (search != list_pickup_position_of_d.end()) {
                    std::vector<unsigned> test;
                    list_pickup_position_of_d.emplace(deliveries_[current_node][i], test);
                    list_pickup_position_of_d[deliveries_[current_node][i]].push_back(list.size());

                } else {
                    list_pickup_position_of_d[deliveries_[current_node][i]].push_back(list.size());
                }
            }
            p_list.push_back(current_node);
            list_position_pickup.emplace(current_node, list.size());

        } else {
            list_position_delivery.emplace(current_node, list.size());
            d_list.push_back(current_node);
        }
        //std::cout<< "asdasd" << std::endl;

        // for (unsigned i=0; i< go_to_list.size(); i++){
        //    std::cout<< go_to_list[i]<< std::endl;
        //}

        std::vector<unsigned> temp = go_to_list;
        go_to_list.clear();
        for (unsigned i = 0; i < temp.size(); i++) {
            if (temp[i] != current_node)
                go_to_list.push_back(temp[i]);
        }
        //  std::cout<< "FINAL" << std::endl;
        list.push_back(current_node);
    }


    //std:: cout << "wow" << std::endl;
    unsigned end_deliveries = list[list.size() - 1];
    double min_end_d = INFINITY;
    double min_end_dd = INFINITY;
    unsigned end_depot = map->number_of_intersectionss + 1;
    // finding end depot
    double current_travel_time = 0;
    for (unsigned i = 0; i < list.size() - 1; i++) {

        current_travel_time += make_Heuristic_calcc(list[i], list[i + 1], 80);
    }
    //std::cout<< current_travel_time <<std::endl;
    two_opt(list, list_pickup_position_of_d, list_position_delivery, d_list, current_travel_time, turn_penalty, deliveries_, p_list, list_position_pickup);
    //std::cout<< current_travel_time << std::endl;
    
    double current_travel_ = 0;
    for (unsigned i = 0; i < list.size() - 1; i++) {

        current_travel_ += make_Heuristic_calcc(list[i], list[i + 1], 80);
    }

   // three_opt(list, list_pickup_position_of_d, list_position_delivery, d_list, current_travel_, turn_penalty, deliveries_, p_list, list_position_pickup);





    for (unsigned i = 0; i < depots.size(); i++) {



        std::vector<unsigned> temp_path = find_path_between_intersections(end_deliveries, depots[i], turn_penalty);
        double xx = compute_path_travel_time(temp_path, turn_penalty);





        if (xx < min_end_d && xx != 0) {
            min_end_d = xx;
            end_depot = depots[i];
        }

    }

    list.push_back(end_depot);
    //three_opt(list, list_pickup_position_of_d, list_position_delivery, d_list, current_travel_, turn_penalty, deliveries_, p_list, list_position_pickup);


    std::vector<unsigned> final_path;

    for (unsigned i = 0; i < list.size() - 1; i++) {
        std::vector<unsigned> temp = find_path_between_intersections(list[i], list[i + 1], turn_penalty);

        add_path(temp, final_path);

    }

    return final_path;


}


#include "omp.h"

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty) {

    int timestart = clock();
    double CLOCKS_PER_SEC;
    int timediff = 0;

    double min_d = INFINITY;
    std::vector<unsigned> final;
    std::unordered_map<double, std::vector<unsigned> > stuff;

    struct compare {

        bool operator()(const double & lhs, const double & rhs) {
            return lhs > rhs;
        }

    };

    std::vector<unsigned> start_depots = {0, depots.size() / 2, depots.size() / 4};

    std::priority_queue<double, std::vector<double>, compare> list;
    std::vector<unsigned> temp;
    double xx;
    unsigned i = 0;
    unsigned thread;
    if (depots.size() > 10) {
        thread = 5;
    } else {
        thread = depots.size();
    }
#pragma omp parallel num_threads(thread)

    {
        //for (unsigned i = 0; i < 3; i++) {




#pragma omp critical   
        temp = traveling_courier3(deliveries, depots, turn_penalty, i);
        xx = compute_path_travel_time(temp, turn_penalty);

        if (!temp.empty()) {
            stuff.emplace(xx, temp);
            list.push(xx);
        }




        i++;


        //}
    }

    final = stuff[list.top()];
    double xxx = compute_path_travel_time(final, turn_penalty);



    std::vector <unsigned> final2 = traveling_courier5(deliveries, depots, turn_penalty, timestart);


    double xxxx = compute_path_travel_time(final2, turn_penalty);

    if (xxx < xxxx) {
        return final;
    } else {
        return final2;
    }
}

void two_opt(std::vector<unsigned>& list,
        std::unordered_map<unsigned, std::vector<unsigned> >& pickup_d,
        std::unordered_map<unsigned, unsigned> &list_d,
        std::vector<unsigned>&d_list, double current_min_time, const float turn_penalty,
        std::unordered_map <unsigned, std::vector<unsigned> > &deliveries_,
        std::vector<unsigned> p_list,
        std::unordered_map <unsigned, unsigned> &list_p) {

    int improve = 0;
    /*for (unsigned i = 0; i < list.size(); i++) {

        //std::cout << list[i] << "start" << std::endl;
    }*/


    while (improve < 3) {
        //std::cout << "yes" << list[pickup_d[59404]] << std::endl;

        for (unsigned j = 1; j < list.size() - 2; j++) {


            //unsigned j = i + 1;

            for (unsigned h = j + 1; h < list.size() - 1; h++) {
                //unsigned k = h + 1;


                bool flag = 0;
                std::vector<unsigned> drop = deliveries_[j];

                for (unsigned x = 0; x < drop.size(); x++) {

                    if (drop[x] == list[h]) {
                        flag = true;
                    }
                }

                bool flag1 = 0;
                auto search11 = list_p.find(list[j]);
                if (search11 != list_p.end()) {





                    std::vector<unsigned> dropa = deliveries_[list[j]];

                    for (unsigned x = 0; x < dropa.size(); x++) {

                        if (list_d[dropa[x]] < h) {
                            flag1 = true;
                        }

                    }
                }

                auto search = list_d.find(list[h]);
                if (search != list_d.end()) {

                    bool flag2 = false;
                    std::vector<unsigned> pickups = pickup_d[list[h]];
                    for (unsigned i = 0; i < pickups.size(); i++) {

                        if (pickups[i] >= j) {
                            flag2 = true;
                        }
                    }





                    if (flag2 == 0 && flag == 0 && flag1 == 0) {


                        unsigned temp = list[j];
                        list[j] = list[h];
                        list[h] = temp;

                        double xx = 0;
                        for (unsigned i = 0; i < list.size() - 1; i++) {
                            xx += make_Heuristic_calcc(list[i], list[i + 1], 80);
                        }



                        if (xx < current_min_time) {

                            //std::cout<< xx << std::endl;
                            auto search1 = list_p.find(list[j]);
                            if (search1 != list_p.end()) {
                                std::vector<unsigned> dropa = deliveries_[list[j]];

                                for (unsigned x = 0; x < dropa.size(); x++) {

                                    std::vector<unsigned> test = pickup_d[dropa[x]];

                                    for (unsigned x = 0; x < test.size(); x++) {
                                        if (test[x] == j) {
                                            test[x] = h;
                                        }
                                    }

                                }
                            }

                            auto search12 = list_d.find(list[j]);
                            if (search12 != list_d.end()) {

                                list_d[list[j]] = h;

                            }

                            list_d[list[h]] = j;

                            current_min_time = xx;

                            /* for (unsigned i = 0; i < list.size(); i++) {

                                 //std::cout << list[i] << std::endl;
                             }*/

                            //improve =0;

                        } else {


                            unsigned temp1 = list[j];
                            list[j] = list[h];
                            list[h] = temp1;
                        }

                        // std::cout<< "yes" << pickup_d[15602] << std::endl;
                    }


                } else if (search == list_d.end()) {


                    bool flag11 = 0;
                    auto search11 = list_p.find(list[j]);
                    if (search11 != list_p.end()) {
                        std::vector<unsigned> dropa = deliveries_[list[j]];

                        for (unsigned x = 0; x < dropa.size(); x++) {

                            if (list_d[dropa[x]] <= h) {


                                flag11 = true;
                            }

                        }
                    }


                    if (flag11 == 0) {



                        unsigned temp = list[j];
                        list[j] = list[h];
                        list[h] = temp;

                        double xx = 0;
                        for (unsigned i = 0; i < list.size() - 1; i++) {
                            xx += make_Heuristic_calcc(list[i], list[i + 1], 80);
                        }


                        if (xx < current_min_time) {






                            current_min_time = xx;
                            //std::cout<< xx << std::endl;

                            auto search1 = list_p.find(list[j]);
                            if (search1 != list_p.end()) {
                                std::vector<unsigned> dropa = deliveries_[list[j]];

                                for (unsigned x = 0; x < dropa.size(); x++) {

                                    std::vector<unsigned> test = pickup_d[dropa[x]];

                                    for (unsigned x = 0; x < test.size(); x++) {
                                        if (test[x] == j) {
                                            test[x] = h;
                                        }
                                    }

                                }
                            } else {
                                list_d[list[j]] = h;
                            }

                            std::vector<unsigned> drops = deliveries_[list[h]];

                            for (unsigned x = 0; x < drops.size(); x++) {

                                std::vector<unsigned> test = pickup_d[drops[x]];

                                for (unsigned x = 0; x < test.size(); x++) {
                                    if (test[x] == h) {
                                        test[x] = j;
                                    }
                                }

                            }


                            //std::cout<< "yes" << pickup_d[15602] << std::endl;

                            //improve =0;
                            //for (unsigned i = 0; i < list.size(); i++) {

                            //std::cout << list[i] << std::endl;
                            //}

                        } else {


                            unsigned temp1 = list[j];
                            list[j] = list[h];
                            list[h] = temp1;
                        }

                    }
                }


            }
        }

        // std::cout << "yes" << list[pickup_d[59404]] << std::endl;
        // std::cout << "no" << list[list_d[59404]] << std::endl;
        improve++;
    }

    //for (unsigned i = 0; i < list.size(); i++) {

    //std::cout << list[i] << "end" << std::endl;
    // }

    //std::cout<< current_min_time << "inside fcn" << std::endl;

}

void three_opt(std::vector<unsigned>& list,
        std::unordered_map<unsigned, std::vector<unsigned> >& pickup_d,
        std::unordered_map<unsigned, unsigned> &list_d,
        std::vector<unsigned>&d_list, double &current_min_time, const float turn_penalty,
        std::unordered_map <unsigned, std::vector<unsigned> > &deliveries_,
        std::vector<unsigned> p_list,
        std::unordered_map <unsigned, unsigned> &list_p) {



    int improve = 0;

    while (improve < 3) {

        std::cout<< "Here" << std::endl;
        for (unsigned a = 1; a < d_list.size() - 2; a++) {

            for (unsigned b = a + 1; b < d_list.size() - 1; b++) {

                for (unsigned c = b + 1; c < d_list.size(); c++) {


                    double xx = INFINITY;
                    double yy = INFINITY;
                    double zz = INFINITY;
                    double aa = INFINITY;
                    double bb = INFINITY;

                    std::vector<unsigned> temp_l;
                    std::vector<unsigned> temp_l2;
                    std::vector<unsigned> temp_l3;
                    std::vector<unsigned> temp_l4;
                    std::vector<unsigned> temp_l5;
                    unsigned temp = list[list_d[d_list[a]]];
                    unsigned temp1 = list[list_d[d_list[b]]];
                    unsigned temp2 = list[list_d[d_list[c]]];

                    //CAB, CBA

                    bool flag2 = false;
                    std::vector<unsigned> pickups = pickup_d[d_list[c]];
                    for (unsigned i = 0; i < pickups.size(); i++) {

                        if (pickups[i] >= list_d[a]) {
                            flag2 = true;
                        }

                        if (pickups[i] >= list_d[b]) {
                            flag2 = true;
                        }


                    }

                    if (flag2 == 0) {

                        std::cout<< " wow" << std::endl;
                        temp_l = list;


                        temp_l[list_d[d_list[a]]] = temp2; //C->A
                        temp_l[list_d[d_list[c]]] = temp; // A->c


                        for (unsigned i = 0; i < temp_l.size() - 1; i++) {
                            xx += make_Heuristic_calcc(temp_l[i], temp_l[i + 1], 80);
                        }


                        temp_l2 = list;


                        temp_l2[list_d[d_list[a]]] = temp2; //C->A
                        temp_l2[list_d[d_list[c]]] = temp1; // B->C                       
                        temp_l2[list_d[d_list[b]]] = temp; // A->B 


                        for (unsigned i = 0; i < temp_l2.size() - 1; i++) {
                            yy += make_Heuristic_calcc(temp_l2[i], temp_l2[i + 1], 80);
                        }

                    }

                    bool flag3 = false;
                    std::vector<unsigned> pickupsss = pickup_d[d_list[c]];
                    for (unsigned i = 0; i < pickupsss.size(); i++) {

                        if (pickupsss[i] >= list_d[b]) {
                            flag3 = true;
                        }

                    }

                    bool flag4 = false;
                    std::vector<unsigned> pickupss = pickup_d[d_list[b]];
                    for (unsigned i = 0; i < pickupss.size(); i++) {

                        if (pickupss[i] >= list_d[a]) {
                            flag4 = true;
                        }

                    }

                    if (flag3 == 0 && flag4 == 0) {
                        
                        temp_l3 = list;


                        temp_l3[list_d[d_list[a]]] = temp1; //B->A
                        temp_l3[list_d[d_list[c]]] = temp; // A->C                       
                        temp_l3[list_d[d_list[b]]] = temp2; // C->B 

                        for (unsigned i = 0; i < temp_l3.size() - 1; i++) {
                            zz += make_Heuristic_calcc(temp_l3[i], temp_l3[i + 1], 80);
                        }
                    }

                    bool flag6 = false;
                    std::vector<unsigned> pickupsy = pickup_d[d_list[c]];
                    for (unsigned i = 0; i < pickupsy.size(); i++) {

                        
                        if (pickupsy[i] >= list_d[b]) {
                            flag6 = true;
                        }


                    }
                    
                    if (flag6 == 0) {
                        std::cout<< " wosw" << std::endl;
                        temp_l4 = list;


                        
                        temp_l4[list_d[d_list[c]]] = temp1; // B->C                       
                        temp_l4[list_d[d_list[b]]] = temp2; // C->B 

                        for (unsigned i = 0; i < temp_l4.size() - 1; i++) {
                            aa += make_Heuristic_calcc(temp_l4[i], temp_l4[i + 1], 80);
                        }
                    }
                    
                    if (flag4==0){
                        
                         temp_l5 = list;


                        
                        temp_l5[list_d[d_list[a]]] = temp1; // B->A                       
                        temp_l5[list_d[d_list[b]]] = temp; // A->B 

                        for (unsigned i = 0; i < temp_l5.size() - 1; i++) {
                            bb += make_Heuristic_calcc(temp_l5[i], temp_l5[i + 1], 80);
                        }
                    }
                    
                    
                    
                    
                    
                    
                    
                    
                    bool flag5 = false;
                    if (xx == yy == zz == aa == bb ==  INFINITY) {

                        flag5 = true;

                    }

                    if (flag5 == false) {

                        double fin = std::min(xx, yy);
                        
                        double fin2 = std::min(aa,bb);
                        
                        double fin3 = std::min(fin,fin2);
                        double final = std::min(zz, fin3);

                        if (final < current_min_time) {

                            if (final == xx) {

                                list = temp_l;

                                d_list[c] = temp;
                                d_list[a] = temp2;

                                unsigned pos = list_d[d_list[a]];
                                list_d[d_list[a]] = list_d[d_list[c]];
                                list_d[d_list[c]] = pos;
                                current_min_time =xx;

                            } else if (final == yy) {

                                list = temp_l2;

                                d_list[c] = temp1;
                                d_list[a] = temp2;
                                d_list[b] = temp;

                                unsigned pos = list_d[d_list[a]];
                                unsigned pos1 = list_d[d_list[b]];
                                unsigned pos2 = list_d[d_list[c]];


                                list_d[d_list[a]] = pos1;
                                list_d[d_list[c]] = pos;
                                list_d[d_list[b]] = pos2;
                                current_min_time =yy;


                            }
                            else if (final == zz) {

                                list = temp_l3;

                                d_list[c] = temp;
                                d_list[a] = temp1;
                                d_list[b] = temp2;

                                unsigned pos = list_d[d_list[a]];
                                unsigned pos1 = list_d[d_list[b]];
                                unsigned pos2 = list_d[d_list[c]];


                                list_d[d_list[a]] = pos2;
                                list_d[d_list[c]] = pos1;
                                list_d[d_list[b]] = pos;
                                current_min_time =zz;


                            }
                            else if (final == aa) {

                                list = temp_l4;

                                d_list[c] = temp1;
                                d_list[b] = temp2;

                                unsigned pos = list_d[d_list[a]];
                                unsigned pos1 = list_d[d_list[b]];
                                unsigned pos2 = list_d[d_list[c]];


                                
                                list_d[d_list[c]] = pos1;
                                list_d[d_list[b]] = pos2;
                                current_min_time =aa;


                            }
                            
                            else if (final == bb) {

                                list = temp_l5;

                                
                                d_list[a] = temp1;
                                d_list[b] = temp;

                                unsigned pos = list_d[d_list[a]];
                                unsigned pos1 = list_d[d_list[b]];
                                unsigned pos2 = list_d[d_list[c]];


                                list_d[d_list[a]] = pos1;
                                
                                list_d[d_list[b]] = pos;
                                current_min_time =bb;


                            }
                        }

                    }





                }
            }
        }

        improve++;
    }

}