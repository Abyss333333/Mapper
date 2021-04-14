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
#include<time.h>
#include<ctime>
#include "m3.h"
#include "m4.h"


float LatAvgMap;

std::string second_int;


// draw map function. creates initial bounds, buttons and where event loop is initialized.

void draw_map()//main
{
    //    bool quitandNewmap = 0;
    setClr();
    double max_lat = map->get_Lat_Lon_i(0).lat();
    double min_lat = max_lat;
    double max_lon = map->get_Lat_Lon_i(0).lon();
    double min_lon = max_lon;

    unsigned interNum = map->get_number_of_intersections(); //total num of intersection

    for (unsigned id = 0; id < interNum; ++id) {
        max_lat = std::max(max_lat, map->get_Lat_Lon_i(id).lat());
        min_lat = std::min(min_lat, map->get_Lat_Lon_i(id).lat());

        max_lon = std::max(max_lon, map->get_Lat_Lon_i(id).lon());
        min_lon = std::min(min_lon, map->get_Lat_Lon_i(id).lon());
    }




    //std::cout<<"enterDraw-screen"<<std::endl;

    t_color BGclr = t_color(240, 236, 236, 255); //clr of bg
    init_graphics("ECE297_Group #29", BGclr);

    //======set range of screen======

    double xminedge = LonToCar(min_lon);
    double xmaxedge = LonToCar(max_lon);
    double yminedge = LatToCar(min_lat);
    double ymaxedge = LatToCar(max_lat);
    //std::cout<<"x"<<xminedge<<"to"<<xmaxedge<<"Y:"<<yminedge<<"-"<<ymaxedge;


    create_button("Window", "Poi", display_POI);

    set_drawing_buffer(OFF_SCREEN);

    create_button("Window", "Feature", display_feat);
    create_button("Window", "Find Int", Search_intersections);
    create_button("Window", "Find POI", Search_POI);
    create_button("Window", "Find Street", Search_street_b);
    create_button("Window", "Legend", display_legend);
    create_button("Window", "Draw Favorites", add_favourite);
    create_button("Window", "Search History", search_History);
    create_button("Window", "Reload Map", load_another_map);
    create_button("Window", "Find Path", Search_Feature);
    create_button("Window", "Help", help);
    /*set_keypress_input(true);
    set_mouse_move_input(true);
     */

    set_keypress_input(true);
    set_mouse_move_input(true);

    init_graphics("ECE297 Group29", BGclr);
    const t_bound_box initial_coords = t_bound_box(xminedge, yminedge, xmaxedge, ymaxedge); // change!!!
    set_visible_world(initial_coords);
    event_loop(act_on_button_press, act_on_mouse_move, act_on_key_press, draw_screen);



    close_graphics();



}//------------end of main drawmap------------------


// function when user clicks the favourite button. Stores either the closes POI to where the user clicked or lets them create their point. 

void ask_fav() {

    std::string name, yes;
    float x = map->get_mouse_click_x();
    float y = map->get_mouse_click_y();

    float xxx = CarxToLon(x);
    float yyy = CaryToLat(y);
    LatLon position(yyy, xxx);


    // check if the user intended pressed on POI
    //if so save that POI in DS
    unsigned i; //i=BAthrus street Theatre // 
    i = find_closest_point_of_interest(position);
    //LatLon temp = map->get_Lat_Lon_i(i);

    // float xx = LonToCar(temp.lon());
    //float yy = LatToCar(temp.lat());

    // t_point temp1 (xx, yy);

    //map->set_ask_fav_loc(temp1);
    //map->set_ask_fav_name(map->get_intersection_name(i));


    // draw_surface(load_png_from_file("libstreetmap/resources/small_image.png"), temp1.x, temp1.y);
    //  setcolor(BLACK);
    //  settextrotation(0);


    std::cout << " Do you mean " << map->get_POI_name(i) << "?  y/n" << std::endl;



    std::getline(std::cin, yes);
    if (yes == "y") {
        std::cout << "saving " << map->get_POI_name(i) << std::endl;
        map->set_ask_fav_name(map->get_POI_name(i));
        LatLon temp = map->get_LatLon_P(i);

        float xx = LonToCar(temp.lon());
        float yy = LatToCar(temp.lat());

        t_point temp1(xx, yy);

        map->set_ask_fav_loc(temp1);

        std::cout << "Added to Favourites" << std::endl;
    }// if not POI
    else if (yes != "y") {
        std::cout << " what do you want to name it?" << std::endl;
        std::getline(std::cin, name);


        map->set_ask_fav_name(name);
        t_point temp2(x, y);
        map->set_ask_fav_loc(temp2);
        std::cout << "Added to Favourites" << std::endl;

    }
    // 
    


}
// The create button function for Favourites. If the button is clicked, sets a boolean value true

void add_favourite(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();
    if (map->get_Fav_click() == true) {
        map->set_Fav_click(false);
        map->ask_fav_name.clear();
        map->ask_fav_loc.clear();
    } else map->set_Fav_click(true);

}
// The create button function for displaying all POI button. If the button is clicked, sets a boolean value true

void display_POI(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_POI_click() == false) {
        map->set_POI_click(true);
    } else map->set_POI_click(false);

}

// The create button function for displaying Features button. If the button is clicked, sets a boolean value true

void display_feat(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_feat_click() == false) {
        map->set_feat_click(true);
        map->set_legend_click(false);
    } else map->set_feat_click(false);

    //copy_off_screen_buffer_to_screen();
}
// The create button function for Legend button. If the button is clicked, sets a boolean value true

void display_legend(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_legend_click() == false) {
        map->set_legend_click(true);
    } else map->set_legend_click(false);

    draw_screen();
}

void help(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_help_click() == false) {
        map->set_help_click(true);
    } else map->set_help_click(false);

    draw_screen();
}
// The create button function for Find Intersections. If the button is clicked, sets a boolean value true

void Search_intersections(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_search_click() == true) {
        map->set_search_click(false);
    } else map->set_search_click(true);
}

void Search_Feature(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_search_f() == true) {
        map->set_search_f(false);
        map->s.clear();
        map->part.clear();
        map->k.clear();
    } else map->set_search_f(true);
}

// The create button function for Searching Points of Interest. If the button is clicked, sets a boolean value true

void Search_POI(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_search_POI() == true) {
        map->set_search_POI(false);
    } else map->set_search_POI(true);
}
// The create button function for reloading map. If the button is clicked, sets a boolean value true

void load_another_map(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();
    if (map->get_load_another_map() == false)
        map->set_load_another_map(true);
    else
        map->set_load_another_map(false);

}
// The create button function for Search Street Button. If the button is clicked, sets a boolean value true

void Search_street_b(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();

    if (map->get_search_street() == true) {
        map->set_search_street(false);
    } else map->set_search_street(true);

}
// The create button function for search history button. If the button is clicked, sets a boolean value true

void search_History(void(*drawscreen_ptr)(void)) {
    drawscreen_ptr();
    if (map->get_search_history_click() == true) {

        map->set_search_history_click(false);
    } else map->set_search_history_click(true);


}
// google function. allows user to intereact with the internet

void google(std::string first) {

    // std::string second="near me";
    std::string path = "firefox http://www.google.com/search?q=";
    //std::string city = "Toronto";
    std::replace(first.begin(), first.end(), ' ', '+');
    // std::replace(second.begin(), second.end(), ' ', '+');

    std::string co = path + first; // + "+" + "and" + "+" + second + "+" + city;
    std::cout << co << std::endl;
    const char* check = co.c_str();
    system(check);
    std::cout << "Press enter to continue" << std::endl;
    std::cin.get();
    //std::cout << "After input" << std::endl;

}
// Checks History. Runs when user clicks Search History button

void checkHistory() {
    std::vector<std::string> history;
    history = map->get_search_histor(); //vector with all entered streets

    int s = history.size();
    //    int x = 0;
    int n;
    std::cout << "1) Show History" << std::endl;
    std::cout << "2) Delete History" << std::endl;
    std::cin>>n;

    if (n == 1) {

        if (s == 0) {
            std::cout << "Your history is empty" << std::endl;
            return;
        }
        for (unsigned i = history.size() - 1; i > 0; i--) {
            std::cout << history[i] << std::endl;
        }
        std::cout << history[0] << std::endl;
    } else if (n == 2) {
        map->set_clear_history();
        history.clear();

    }

}
// Search Intersection function. Runs when user clicks Search Interesection button. Type in 2 street and returns their intersection if exists.

LatLon Search() {
    std::vector<unsigned> id1, id2;
    std::vector<unsigned> intersections; //
    std::string first, second, line;
    std::cout << "Enter first street" << std::endl;
    std::getline(std::cin, first);

    std::cout << "Enter second street" << std::endl;
    std::getline(std::cin, second);

    map->set_search_histor(first);
    map->set_search_histor(second);
    LatLon temp;



    id1 = map->get_street_inter_id(first); // vector for all intersections of first street
    id2 = map->get_street_inter_id(second); //vector for all intersections of second street
    ////find common elements between 2 vectors
    std::sort(id1.begin(), id1.end());
    std::sort(id2.begin(), id2.end());

    for (unsigned i = 0; i < id1.size(); i++) {
        auto search = find(id2.begin(), id2.end(), id1[i]);
        //std::cout<< i<< std::endl;
        if (search != id2.end()) {

            map->set_temp_LatLon(map->get_Lat_Lon_i(id1[i]));
            map->set_draw_int(true);
            map->set_found_intersection(getIntersectionName(id1[i]));
            std::cout << getIntersectionName(id1[i]) << std::endl;

            return temp;


        }


    }

    std::cout << "Not Found" << std::endl;

    //map->set_search_click(false);

    return temp;
}
// Search POI function. Runs if user clicked Search POI button. allows user to search for any existing POI.
// Also lets them connect to internet if what they typed is not found.

LatLon Search_POI() {
    std::vector <unsigned> id; //

    std::string first, line;
    std::cout << "Enter POI" << std::endl;
    std::getline(std::cin, first);



    map->set_search_histor(first);
    LatLon temp;



    id = map->get_POI_id_from_name(first); // vector for all intersections of first street

    if (id.empty() != true) {
        map->set_draw_found_POI(true);
        map->set_found_POI_id(id);

        std::cout << "Found " << std::endl;

        return temp;
    }

    std::cout << "Not Found" << std::endl;
    std::cout << "Do you want to search the Internet? y/n" << std::endl;
    std::string yes;
    std::getline(std::cin, yes);
    if (yes == "y")
        // search the Internet
    {
        google(first);
    }

    std::cout << "Did You Find What You were looking for? y/n" << std::endl;

    std::string yess;
    std::getline(std::cin, yess);
    if (yess == "y") {
        std::string first, line;
        std::cout << "Enter POI" << std::endl;
        std::getline(std::cin, first);



        map->set_search_histor(first);
        LatLon temp;



        id = map->get_POI_id_from_name(first); // vector for all intersections of first street

        if (id.empty() != true) {
            map->set_draw_found_POI(true);
            map->set_found_POI_id(id);

            std::cout << "Found " << std::endl;

            return temp;
        }
    }


    return temp;
}
// Search Street function. Runs if user clicked Search street button. allows user to search for any existing street.
// Also lets them connect to internet if what they typed is not found.

LatLon Search_street() {
    std::vector<unsigned> id; //

    std::string first, line;
    std::cout << "Enter Street" << std::endl;
    std::getline(std::cin, first);



    map->set_search_histor(first);
    LatLon temp;



    id = map->get_street_name_street_segment(first); // 

    if (id.empty() != true) {
        map->set_draw_found_street(true);
        map->set_found_street_id(id);

        std::cout << "Found " << std::endl;

        return temp;

    }

    std::cout << "Not Found" << std::endl;
    std::cout << "Do you want to search the Internet? y/n" << std::endl;
    std::string yes;
    std::getline(std::cin, yes);
    if (yes == "y")
        // search the Internet
    {
        google(first);
    }

    std::cout << "Did You Find What You were looking for? y/n" << std::endl;
    std::string yess;
    std::getline(std::cin, yess);
    if (yess == "y") {
        std::string first, line;
        std::cout << "Enter Street" << std::endl;
        std::getline(std::cin, first);







        map->set_search_histor(first);
        LatLon temp;



        id = map->get_street_name_street_segment(first); // 

        if (id.empty() != true) {
            map->set_draw_found_street(true);
            map->set_found_street_id(id);

            std::cout << "Found " << std::endl;

            return temp;
        }
    }

    //map->set_search_click(false);

    return temp;
}
#include <X11/keysym.h>
//Act on Key press function. If certain keys are pressed, sets certain bool variables to true.
// has similar function to clicking buttons

void act_on_key_press(char x, int keysym) {

    // (void )(keysym);

    if (map->get_search_f() == true) { //if the user searching features

        std::vector<std::string> names = map->intersection_name;
        std::vector<std::string> namesp = map->POI_names;

        // std::vector<std::string> names = map->intersection_name;
        std::stringstream ss;
        std::string r;
        ss << x;
        ss>>r;

        unsigned l = map->s.size();
        map->s += r;
        map->s[0] = toupper(map->s[0]);
        for (unsigned i = 0; i < l; i++) {
            if (map->s[i] == ' ') {
                if ((map->s[i + 1] == 'o' && map->s[i + 2] == 'f' && map->s[i + 3] == ' ') || (map->s[i + 1] == 'a' && map->s[i + 2] == 't' && map->s[i + 3] == ' ') || (map->s[i + 1] == '-')) {
                } else if (map->s[i + 1] == 'A' && map->s[i + 2] == 'n' && map->s[i + 3] == 'd') {
                    map->s[i + 1] = '&';
                    map->s.erase(l - 1);
                } else if ((map->s[i + 1] == 'T' && map->s[i + 2] == 'o')) {
                    map->s[i + 1] == 't';
                    map->s.erase(l - 2);
                    map->k = map->s;
                    map->s.clear();

                } else if ((map->s[i + 1] == 'P' && map->s[i + 2] == 'o')) {
                    map->s[i + 1] == 'p';
                    map->s.erase(l - 2);
                    // map->l = map->s;
                    map->s.clear();

                } else {
                    map->s[i + 1] = toupper(map->s[i + 1]);
                }
            }


        }

        if (keysym == XK_BackSpace && l > 0) {
            map->s.erase(l - 1);
        } else if (keysym == XK_space) {
            std::string j = " ";
            map->s += j;
        }











        map->set_search_histor(map->s);

        draw_found_intersection_path();
        //}
        for (unsigned i = 0; i < names.size(); i++) {
            std::string temp = names[i];
            std::string partial = temp.substr(0, map->s.size());
            if (partial == map->s) {
                map->part.push_back(temp);
            }

        }

        for (unsigned i = 0; i < namesp.size(); i++) {
            std::string temp = namesp[i];
            std::string partial = temp.substr(0, map->s.size());
            if (partial == map->s) {
                map->l.push_back(temp);
            }

        }



    }//if not searching features

    else if (x == 'p') {
        if (map->get_POI_click() == true) {
            map->set_POI_click(false);
        } else
            map->set_POI_click(true);
    } else if (x == 'f') {
        if (map->get_feat_click() == true) {
            map->set_feat_click(false);
        } else
            map->set_feat_click(true);
    } else if (x == 'i') {
        if (map->get_int_click() == true) {
            map->set_int_click(false);
        } else
            map->set_int_click(true);
    } else if (x == 'x') {
        map->set_mouse_click_x2(10);
        map->set_mouse_click_y2(10);
        map->set_mouse_click2(false);
    } else if (x == 'z') {
        map->set_mouse_click_x(10);
        map->set_mouse_click_y(10);
        map->set_mouse_click(false);
    } else if (x == 'd') {
        if (map->get_draw_int() == false) {
            map->set_draw_int(true);
        } else
            map->set_draw_int(false);
        if (map->get_draw_found_POI() == false) {
            map->set_draw_found_POI(true);
        } else
            map->set_draw_found_POI(false);
        if (map->get_draw_found_street() == false) {
            map->set_draw_found_street(true);
        } else
            map->set_draw_found_street(false);

    } else if (x == 'l') {
        if (map->get_legend_click() == true) {
            map->set_legend_click(false);
        } else
            map->set_legend_click(true);
    } else if (x == '1') {
        if (map->get_ed_click() == true) {
            map->set_ed_click(false);
        } else
            map->set_ed_click(true);
    } else if (x == '2') {
        if (map->get_food_click() == true) {
            map->set_food_click(false);
        } else
            map->set_food_click(true);
    } else if (x == '3') {
        if (map->get_trans_click() == true) {
            map->set_trans_click(false);
        } else
            map->set_trans_click(true);
    } else if (x == '4') {
        if (map->get_med_click() == true) {
            map->set_med_click(false);
        } else
            map->set_med_click(true);
    } else if (x == '5') {
        if (map->get_emergency_click() == true) {
            map->set_emergency_click(false);
        } else
            map->set_emergency_click(true);
    } else if (x == '6') {
        if (map->get_fun_click() == true) {
            map->set_fun_click(false);
        } else
            map->set_fun_click(true);
    } else if (x == '7') {
        if (map->get_sub_click() == true) {
            map->set_sub_click(false);

        } else
            map->set_sub_click(true);
    } else if (x == 'h') {
        if (map->get_help_click() == false) {
            map->set_help_click(true);
        } else
            map->set_help_click(false);
    }





    draw_screen();
}//---end of [act on key press]---

// stores XY co-ordinates of where Shift+button is pressed or where Ctrl+button is pressed

void act_on_button_press(float x, float y, t_event_buttonPressed event) {

    /* Called whenever event_loop gets a button press in the graphics *
     * area.  Allows the user to do whatever he/she wants with button *
     * clicks.                                                        */



    if (event.shift_pressed || event.ctrl_pressed) {



        if (event.shift_pressed) { //use this for add fav

            map->set_mouse_click_x(x);
            map->set_mouse_click_y(y);
            map->set_mouse_click(true);



        } else if (event.ctrl_pressed) {
            map->set_mouse_click_x2(x);
            map->set_mouse_click_y2(y);
            map->set_mouse_click2(true);

        }


    }

    draw_screen();

}

// function for getting coordinates of where the mouse is at all times. Super important as it is the main part of the Hover features.

void act_on_mouse_move(float x, float y) {
    // function to handle mouse move event, the current mouse position in the current world coordinate
    // system (as defined in your call to init_world) is returned

    map->set_mouse_move_x(x);
    map->set_mouse_move_y(y);

    draw_screen();

}

//============================================DRAW screen=====================
// draw screen function. Where all the drawing happens. Also uses all the bool values previously set to true to draw certain portions of the map.

void draw_screen() {


    //set_drawing_buffer(OFF_SCREEN);
    set_draw_mode(DRAW_NORMAL); // Should set this if your program does any XOR drawing in callbacks.
    clearscreen(); /* Should precede drawing for all drawscreens */

    t_bound_box current_bounds = get_visible_world();

    setfontsize(10);
    setlinestyle(SOLID);
    setlinewidth(1);
    setcolor(BLACK);

    if (map->get_feat_click() == true) {
        drawAllFeatures(current_bounds);
    }


    drawAllSegs(current_bounds); //draww all the street segments
    drawAllPoi(current_bounds);


    if (map->get_search_history_click() == true) {
        checkHistory();
    }
    map->set_search_history_click(false);
    // else map->set_search_history_click(true);



    if (map->get_int_click() == true) {
        drawAllInt(current_bounds);
    }
    if (map->get_mouse_click2() == true) {
        draw_closest_POI_and_Int(current_bounds);
    }
    if (map->get_Fav_click() == true) {
        drawAllMyFav();
    }

    if (map->get_search_click()) {
        Search();

    }
    map->set_search_click(false);

    if (map->get_search_POI()) {
        Search_POI();

    }
    map->set_search_POI(false);

    if (map->get_search_street()) {
        Search_street();

    }

    map->set_search_street(false);

    if (map->get_draw_int() == true) {
        draw_int();

    }

    if (map->get_draw_found_POI() == true) {
        draw_found_POI();

    }

    if (map->get_draw_found_street() == true) {
        draw_found_street();

    }

    if (map->get_legend_click() == true && map->get_search_f() == false) {
        drawInstruction();
    }
    if (map->get_sub_click() == true) {
        drawSubway();
    }

    if (map->get_mouse_click() == true) {
        ask_fav();
    }


    map->set_mouse_click(false);

    if (map->get_search_f() == true && map->get_help_click() == false) { //draw help box
        set_coordinate_system(GL_SCREEN);
        settextrotation(0);
        setfontsize(18);
        setcolor(WHITE);
        setlinestyle(SOLID);
        t_point BGrange3[4] = {
            {100, 10},
            {100, 60},
            {1000, 60},
            {1000, 10}
        };

        fillpoly(BGrange3, 4);
        setcolor(BLUE);
        drawrect(100, 10, 1000, 60);
        drawrect(100, 10, 200, 60);
        setcolor(BLACK);
        drawtext(150, 40, "Search");
        unsigned siz = map->s.size();
        unsigned sizz = map->part.size();
        unsigned sizzz = map->l.size();
        float xxx;
        float zzz = sizzz * 15;

        xxx = sizz * 15;

        setfontsize(15);
        if (siz > 10) {
            setfontsize(12);

        }
        if (map->second_bar) {
            setcolor(BLUE);
        }
        drawtext(600, 40, map->s);




        if (!map->part.empty()) {
            if (siz > 3) {
                setcolor(LIGHTSKYBLUE);
                t_point BGrange4[4] = {
                    {300, 60},
                    {300, 60 + xxx},
                    {800, 60 + xxx},
                    {800, 60}
                };
                fillpoly(BGrange4, 4);
                for (unsigned j = 0; j < sizz; j++) {
                    setcolor(BLACK);
                    setfontsize(10);
                    float yyy = 15 * j;
                    drawtext(600, 65 + yyy, map->part[j]);

                }
                map->part.clear();

            }
        }
        /*else if (!map->l .empty()){
            if (siz > 3) {
            setcolor(CYAN);
            t_point BGrange4[4] = {
                {300, 60},
                {300, 60 + zzz},
                {800, 60 + zzz},
                {800, 60}
            };
            fillpoly(BGrange4, 4);
            for (unsigned j = 0; j < sizzz; j++) {
                setcolor(BLACK);
                setfontsize(10);
                float yyy = 15 * j;
                
                drawtext(600, 65 + yyy, map->l[j]);

            }
            map->l.clear();

        }
        }*/
        set_coordinate_system(GL_WORLD);
    }

    drawSpecificPoi(current_bounds);
    if (map->get_mouse_click2() == false && map->get_search_f() == true) {
        draw_found_intersection_path();
    }

    if (map->get_help_click() == true) {
        set_coordinate_system(GL_SCREEN);
        settextrotation(0);
        setfontsize(18);
        setcolor(WHITE);
        setlinestyle(SOLID);

        t_point BGrange3[4] = {
            {300, 10},
            {300, 300},
            {800, 300},
            {800, 10}
        };

        fillpoly(BGrange3, 4);

        setcolor(BLACK);
        drawrect(300, 10, 800, 300);
        setfontsize(18);
        drawtext(550, 40, "Help");
        setfontsize(15);
        drawtext(550, 75, "Press Shift + Click to save location ");
        drawtext(550, 95, "Press Draw Favorites to show stored locations ");
        drawtext(550, 115, "Use Find Buttons to aid Finding locations ");
        drawtext(550, 145, "If location not found,  ");
        drawtext(550, 165, " you will have the option to search Google");
        drawtext(550, 195, "Press Ctrl+ Click Between 2 locations,");
        drawtext(550, 215, "to find path between them ");
        drawtext(550, 245, "Enjoy!");
        drawtext(550, 275, "press 'h' to close help");
        set_coordinate_system(GL_WORLD);

    }

    //-----------TEST!!-----------
    std::vector<unsigned> path;

    double turn_penalty;
    double expected;
    double actual;
    std::string tim = "Tim Hortons";
    //path = {1486, 1487, 154905, 154906, 154907, 154908, 154909, 154910, 154911, 4361, 151744, 151745, 148848, 148842, 148843, 148844, 148845, 148846, 148847, 32804, 32805, 7796, 32830, 32831, 32832, 32833, 19408, 139891, 139892, 139893, 149286, 149285, 149278, 149279, 149280, 149281, 149282, 149283, 149284, 27995, 27996, 27997, 16048, 16049, 86404, 16050, 16051, 16052, 16053, 16054, 152019, 152018, 152009, 152010, 152011, 152012, 152013, 152014, 152015, 152016, 152017, 133941, 110079, 110078, 133940, 145447, 133928, 133929, 133930, 133931, 133932, 133933, 133934, 133935, 133936, 133937, 133938, 133939, 151364, 151365, 87352, 87353, 143709, 87350, 87351, 87354, 135898, 135899, 39836, 39837, 39834, 113326, 39835, 135900, 98630, 98631, 126625, 112866, 112864, 112865, 152155, 152156, 152157, 152158, 152159, 135938, 135939, 148781, 125625, 125626, 84001, 145222, 145223, 145224, 126768, 126769, 126764, 126765, 126766, 126767, 154923, 154924, 154922, 154925, 154926, 154927, 147782, 84002, 10697, 10698, 86897, 84896, 86898, 84897, 84898, 84899, 84900, 84901, 84902, 112060, 112061, 112062, 45741, 45742, 136584, 70778, 70776, 70777, 99007, 39832, 39831, 3790, 86773, 39830, 113605, 113603, 113608, 112381, 112380, 112379, 125533, 125532, 125531};
    //path = find_path_between_intersections(75020,64489, 15);
    //  std::vector<DeliveryInfo> deliveries = {DeliveryInfo(23069, 80185), DeliveryInfo(23459, 60949), DeliveryInfo(62940, 18214), DeliveryInfo(81951, 19441), DeliveryInfo(86025, 46362)};
    // std::vector<unsigned>   depots = {80138, 25819, 5507};
    //turn_penalty = 15;
    // std::vector<unsigned>    result_path = traveling_courier(deliveries, depots, turn_penalty);


    turn_penalty = 31.34028327739956765;
    if (map->point.size() >= 2) {
        map->pathll = find_path_between_intersections(map->point[map->point.size() - 2], map->point[map->point.size() - 1], 0);

        drawFoundPath(map->pathll);

    }


    //  drawFoundPath(result_path);


    copy_off_screen_buffer_to_screen();
}//-----------end of draw screen-----------

void draw_found_feature() {

    /* std::vector<std::string> names = map->intersection_name;
    
     std::string j;
     std::string k;
     unsigned count =0;
     while (map->s[i+1]!= ' ' && map->s[i+2]!= 'T' && map->s[i+3]!= 'o' ){
         j+= map->s[i];
         count=i;
     }
    
     for( unsigned i= (count+5); i< map->s.size(); i++){
         k+= map->s[i];
     }

     auto search = find(names.begin(), names.end(),j );
     if (search != names.end()) {
        // unsigned js = map->intersection_id_from_names[mj];
         unsigned num = map->get_feature_pointnum(js);

         float maxx = -1000;
         float minx = 1000;
         float maxy = -1000;
         float miny = 1000;

         t_point polypts[num];
         for (unsigned p = 0; p < num; ++p) {
             LatLon thepoint = map->get_feature_points(j, p);
             float iy = LatToCar(thepoint.lat());
             float ix = LonToCar(thepoint.lon());

             polypts[p] = {ix, iy};

             if (ix > maxx)
                 maxx = ix;
             if (ix < minx)
                 minx = ix;
             if (iy < miny)
                 miny = iy;
             if (iy > maxy)
                 maxy = iy;
         }


         setcolor(BLACK);
         settextrotation(0);
         drawtext((maxx + minx) / 2, (maxy + miny) / 2, map->s, 100, 100);
         draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), (maxx + minx) / 2, (maxy + miny) / 2);


     }
     */



}

void draw_found_intersection_path() {

    std::vector<std::string> names = map->intersection_name;
    std::vector<std::string> Poi_name = map->POI_names;
    std::vector<unsigned>Poi_path;



    unsigned j;

    auto search = find(names.begin(), names.end(), map->k);
    if (search != names.end()) {
        j = map->intersection_id_from_names[map->k];


        LatLon temp = map->get_Lat_Lon_i(j);
        float xx = LonToCar(temp.lon());
        float y = LatToCar(temp.lat());

        setcolor(BLACK);

        draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
        settextrotation(0);

        drawtext(xx, y + 0.00000025, map->k, 100000, 100000);


    }
    unsigned yt = map->number_of_intersectionss + 1;
    unsigned jj = yt;
    std::string kk;
    bool POI = false;

    auto search1 = find(names.begin(), names.end(), map->s);
    if (search1 != names.end()) {
        jj = map->intersection_id_from_names[map->s];


        LatLon temp = map->get_Lat_Lon_i(jj);
        float xx = LonToCar(temp.lon());
        float y = LatToCar(temp.lat());

        setcolor(BLACK);

        draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
        settextrotation(0);

        drawtext(xx, y + 0.00000025, map->s, 100000, 100000);
    } else if (search1 == names.end()) {


        auto search2 = find(Poi_name.begin(), Poi_name.end(), map->s);
        if (search2 != Poi_name.end()) {


            kk = map->s;

            POI = true;

        }

    }



    if (jj != yt || !kk.empty()) {
        if (POI == false) {
            std::vector<unsigned>path = find_path_between_intersections(j, jj, 0);

            drawFoundPath(path);
        } else {
            Poi_path = find_path_to_point_of_interest(j, kk, 0);
            drawFoundPath(Poi_path);

            float xx = LonToCar(map->path_poi.lon());
            float y = LatToCar(map->path_poi.lat());

            setcolor(BLACK);

            draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
            settextrotation(0);

            drawtext(xx, y + 0.00000025, kk, 100000, 100000);
        }
    }


}












// draws Subway entrances on the map
// has hover feature where if hovered over the certain entrance, name will be displayed

void drawSubway() {
    std::vector<LatLon> tx = map->get_OSM_vector();
    t_bound_box x = get_visible_world();
    for (unsigned i = 0; i < tx.size(); i++) {


        float xx = LonToCar(tx[i].lon());
        float y = LatToCar(tx[i].lat());
        setcolor(DARKSLATEBLUE);
        /*if (x.get_width() >= 0.00095 || x.get_height() >= 0.00095) {
            fillarc(xx, y, 0.0000055, 0, 360);
        } else if ((x.get_width() <= 0.00095 && x.get_width() > 0.0001) || (x.get_height() <= 0.00095 && x.get_height() > 0.001)) {
            fillarc(xx, y, 0.0000015, 0, 360);
            if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {

                    setcolor(BLUE);
                    settextrotation(0);
                    drawtext(xx, y + 0.0000002, "Subway Entrance", 100000, 100000);
                }
            }
        } else if (x.get_width() <= 0.0001 || x.get_height() <= 0.0001) {
            fillarc(xx, y, 0.00000025, 0, 360);
            if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {

                    setcolor(BLUE);
                    settextrotation(0);
                    drawtext(xx, y + 0.0000002, "Subway Entrance", 100000, 100000);
                }
            }
        }*/

        draw_surface(load_png_from_file("libstreetmap/resources/Subwayyyy.png"), xx, y);
        if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
            if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {
                setcolor(BLUE);
                settextrotation(0);
                drawtext(xx, y + 0.0000002, "Subway Entrance", 100000, 100000);
            }
        }
    }


}

// draws specific POI on the map depending on what button is pressed. these buttons can be found in legend.
// has hover feature where if hovered over the certain POI, name will be displayed

void drawSpecificPoi(t_bound_box x) {


    if (map->get_ed_click() == true) {
        std::vector<unsigned> education = map->get_type_education();

        for (unsigned i = 0; i < education.size(); i++) {
            LatLon temp = map->get_LatLon_P(education[i]);
            float xx = LonToCar(temp.lon());
            float y = LatToCar(temp.lat());
            setcolor(SADDLEBROWN);
            /*if (x.get_width() >= 0.00095 || x.get_height() >= 0.00095) {
                fillarc(xx, y, 0.0000055, 0, 360);
            }
            else if ((x.get_width() <= 0.00095 && x.get_width() > 0.0001) || (x.get_height() <= 0.00095 && x.get_height() > 0.001)) {
                fillarc(xx, y, 0.0000015, 0, 360);
                if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                   if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005){    
                        
                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(education[i]), 100000, 100000);
                    }
                }
            }
            else if (x.get_width() <= 0.0001 || x.get_height() <= 0.0001) {
                fillarc(xx, y, 0.00000025, 0, 360);
                if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                   if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005){    
                        
                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(education[i]), 100000, 100000);
                    }
                }
            }*/

            draw_surface(load_png_from_file("libstreetmap/resources/abc.png"), xx, y);
            if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {
                    setcolor(BLUE);
                    settextrotation(0);
                    drawtext(xx, y + 0.0000002, map->get_POI_name(education[i]), 100000, 100000);
                }
            }
        }



    }

    if (map->get_food_click() == true) {
        std::vector<unsigned> food = map->get_type_food();

        for (unsigned i = 0; i < food.size(); i++) {
            LatLon temp = map->get_LatLon_P(food[i]);
            float xx = LonToCar(temp.lon());
            float y = LatToCar(temp.lat());
            setcolor(RED);
            if (x.get_width() >= 0.00095 || x.get_height() >= 0.00095) {
                fillarc(xx, y, 0.0000055, 0, 270);
            } else if ((x.get_width() <= 0.00095 && x.get_width() > 0.0001) || (x.get_height() <= 0.00095 && x.get_height() > 0.001)) {
                fillarc(xx, y, 0.0000015, 0, 270);
                if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                    if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {
                        setcolor(GREEN);
                        fillarc(xx, y, 0.0000015, 0, 270);
                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(food[i]), 100000, 100000);
                    }
                }
            } else if (x.get_width() <= 0.0001 || x.get_height() <= 0.0001) {
                fillarc(xx, y, 0.00000025, 0, 270);
                if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                    if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {
                        setcolor(GREEN);
                        fillarc(xx, y, 0.0000015, 0, 270);
                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(food[i]), 100000, 100000);
                    }
                }
            }
        }

    }

    if (map->get_trans_click() == true) {
        std::vector<unsigned> transport = map->get_type_trans();

        for (unsigned i = 0; i < transport.size(); i++) {
            LatLon temp = map->get_LatLon_P(transport[i]);
            float xx = LonToCar(temp.lon());
            float y = LatToCar(temp.lat()); //Icon made by Freepik from www.flaticon.com
            draw_surface(load_png_from_file("libstreetmap/resources/trans.png"), xx, y);
            if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {
                    setcolor(BLUE);
                    settextrotation(0);
                    drawtext(xx, y + 0.0000002, map->get_POI_name(transport[i]), 100000, 100000);
                }
            }
        }
    }

    if (map->get_med_click() == true) {
        std::vector<unsigned> med = map->get_type_med();

        for (unsigned i = 0; i < med.size(); i++) {
            LatLon temp = map->get_LatLon_P(med[i]);
            float xx = LonToCar(temp.lon());
            float y = LatToCar(temp.lat());
            setcolor(LIGHTSKYBLUE);

            if (x.get_width() >= 0.00095 || x.get_height() >= 0.00095) {
                fillarc(xx, y, 0.000025, 0, 180);
            } else if ((x.get_width() <= 0.00095 && x.get_width() > 0.0001) || (x.get_height() <= 0.00095 && x.get_height() > 0.001)) {
                fillarc(xx, y, 0.0000025, 0, 180);
                if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                    if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {

                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(med[i]), 100000, 100000);
                    }
                }
            } else if (x.get_width() <= 0.0001 || x.get_height() <= 0.0001) {
                fillarc(xx, y, 0.00000025, 0, 180);
                if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                    if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005) {

                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(med[i]), 100000, 100000);
                    }
                }
            }
        }

    }

    if (map->get_emergency_click() == true) {
        std::vector<unsigned> emergency = map->get_type_emergency();

        for (unsigned i = 0; i < emergency.size(); i++) {
            LatLon temp = map->get_LatLon_P(emergency[i]);
            float xx = LonToCar(temp.lon());
            float y = LatToCar(temp.lat()); //Icon made by Freepik from www.flaticon.com
            draw_surface(load_png_from_file("libstreetmap/resources/hospital.png"), xx, y);
            if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                // if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005){    
                setcolor(BLUE);
                settextrotation(0);
                drawtext(xx, y + 0.0000002, map->get_POI_name(emergency[i]), 100000, 100000);
                //}
            }
        }
    }

    if (map->get_fun_click() == true) {
        std::vector<unsigned> rec = map->get_type_fun();

        for (unsigned i = 0; i < rec.size(); i++) {
            LatLon temp = map->get_LatLon_P(rec[i]);
            float xx = LonToCar(temp.lon());
            float y = LatToCar(temp.lat()); //Icon made by Freepik from www.flaticon.com
            draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
            if (map->get_mouse_move_x() <= (xx + 0.0000008) && map->get_mouse_move_x() >= (xx - 0.0000008) && map->get_mouse_move_y() <= (y + 0.0000008) && map->get_mouse_move_y() >= (y - 0.0000008)) {
                // if (x.get_width() <= 0.0005 || x.get_height() <= 0.0005){    
                setcolor(BLUE);
                settextrotation(0);
                drawtext(xx, y + 0.0000002, map->get_POI_name(rec[i]), 10000, 10000);
                //}
            }
        }
    }




}
// draws found intersection. Corresponds to find Intersection button/feature.

void draw_int() {

    LatLon temp = map->get_temp_Latlon();
    float xx = LonToCar(temp.lon());
    float y = LatToCar(temp.lat());
    std::string name = map->get_found_intersection();
    t_bound_box x = get_visible_world();
    setcolor(FIREBRICK);

    if (x.get_width() >= 0.0075 || x.get_height() >= 0.0075) {//Icon made by Freepik from www.flaticon.com
        draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
        settextrotation(0);

        drawtext(xx, y + 0.00000025, name, 100000, 100000);
    } else if (x.get_width() <= 0.0075 || x.get_height() <= 0.0075) {//Icon made by Freepik from www.flaticon.com
        draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
        settextrotation(0);

        drawtext(xx, y + 0.00000025, name, 100000, 100000);
    } else if (x.get_width() <= 0.0025 || x.get_height() <= 0.0025) {//Icon made by Freepik from www.flaticon.com
        draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
        settextrotation(0);

        drawtext(xx, y + 0.00000025, name, 100000, 100000);
    }


}
// draws the Found POI if exists. Corresponds to the Find POI button/feature

void draw_found_POI() {

    std::vector<unsigned> id = map->get_found_POI_id();
    float cx, cy;
    cx = map->get_mouse_move_x();
    cy = map->get_mouse_move_y();
    float lx = CarxToLon(cx);
    float ly = CaryToLat(cy);
    LatLon min(ly, lx);

    double min_d = INFINITY;
    for (unsigned i = 0; i < id.size(); i++) {

        LatLon temp = map->get_LatLon_P(id[i]);
        float xx = LonToCar(temp.lon());
        float y = LatToCar(temp.lat());
        std::string name = map->get_POI_name(id[i]);
        t_bound_box x = get_visible_world();

        double dist = map->distance_calc(temp, min);
        if (dist < min_d) {
            min_d = dist;



            setcolor(BLACK);
            if (x.get_width() >= 0.0075 || x.get_height() >= 0.0075) {//Icon made by Freepik from www.flaticon.com
                draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
                settextrotation(0);

                //drawtext(xx, y + 0.00000025, name, 100000, 100000);
            } else if (x.get_width() <= 0.0075 || x.get_height() <= 0.0075) {//Icon made by Freepik from www.flaticon.com
                draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
                settextrotation(0);

                drawtext(xx, y + 0.00000025, name, 100000, 100000);
            } else if (x.get_width() <= 0.0025 || x.get_height() <= 0.0025) {//Icon made by Freepik from www.flaticon.com
                draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);
                settextrotation(0);

                drawtext(xx, y + 0.00000025, name, 100000, 100000);
            }
        }
    }
}
// draws the Found Street if exists. Corresponds to the Find Street button/feature

void draw_found_street() {

    std::vector<unsigned> id = map->get_found_street_id();



    for (unsigned i = 0; i < id.size(); i++) {
        setcolor(RED);
        StreetSegmentInfo thisSegment = map->get_Seg_Info_v(id[i]);
        draw_seg(thisSegment, id[i]);
    }
}
// draws all intersections on the map. draws the name of the intersection,  along with their street segments if hovered over that particular intersection. 

void drawAllInt(t_bound_box x) {
    if (x.get_width() <= 0.0075 || x.get_height() <= 0.0075) {
        for (unsigned i = 0; i < map->get_number_of_intersections(); i++) {


            LatLon temp = map->get_Lat_Lon_i(i);
            float xx = LonToCar(temp.lon());
            float y = LatToCar(temp.lat());

            if (i == 75020 || i == 108860) { //test
                draw_surface(load_png_from_file("libstreetmap/resources/placeholder.png"), xx, y);

            } else {
                setcolor(PURPLE);
            }

            fillarc(xx, y, 0.0000003, 0, 360);
            if (map->get_mouse_move_x() <= (xx + 0.0000005) && map->get_mouse_move_x() >= (xx - 0.0000005) && map->get_mouse_move_y() <= (y + 0.0000005) && map->get_mouse_move_y() >= (y - 0.0000005)) {
                setcolor(LIGHTGREY);
                fillarc(xx, y, 0.0000003, 0, 360);
                if (x.get_width() <= 0.0006 || x.get_height() <= 0.0006) {
                    setcolor(PURPLE);
                    settextrotation(0);
                    std::string name = map->get_intersection_name(i) + std::to_string(i); //TEST!! added ID display
                    drawtext(xx, y + 0.00000025, name, 100000, 100000);

                    std::vector<unsigned> temp1 = map->get_intersection_street_segment(i);
                    for (unsigned j = 0; j < temp1.size(); j++) {
                        StreetSegmentInfo thisSegment = map->get_Seg_Info_v(temp1[j]);
                        setcolor(BLACK);
                        drawTextTpoint(map->get_seg_from(temp1[j]), map->get_seg_to(temp1[j]), thisSegment);


                    }

                }
            }



        }
    }

    return;
}
// if the Favourite button is clicked, this function will draw all the favourites stored (by user when they shift+clicked)

void drawAllMyFav() {
    //std::cout<<" inside drawAllMyFav"<<std::endl;
    std::vector<std::string> names = map->get_ask_fav_name();
    std::vector<t_point> location = map->get_ask_fav_loc();

    t_bound_box box = get_visible_world();
    for (unsigned i = 0; i < names.size(); i++) {

        t_point xx = location[i];
        draw_surface(load_png_from_file("libstreetmap/resources/bahen.png"), xx.x, xx.y);
        setcolor(BLACK);
        settextrotation(0);

        if ((box.get_width() <= 0.00075) || (box.get_height() <= 0.00075)) {
            drawtext(xx.x, xx.y + 0.0000002, names[i], 100000, 100000);

        }

    }

    //std::vector<unsigned> path = find_path_between_intersections(map->intersection_id_from_names[names[names.size()-2]],map->intersection_id_from_names[names[names.size()-1]],0 );
    //  drawFoundPath(path);
    // std::cout<< names[names.size()-2] << " " << names[names.size()-1] << std::endl;
}
// draws all POI when POI button is clicked. If mouse is hovered over any POI, it will light up and display its name.

void drawAllPoi(t_bound_box x) {

    if (map->get_POI_click() == true) {

        if ((x.get_width() <= 0.00075 && x.get_width() > 0.0001) || (x.get_height() <= 0.00075 && x.get_height() > 0.001)) {
            for (unsigned i = 0; i < map->get_number_of_POI(); i++) {


                LatLon temp = map->get_LatLon_P(i);
                float xx = LonToCar(temp.lon());
                float y = LatToCar(temp.lat());


                setcolor(FIREBRICK);


                fillarc(xx, y, 0.000001, 0, 360);

                if (map->get_mouse_move_x() <= (xx + 0.0000005) && map->get_mouse_move_x() >= (xx - 0.0000005) && map->get_mouse_move_y() <= (y + 0.0000005) && map->get_mouse_move_y() >= (y - 0.0000005)) {

                    setcolor(GREEN);

                    fillarc(xx, y, 0.000001, 0, 360);




                    if (x.get_width() <= 0.0002 || x.get_height() <= 0.0002) {
                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(i), 100000, 100000);

                    }
                }
            }
        } else if (x.get_width() <= 0.0001 || x.get_height() <= 0.0001) {

            for (unsigned i = 0; i < map->get_number_of_POI(); i++) {


                LatLon temp = map->get_LatLon_P(i);
                float xx = LonToCar(temp.lon());
                float y = LatToCar(temp.lat());


                setcolor(FIREBRICK);


                fillarc(xx, y, 0.00000025, 0, 360);



                if (map->get_mouse_move_x() <= (xx + 0.0000005) && map->get_mouse_move_x() >= (xx - 0.0000005) && map->get_mouse_move_y() <= (y + 0.0000005) && map->get_mouse_move_y() >= (y - 0.0000005)) {

                    setcolor(GREEN);

                    fillarc(xx, y, 0.00000035, 0, 360);




                    if (x.get_width() <= 0.0002 || x.get_height() <= 0.0002) {
                        setcolor(BLUE);
                        settextrotation(0);
                        drawtext(xx, y + 0.0000002, map->get_POI_name(i), 100000, 100000);

                    }
                }



            }
        }
    }
    return;
}
// draws closet POI and Intersection (with name) when and where user clicks with ctrl pressed.

void draw_closest_POI_and_Int(t_bound_box x) {
    float xx = map->get_mouse_click_x2();
    float y = map->get_mouse_click_y2();

    //if(xx!= 0 && y!= NULL){

    float xxx = CarxToLon(xx);
    float yy = CaryToLat(y);

    LatLon comp(yy, xxx);

    //bool x=0;

    //unsigned j = find_closest_intersection(comp);


    unsigned k = find_closest_intersection(comp);
    map->point.push_back(k);

    //if (point.size()>=2){


    std::cout << map->pathll.size() << std::endl;
    //draw_surface(load_png_from_file("libstreetmap/resources/small_image.png"), xx, y);
    //drawtext(xx, y + 0.000002, map->get_intersection_name(k), 100000, 100000);

    //}







    /*std::vector<unsigned> path = find_path_to_point_of_interest(map->intersection_id_from_names[map->k], map->get_POI_name(k), 0);
    drawFoundPath(path);
    //LatLon intersection = map->get_Lat_Lon_i(j);
    LatLon POI = map->get_LatLon_P(k);

    //float int_y = LatToCar(intersection.lat());
    //float int_x = LonToCar(intersection.lon());
    float POI_y = LatToCar(POI.lat());
    float POI_x = LonToCar(POI.lon());
    if (x.get_width() >= 0.0004 || x.get_height() >= 0.0004) {
        settextrotation(0);
        setcolor(PURPLE);
        // fillarc(int_x, int_y, 0.00001, 0, 360);
        // setcolor(BLACK);
        //drawtext(int_x, int_y + 0.000002, map->get_intersection_name(j), 100000, 100000);
        setcolor(LIMEGREEN);
        fillarc(POI_x, POI_y, 0.00001, 0, 360);
        setcolor(BLACK);
        drawtext(POI_x, POI_y + 0.000002, map->get_POI_name(k), 100000, 100000);
        setcolor(RED);
        //fillarc(xx, y, 0.00001, 0, 360);
        draw_surface(load_png_from_file("libstreetmap/resources/small_image.png"), xx, y);
        setcolor(BLACK);
        //drawtext(xx, y + 0.000002, "My Position", 100, 100);
    } else if (x.get_width() <= 0.0004 || x.get_height() <= 0.0004) {
        settextrotation(0);
        setcolor(PURPLE);
        // fillarc(int_x, int_y, 0.000001, 0, 360);
        setcolor(BLACK);
        //drawtext(int_x, int_y + 0.000002, map->get_intersection_name(j), 100000, 100000);
        setcolor(LIMEGREEN);
        fillarc(POI_x, POI_y, 0.000001, 0, 360);
        setcolor(BLACK);
        drawtext(POI_x, POI_y + 0.000002, map->get_POI_name(k), 100000, 100000);
        setcolor(RED);
        draw_surface(load_png_from_file("libstreetmap/resources/small_image.png"), xx, y);
        setcolor(BLACK);
        //drawtext(xx, y + 0.000002, "My Position", 100, 100);
    }*/



    //}
}

// start of Drawing all street segments.

void drawAllSegs(t_bound_box x) {
    unsigned totalSeg = getNumberOfStreetSegments();

    for (unsigned i = 0; i < totalSeg; i++) {

        drawSegByID(i, x);

    }
    return;
}


// Street Segments are differentiated by color and speed limit.
//also draws one way signs along street segment if ONE Way

void drawSegByID(StreetSegmentIndex street_segment_id, t_bound_box x) { //unsigned id
    StreetSegmentInfo thisSegment = getStreetSegmentInfo(street_segment_id);
    //LatLon begin = map->get_Lat_Lon_i(thisSegment.from);
    //LatLon end = map->get_Lat_Lon_i(thisSegment.to);
    setcolor(t_color(246, 246, 238, 255)); //BG COLOUR
    setlinestyle(SOLID);

    //    unsigned street_id = thisSegment.streetID;
    //double length = map->get_street_length(street_id);

    if (thisSegment.speedLimit >= 80) {//80-120
        setcolor(ORANGE);
        setlinewidth(5);
        draw_seg(thisSegment, street_segment_id);
        //if (x.get_width()<=0.01 && x.get_height()<=0.01 &&(map->get_int_click() != true)){


    } else if (thisSegment.speedLimit >= 60 && x.get_width() <= 0.01 && x.get_height() <= 0.01) {//60-79
        setcolor(WHITE);
        setlinewidth(4);

        draw_seg(thisSegment, street_segment_id);


    } else if (thisSegment.speedLimit >= 50 && x.get_width() <= 0.001 && x.get_height() <= 0.001) {//50-59
        setcolor(WHITE);
        setlinewidth(3);

        draw_seg(thisSegment, street_segment_id);


    } else if (thisSegment.speedLimit >= 40 && x.get_width() <= 0.0005 && x.get_height() <= 0.0005) {//40-49
        setcolor(WHITE);
        setlinewidth(2);

        draw_seg(thisSegment, street_segment_id);



    } else if (x.get_width() <= 0.00025 && x.get_height() <= 0.00025) {//below 40


        setcolor(WHITE);
        setlinewidth(1);

        draw_seg(thisSegment, street_segment_id);


    } else return;

    if (thisSegment.oneWay && x.get_width() <= 0.001 && x.get_height() <= 0.001) { //one way arrow
        float dx = map->get_seg_to(street_segment_id).x - map->get_seg_from(street_segment_id).x;
        float dy = map->get_seg_to(street_segment_id).y - map->get_seg_from(street_segment_id).y;
        float xAvg = 0.5 * (map->get_seg_to(street_segment_id).x + map->get_seg_from(street_segment_id).x);
        float yAvg = 0.5 * (map->get_seg_to(street_segment_id).y + map->get_seg_from(street_segment_id).y);
        setcolor(t_color(60, 60, 80, 160));
        float rotation = atan(dy / dx)*180 / PI;
        if (dx < 0) {
            rotation = rotation + 180;
        }
        settextrotation(rotation);
        drawtext(xAvg, yAvg, "→", 10000, 10000); //arrow
        setfontsize(10);
        settextrotation(0);
        // std::cout<<rotation<<std::endl;

    }

}
// differentiates between curve points and then calls the drawLineTpoint function to actually draw the segment and its name.

void draw_seg(StreetSegmentInfo thisSegment, StreetSegmentIndex street_segment_id) { //subfunction of drawSegByID



    if (thisSegment.curvePointCount == 0) {
        drawLineTPoint(map->get_seg_from(street_segment_id), map->get_seg_to(street_segment_id), thisSegment);



        drawLineTPoint(map->get_seg_from(street_segment_id), map->get_seg_to(street_segment_id), thisSegment);
    } else {


        drawLineTPoint(map->get_seg_from(street_segment_id), map->get_seg_curve_points(street_segment_id, 0), thisSegment);



        unsigned numCP = map->get_seg_curve_num(street_segment_id);

        unsigned id = 0;
        for (; id < numCP - 1;) {
            drawLineTPoint(map->get_seg_curve_points(street_segment_id, id),
                    map->get_seg_curve_points(street_segment_id, id + 1), thisSegment);
            id++;
        }
        if (thisSegment.curvePointCount == 1)
            drawLineTPoint(map->get_seg_to(street_segment_id), map->get_seg_curve_points(street_segment_id, 0), thisSegment);
        else
            drawLineTPoint(map->get_seg_to(street_segment_id), map->get_seg_curve_points(street_segment_id, id), thisSegment);

        //std::cout<<numCP<<"ok?"<<map->get_seg_curve_points(street_segment_id,id).lat()<<std::endl;



    }
}


// start of drawing all features and names

void drawAllFeatures(t_bound_box x) {
    unsigned totalFeature = getNumberOfFeatures();
    for (unsigned i = 0; i < totalFeature; i++) {
        drawFeatureByID(i, x);

    }
    for (unsigned i = 0; i < totalFeature; i++) {

        drawFeature_name(i);

    }
}
// draws all features. First differentiates by ID and sets color.
// also draws name of the feature if the mouse is hovered on the line if open

void drawFeatureByID(FeatureIndex ID, t_bound_box x) {
    unsigned num = map->get_feature_pointnum(ID);

    float maxx = -1000;
    float minx = 1000;
    float maxy = -1000;
    float miny = 1000;

    t_point polypts[num];
    for (unsigned p = 0; p < num; ++p) {
        LatLon thepoint = map->get_feature_points(ID, p);
        float iy = LatToCar(thepoint.lat());
        float ix = LonToCar(thepoint.lon());

        polypts[p] = {ix, iy};

        if (ix > maxx)
            maxx = ix;
        if (ix < minx)
            minx = ix;
        if (iy < miny)
            miny = iy;
        if (iy > maxy)
            maxy = iy;
    }




    if (getFeatureType(ID) == 7 && x.get_width() > 0.001 && x.get_height() > 0.001)//don`t draw building when zoom out
        return;

    if (getFeatureType(ID) == 1 || getFeatureType(ID) == 9) {//park/golf
        setcolor(t_color(192, 236, 174, 255));
    } else if (getFeatureType(ID) == 2) {
        setcolor(t_color(250, 243, 199, 255));
    } else if (getFeatureType(ID) == 3 || getFeatureType(ID) == 4) {//lake/river
        setcolor(t_color(167, 192, 224, 255));
    } else if (getFeatureType(ID) == 5 || getFeatureType(ID) == 6) {//island
        setcolor(t_color(230, 220, 189, 255));
    } else if (getFeatureType(ID) == 7) {//building
        setcolor(LIGHTGREY);
    } else if (getFeatureType(ID) == 8) {//greenspace
        setcolor(t_color(206, 230, 189, 255));
    } else if (getFeatureType(ID) == 10) {//stream
        setcolor(t_color(170, 218, 255, 255));
    } else if (getFeatureType(ID) == 0) {
        setcolor(BLUE);
    }


    if (polypts[0].x == polypts[num - 1].x && polypts[0].y == polypts[num - 1].y) {
        fillpoly(polypts, num);
        t_color curr_color = getcolor();
        setcolor(DARKGREY);
        if (getFeatureType(ID) == 7 && x.get_width() < 0.0005 && x.get_height() < 0.0005) {
            for (unsigned IDline = 0; IDline + 1 < num; IDline++) {
                drawLineTPoint_f(polypts[IDline], polypts[IDline + 1], ID);
            }
        }


        setcolor(curr_color);
    } else {
        for (unsigned IDline = 0; IDline + 1 < num; IDline++) {
            drawLineTPoint_f(polypts[IDline], polypts[IDline + 1], ID);

        }
    }


}
//draws feature names for closed features

void drawFeature_name(FeatureIndex ID) {
    unsigned num = map->get_feature_pointnum(ID);

    float maxx = -1000;
    float minx = 1000;
    float maxy = -1000;
    float miny = 1000;

    t_point polypts[num];
    for (unsigned p = 0; p < num; ++p) {
        LatLon thepoint = map->get_feature_points(ID, p);
        float iy = LatToCar(thepoint.lat());
        float ix = LonToCar(thepoint.lon());

        polypts[p] = {ix, iy};

        if (ix > maxx)
            maxx = ix;
        if (ix < minx)
            minx = ix;
        if (iy < miny)
            miny = iy;
        if (iy > maxy)
            maxy = iy;
    }


    if (polypts[0].x == polypts[num - 1].x && polypts[0].y == polypts[num - 1].y) {

        t_color curr_color = getcolor();
        std::string name = map->get_feature_name(ID);

        if (map->get_mouse_move_x() <= (maxx) && map->get_mouse_move_x() >= (minx) && map->get_mouse_move_y() <= (maxy) && map->get_mouse_move_y() >= (miny)) {

            setcolor(BLACK);
            settextrotation(0);
            drawtext((maxx + minx) / 2, (maxy + miny) / 2, name, 100, 100);



        }
        setcolor(curr_color);
    }
}


// drawing the line and name(calling another fcn). Used for drawing street segments.
// name is drawn only if mouse hovers over the segment.

void drawLineTPoint(t_point A, t_point B, StreetSegmentInfo x) {
    drawline(A.x, A.y, B.x, B.y);

    float xxl = -1000;
    float xxs = 1000;
    float yyl = -1000;
    float yys = 1000;
    if (A.x > B.x) {
        xxl = A.x;
        xxs = B.x;
    } else if (B.x > A.x) {
        xxl = B.x;
        xxs = A.x;
    }

    if (A.y > B.y) {
        yyl = A.y;
        yys = B.y;
    } else if (B.y > A.y) {
        yyl = B.y;
        yys = A.y;
    }
    t_color curr_color = getcolor();
    if (map->get_mouse_move_x() <= (xxl) && map->get_mouse_move_x() >= (xxs) && map->get_mouse_move_y() <= (yyl) && map->get_mouse_move_y() >= (yys) && map->get_int_click() != true) {
        setcolor(BLACK);
        drawTextTpoint(A, B, x); // draw text fcn
    }

    setcolor(curr_color);

    return;
}
// drawing the line and name(calling another fcn). Used for drawing open features.
// name is drawn only if mouse hovers over the feature line.

void drawLineTPoint_f(t_point A, t_point B, FeatureIndex ID) {
    drawline(A.x, A.y, B.x, B.y);
    float xxl = -1000;
    float xxs = 1000;
    float yyl = -1000;
    float yys = 1000;
    if (A.x > B.x) {
        xxl = A.x;
        xxs = B.x;
    } else if (B.x > A.x) {
        xxl = B.x;
        xxs = A.x;
    }

    if (A.y > B.y) {
        yyl = A.y;
        yys = B.y;
    } else if (B.y > A.y) {
        yyl = B.y;
        yys = A.y;
    }
    t_color curr_color = getcolor();
    if (map->get_mouse_move_x() <= (xxl) && map->get_mouse_move_x() >= (xxs) && map->get_mouse_move_y() <= (yyl) && map->get_mouse_move_y() >= (yys) && map->get_int_click() != true && getFeatureType(ID) != 7) {
        setcolor(BLACK);
        drawTextTpoint_f(A, B, ID); // draw text for feature fcn
    }

    setcolor(curr_color);

    return;
}



// draw Text function for segments.
// sets angle of segment first and then uses "drawtext"

void drawTextTpoint(t_point A, t_point B, StreetSegmentInfo x) {
    double angle;
    angle = atan((B.y - A.y) / (B.x - A.x)) / DEG_TO_RAD;
    settextrotation(angle);
    std::string name = getStreetName(x.streetID);
    int length = (x.speedLimit);

    std::string len;


    len = name + " -- " + std::to_string(length) + " km/h" + " ";



    drawtext((A.x + B.x) / 2, (B.y + A.y) / 2, len, 100, 100);

}
// draw Text function for open features.
// sets angle of feature first and then uses "drawtext"

void drawTextTpoint_f(t_point A, t_point B, FeatureIndex ID) {
    double angle;
    angle = atan((B.y - A.y) / (B.x - A.x)) / DEG_TO_RAD;
    settextrotation(angle);
    std::string name = map->get_feature_name(ID);
    drawtext((A.x + B.x) / 2, (B.y + A.y) / 2, name, 100, 100);

}
int zone;

std::string timeZone(std::string name) {



    if (name == "Toronto") {
        zone = -4;

    }
    if (name == "Beijing") {
        zone = +8;

    }
    if (name == "Cairo") {
        zone = +2;
    }
    if (name == "Capetown") {
        ;
        zone = 2;
    }
    if (name == "GoldenHorseshoe") {

        zone = -4;
    }
    if (name == "Hamilton") {

        zone = -4;
    }
    if (name == "Hongkong") {

        zone = 8;
    }
    if (name == "Iceland") {

        zone = 0;
    }
    if (name == "Interlaken") {

        zone = +1;
    }
    if (name == "London") {

        zone = 0;
    }
    if (name == "Moscow") {

        zone = 3;
    }

    if (name == "NewYork") {

        zone = -4;
    }
    if (name == "RioDeJaneiro") {

        zone = -3;
    }
    if (name == "SaintHelena") {

        zone = 0;
    }
    if (name == "Singapore") {

        zone = 8;
    }
    if (name == "Sydney") {

        zone = +11;
    }

    if (name == "Tokyo") {

        zone = 9;
    }
    return 0;
}
//this function outputs the the local time and its displayed on bottom right corner

void drawclock() {

    time_t myTime;
    struct tm * ptm;

    char buffer[80];

    time(&myTime);
    //   std::cout<<asctime(gmtime(&myTime));
    ptm = gmtime(&myTime);
    ptm->tm_hour = ptm->tm_hour - 4;
    strftime(buffer, sizeof (buffer), "%d-%m-%Y %I:%M:%S\n", ptm);
    std::string str(buffer);
    set_coordinate_system(GL_SCREEN);
    settextrotation(0);
    setfontsize(12);
    setcolor(t_color(180, 150, 100, 100));
    setlinestyle(SOLID);
    t_point BGrange7[4] = {
        {835, 680},
        {835, 700},
        {986, 700},
        {986, 680}
    }; //background
    fillpoly(BGrange7, 4);

    setcolor(BLACK);
    drawtext(910, 690, str);

}
// draws legend on screen when Legend button is clicked. is drawn on the screen and doesnt get affected by Zoom

void drawInstruction() {
    drawclock();
    set_coordinate_system(GL_SCREEN);
    settextrotation(0);
    setfontsize(10);
    setcolor(t_color(100, 100, 180, 120));
    setlinestyle(SOLID);
    t_point BGrange[4] = {
        {5, 10},
        {5, 130},
        {200, 130},
        {200, 10}
    }; //background
    fillpoly(BGrange, 4);
    //drawrect(10, 10, 100, 100);
    // Specifying FLT_MAX for the size limits on the text (in world coordinates) so
    // it will always be drawn.
    setcolor(WHITE);
    drawtext(100, 20, "F - Toggle Features");
    drawtext(100, 35, "P - Toggle All POI");
    drawtext(100, 50, "I - Toggle Intersection");
    drawtext(100, 65, "ctrl+click - closest POI and Int");
    drawtext(100, 110, "X - remove closest POI and INT");
    drawtext(100, 80, "shift+click - add to favorites");
    drawtext(100, 95, "D - to remove 'found' drawings ");
    drawtext(100, 125, "L - close Legend");

    setcolor(t_color(100, 100, 180, 120));
    t_point BGrange2[4] = {
        {1000, 10},
        {1000, 130},
        {1200, 130},
        {1200, 10}
    };
    fillpoly(BGrange2, 4);
    setcolor(WHITE);
    drawtext(1100, 20, "Types of Locations");
    drawtext(1100, 35, "1 - Places of Education");
    drawtext(1100, 50, "2 - Places to Eat");
    drawtext(1100, 65, "3 - Bus Terminal /Car Sharing");
    drawtext(1100, 80, "4 - HealthCare Services");
    drawtext(1100, 95, "5 - Emergency Services");
    drawtext(1100, 110, "6 - Recreation");
    drawtext(1100, 125, "7 - Subway Entrance");


    t_point BGrange3[4] = {
        {500, 500},
        {500, 600},
        {1200, 600},
        {1200, 500}
    };
    if (map->get_load_another_map()) {
        setcolor(RED);
        fillpoly(BGrange3, 4);
        setcolor(WHITE);
        setfontsize(20);
        drawtext(810, 550, "To Reload Map, Close the Window");
        drawtext(810, 575, "DO NOT CLICK EXIT");
        setfontsize(10);
    }
    set_coordinate_system(GL_WORLD);
}
// LatLon to Cartestian conversions

void drawFoundPath(const std::vector<unsigned>& segID) { //Draw func for m3,
    unsigned segNum = segID.size(); //total number of segs
    if (segNum == 0) {
        set_coordinate_system(GL_SCREEN);
        t_point topLeft = t_point(5, 160); //top left corner
        t_bound_box InstructionBox = t_bound_box(topLeft, 300, 12);
        setcolor(t_color(27, 54, 92, 240)); //Box colour
        fillrect(InstructionBox);
        setcolor(t_color(228, 201, 157, 255)); //text colour
        setfontsize(10);
        settextrotation(0);
        int txtPosition = 155;
        drawtext(160, txtPosition + 12, "not found");
        set_coordinate_system(GL_WORLD);
    } else if (segNum != 0) {
        setcolor(t_color(210, 90, 230, 255)); //line colour
        setlinewidth(8); //line width
        unsigned previousStreetID = getStreetSegmentInfo(segID[0]).streetID;
        unsigned currentStreetID;
        unsigned numOfTurns = 0;
        std::vector<char>turnDirection;
        std::vector<std::string>turnStreetName;

        //First draw all the lines,and record Turn numbers
        for (unsigned iSeg = 0; iSeg < segNum; iSeg++) {
            StreetSegmentInfo currentSegInfo = getStreetSegmentInfo(segID[iSeg]);
            draw_seg(currentSegInfo, segID[iSeg]);
            currentStreetID = getStreetSegmentInfo(segID[iSeg]).streetID;

            if (iSeg == (segNum - 1) && segNum > 1) {//draw finishing flag
                t_point destNode;
                //std::cout<<"cccc";
                if (map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).to).lat() == map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).to).lat()
                        || map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).from).lat() == map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).to).lat()) {
                    LatLon destLL = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).from);
                    destNode.y = LatToCar(destLL.lat());
                    destNode.x = LatToCar(destLL.lon());
                    //std::cout<<"aaaaa";

                }
                if (map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).to).lat() == map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).from).lat()
                        || map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).from).lat() == map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).from).lat()) {
                    LatLon destLL = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).to);
                    destNode.y = LatToCar(destLL.lat());
                    destNode.x = LatToCar(destLL.lon());
                    //std::cout<<"bbbbb";

                }
                draw_surface(load_png_from_file("libstreetmap/resources/finishFlg.png"), destNode.x, destNode.y);

            }//end of draw finish flg

            if (currentStreetID != previousStreetID) { //If A Turn Is Made
                LatLon FromInt;
                LatLon MidInt;
                LatLon ToInt; //now determine which is which
                //numOfTurns+=1;
                if (getStreetSegmentInfo(segID[iSeg - 1]).to == getStreetSegmentInfo(segID[iSeg]).from) {
                    MidInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).to);
                    FromInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).from);
                    ToInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).to);
                } else if (getStreetSegmentInfo(segID[iSeg - 1]).from == getStreetSegmentInfo(segID[iSeg]).from) {
                    MidInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).from);
                    FromInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).to);
                    ToInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).to);
                } else if (getStreetSegmentInfo(segID[iSeg - 1]).from == getStreetSegmentInfo(segID[iSeg]).to) {
                    MidInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).from);
                    FromInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg - 1]).to);
                    ToInt = map->get_Lat_Lon_i(getStreetSegmentInfo(segID[iSeg]).from);
                }
                double crossI = (MidInt.lon() - FromInt.lon())*(ToInt.lat() - MidInt.lat());
                double crossJ = (MidInt.lat() - FromInt.lat())*(ToInt.lon() - MidInt.lon());
                //do cross product to determine left or right 
                if ((crossI + crossJ) < 0)
                    turnDirection.push_back('r');
                else
                    turnDirection.push_back('l');

                turnStreetName.push_back(getStreetName(getStreetSegmentInfo(segID[iSeg]).streetID));
                previousStreetID = currentStreetID;
                numOfTurns++;
            }
            //now draw instructions
            //now draw instructions
            /*set_coordinate_system(GL_SCREEN);
            t_point topLeft =t_point(5,160); //top left corner
            t_bound_box InstructionBox = t_bound_box(topLeft, 300, 12*numOfTurns+30);
            setcolor(t_color(27,54,92,240));//Box colour
            fillrect(InstructionBox);
            setcolor(t_color(228,201,157,255));//text colour
            setfontsize(10);
            settextrotation(0);
            int txtPosition = 155 + 12*numOfTurns+24;
            drawtext(160,txtPosition,"Starting");
            txtPosition -= 12; 
            for(unsigned i= 0; i<numOfTurns; i++){
                if(turnDirection[i]=='l')
                    drawtext(30,txtPosition,"<-Left");
                else{
                    drawtext(30,txtPosition,"Right->");}
                drawtext(170,txtPosition,turnStreetName[i]);        
                txtPosition -= 12;           
            }
            drawtext(160,txtPosition,"Reached Destination");
            set_coordinate_system(GL_WORLD);
            }*/
        }
        //now draw instructions
        set_coordinate_system(GL_SCREEN);
        t_point topLeft = t_point(5, 160); //top left corner
        t_bound_box InstructionBox = t_bound_box(topLeft, 300, 12 * numOfTurns + 30);
        setcolor(t_color(27, 54, 92, 240)); //Box colour
        fillrect(InstructionBox);
        setcolor(t_color(228, 201, 157, 255)); //text colour
        setfontsize(10);
        settextrotation(0);
        int txtPosition = 155 + 12 * numOfTurns + 24;
        drawtext(160, txtPosition, "Starting");
        txtPosition -= 12;
        for (unsigned i = 0; i < numOfTurns; i++) {
            if (turnDirection[i] == 'l')
                drawtext(30, txtPosition, "<-Left");
            else {
                drawtext(30, txtPosition, "Right->");
            }
            drawtext(170, txtPosition, turnStreetName[i]);
            txtPosition -= 12;
        }

        std::string j = std::to_string(numOfTurns);
        drawtext(160, txtPosition, "Reached Destination");
        set_coordinate_system(GL_WORLD);
    }




}//Draw all path seg, and draw instructions

float LatToCar(double LatY) {
    float Y = LatY*DEG_TO_RAD;
    return Y;
}

float LonToCar(double LonX) {
    float X = LonX * cos(LatAvgMap) * DEG_TO_RAD;
    return X;
}

// Cartesian to Latlon conversions

float CarxToLon(float X) {
    float Lon = X / cos(LatAvgMap) / DEG_TO_RAD;
    return Lon;
}

float CaryToLat(float Y) {
    float Lat = Y / DEG_TO_RAD;
    return Lat;
}

void setClr() {
    /*t_color SeaClr =t_color(167,192,224,255) ;
    //t_color BuildingClr =t_color(167,192,224,255);
    t_color StreamClr = t_color(170,218,255,255);
    t_color GreenClr =t_color(206,230,189,255);
    t_color ParkClr =t_color(192,236,174,255);
    t_color BeachClr =t_color(250,243,199,255);
    t_color HWClr =t_color(246,184,116,255);
    t_color RoadClr =t_color(254,246,214,255);
    return;*/
}

void drawWalkingDistance() {
    //parameter: location of nearest intersection to poi , poi location

    //find distance between 2 to find the length of walking distance

    //convert parameters to x,y
    setcolor(t_color(210, 90, 230, 255));
    setlinewidth(5); //line width
    unsigned i = 432;
    unsigned j;
    StreetSegmentInfo seginfo;
    LatLon p = getPointOfInterestPosition(j);





}
