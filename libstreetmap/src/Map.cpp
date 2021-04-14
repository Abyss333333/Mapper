#include <algorithm>
#include "OSMDatabaseAPI.h"
#include "Map.h"
#include "m2.h"
#include "fontcache.h"

// all make functions are containers outside of "make_number_of_intersections_and_POI"
// all get functions are to access the private variable members in the class

Map::Map() {

    make_street_seg_length_and_travel();
    make_LatLon_P();
    make_intersection_street_segment_and_Lat_Lon_i();
    make_street_segment_and_Seg_Info();
    make_number_of_intersections_and_POI();
    make_street_ids_from_names();
    make_street_inter_id();
    make_POI_name();
    make_intersection_names();
    make_feature_name();
    make_POI_type();
    //OSM_database();//FOR TESTER COMMENT THIS OUT
    
    //-----below are drawmap related
    make_feature_num_and_points();
    make_streetseg_position();
    make_adjacent_intersections();
    

    POI_Click = false;
    feat_Click = true;
    int_Click= false;
    mouse_click = false;
    mouse_click_x = 1;
    mouse_click_y = 1;
    mouse_click2 = false;
    mouse_click_x2 = 1;
    mouse_click_y2 = 1;
    search_click = false;
    mouse_move_x = 1;
    mouse_move_y= 1;
    draw_int= false;
    ed_click=false;
    food_click= false;
    legend_click= true;
    trans_click= false;
    med_click=false;
    emergency_click=false;
    fun_click = false;
    draw_found_POI= false;
    draw_found_street=false;
    search_POI= false;
    search_street= false;
    search_history_click=false;
    search_f = false;
    help_click = false;
    
    load_another_map = false;
    Fav_click = false;
    sub_click = false;
    second_bar = false;

   //make_street_length(); 
    make_from_to();
    
    make_nodes_and_edges();
    
}

Map::~Map() {
    void clear_structure();
}

void Map::clear_structure() { // clearing all containers
    intersection_street_segment.clear();
    street_street_segment.clear();
    street_ids_from_names.clear();
    street_inter_id.clear();
    travel.clear();
    street_seg_length.clear();
    Lat_Lon_i.clear();
    Lat_Lon_p.clear();
    Seg_Info_v.clear();
    number_of_intersections = 0;
    number_of_POI = 0;
    feature_pointnum.clear();
    feature_points.clear();
    curve_seg_points.clear();
    seg_curve_num.clear();
    seg_from.clear();
    seg_to.clear();
    street_length.clear();
    type_education.clear();
    type_food.clear();
    type_transport.clear();
    type_med.clear();
    type_emergency.clear();
    type_fun.clear();
    POI_id_from_name.clear();
    street_name_street_segment.clear();
    found_POI_id.clear();
    found_street_segment_ids.clear();
    search_history.clear();
    POI_names.clear();
    found_intersection.clear();
    intersection_name.clear();
    feature_name.clear();
    POI_type.clear();
    ask_fav_name.clear();
    ask_fav_loc.clear();
    OSM_vector.clear();
    feature_id_from_name.clear();
    from_to_int_travel_time.clear();
    from_to_int_street_id.clear();
    adj_int.clear();
    adj_int_opp.clear();
    intersection_id_from_names.clear();
    
    from_to_int_street_seg_id1.clear();
    k.clear();
    s.clear();
    
    nodes.clear();
    edges.clear();
    
    point.clear();
    pathll.clear();
    
}

double Map::distance_calc(LatLon x, LatLon y) { // distance function
    double y1 = x.lat() * DEG_TO_RAD;
    double y2 = y.lat() * DEG_TO_RAD;
    double LatAvg = (y1 + y2) / 2;
    double x1 = x.lon() * cos(LatAvg) * DEG_TO_RAD;
    double x2 = y.lon() * cos(LatAvg) * DEG_TO_RAD;
    double l = sqrt((y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1)) * EARTH_RADIUS_IN_METERS;

    return l;
}

void Map::make_number_of_intersections_and_POI() {
    number_of_intersections = getNumberOfIntersections();
    number_of_intersectionss= number_of_intersections;
    number_of_POI = getNumberOfPointsOfInterest();
    number_of_features = getNumberOfFeatures();
    

}

unsigned Map::get_number_of_intersections() {
    return number_of_intersections;
}

unsigned Map::get_number_of_POI() {
    return number_of_POI;
}

void Map::set_draw_int(bool x){
    draw_int = x;
}
bool Map:: get_draw_int(){
    return draw_int;
}

void Map::OSM_database(){
    unsigned num = getNumberOfNodes();
    for (unsigned i=0; i<num; i++){
        const OSMNode* e= getNodeByIndex(i);
        std::string key,value;
        unsigned tagnum = getTagCount(e);
        for (unsigned t=0; t< tagnum; t++){
            std::tie(key,value) = getTagPair(e,t);
            
            if (value == "subway_entrance"){
                LatLon tx = e->coords();
                OSM_vector.push_back(tx);
                
            }
        }
    }
    
    
}



std::vector<LatLon> Map:: get_OSM_vector(){
    return OSM_vector;
}



std::vector<unsigned> Map::unique_vector(std::vector<unsigned> tempo) { // return unique vector
    unsigned i;
    unsigned j;
    bool x;
    std::vector<unsigned> unique;
    for (i = 0; i < tempo.size(); i++) {
        for (j = 0; j < i; j++) {
            if (tempo [j] == tempo[i]) {
                x = false;
            }
        }
        if (x)
            unique.push_back(tempo[i]);

        x = true;

    }
    return unique;
}

std::vector<unsigned> Map::unique_vector_from_two_vectors(std::vector<unsigned> tempx, std::vector<unsigned> temp2) {

    std::unordered_map <unsigned, unsigned> check; // hash used to check for duplicate keys
    unsigned i;
    unsigned j;
    std::vector<unsigned> ret;


    for (i = 0; i < tempx.size(); i++) {
        for (j = 0; j < temp2.size(); j++) {
            if (temp2[j] == tempx[i]) {

                auto search = check.find(tempx[i]);
                if (search == check.end()) {
                    check.emplace(tempx[i], 1);
                    ret.push_back(tempx[i]);
                }

            }
        }
    }

    return ret;
}

void Map::set_temp_LatLon(LatLon x){
    temporary =x;
}
LatLon Map::get_temp_Latlon(){
    return temporary;
}

void Map::make_street_seg_length_and_travel() {
        

    unsigned i;
    StreetSegmentInfo Seg_Info;
    for (i = 0; i < getNumberOfStreetSegments(); i++) {
        Seg_Info = getStreetSegmentInfo(i);
        LatLon begin = getIntersectionPosition(Seg_Info.from);
        LatLon end = getIntersectionPosition(Seg_Info.to);
        double l = 0;
        double speed_limit = Seg_Info.speedLimit;

        if (Seg_Info.curvePointCount == 0) {

            l = distance_calc(begin, end);
            street_seg_length.push_back (l);

            double time = (3.6 * l) / speed_limit;
            travel.push_back(time);

        }
        else {
            LatLon zero = getStreetSegmentCurvePoint(i, 0);
            l = distance_calc(begin, zero);
    
            unsigned q = 0;
            LatLon LL1 = getStreetSegmentCurvePoint(i, q);
            while ((q + 1 < Seg_Info.curvePointCount)) {
                LatLon LL2 = getStreetSegmentCurvePoint(i, q + 1);

                double tempx = distance_calc(LL1, LL2);

                l += tempx;
                q = q + 1;
                LL1 = LL2;
            }

            LatLon LL3 = getStreetSegmentCurvePoint(i, q);


            double temp1 = distance_calc(LL3, end);
            l += temp1;

            double time = (3.6 * l) / speed_limit;


            street_seg_length.push_back( l);
            travel.push_back(time);
        }

    }
}

double Map::get_street_seg_length(unsigned i) {
    return street_seg_length[i];
}

double Map::get_travel(unsigned i) {
    return travel[i];
}

void Map::make_intersection_street_segment_and_Lat_Lon_i() {

    unsigned i;
    unsigned x;
    double min_lat = getIntersectionPosition(0).lat();
    double max_lat = min_lat;
    for (i = 0; i < getNumberOfIntersections(); ++i) {
        intersection_street_segment.push_back(std::vector<unsigned>());
        LatLon Comp = getIntersectionPosition(i);
        Lat_Lon_i.push_back(Comp);
        max_lat = std::max(max_lat, getIntersectionPosition(i).lat());
        min_lat = std::min(min_lat, getIntersectionPosition(i).lat());

        for (x = 0; x < getIntersectionStreetSegmentCount(i); ++x) {
            intersection_street_segment[i].push_back(getIntersectionStreetSegment(i, x));
        }
    }
    LatAvgMap = (min_lat + max_lat) *DEG_TO_RAD/ 2;
}

void Map::make_LatLon_P() {
    unsigned i;
    for (i = 0; i < getNumberOfPointsOfInterest(); i++) {
        LatLon Comp = getPointOfInterestPosition(i);
        Lat_Lon_p.push_back(Comp);
        
        
    }
}

LatLon Map::get_LatLon_P(unsigned i) {
    return Lat_Lon_p[i];
}





LatLon Map::get_Lat_Lon_i(unsigned i) {
    return Lat_Lon_i[i];
}

std::vector<unsigned> Map::get_intersection_street_segment(unsigned i) {
    return intersection_street_segment[i];
}

void Map::make_street_segment_and_Seg_Info() {
    StreetSegmentInfo Seg_Info;
    unsigned i;

    for (i = 0; i < getNumberOfStreetSegments(); i++) {
        Seg_Info = getStreetSegmentInfo(i);
        std::string name = getStreetName(Seg_Info.streetID);
        
        Seg_Info_v.push_back(Seg_Info); // vector of seg infos.
        auto search = street_street_segment.find(Seg_Info.streetID);
        if (search != street_street_segment.end()) {
            std::vector <unsigned> temp;
            std::vector <unsigned> temp2;
            street_street_segment.emplace(Seg_Info.streetID, temp);
            street_street_segment[Seg_Info.streetID].push_back(i);

        } else{
            street_street_segment[Seg_Info.streetID].push_back(i);
        }
        
        auto search1 = street_name_street_segment.find(name);
        if (search1 != street_name_street_segment.end()) {
            std::vector <unsigned> temp1;
            
            street_name_street_segment.emplace(name, temp1);
            street_name_street_segment[name].push_back(i);

        } else
            street_name_street_segment[name].push_back(i);
    
    
    }   
}

StreetSegmentInfo Map::get_Seg_Info_v(unsigned i) {
    return Seg_Info_v[i];
}

std::vector<unsigned> Map::get_street_street_segment(unsigned i) {
    return street_street_segment[i];
}
std::vector<unsigned> Map::get_street_name_street_segment(std::string i) {
    return street_name_street_segment[i];
}
void Map::make_POI_name() {
    unsigned POI = get_number_of_POI();
    std::string name;
    for (unsigned i = 0; i < POI; i++) {
        name = getPointOfInterestName(i);
        POI_names.push_back(name);
        auto search = POI_id_from_name.find(name);
        if (search != POI_id_from_name.end()) {
            std::vector <unsigned> temp;
            
            POI_id_from_name.emplace(name, temp);
            POI_id_from_name[name].push_back(i);

        } else
            POI_id_from_name[name].push_back(i);
        
    }

}

std::vector<unsigned>  Map::get_POI_id_from_name(std::string x){
    return POI_id_from_name[x];
}

void Map::make_POI_type() {
    unsigned POI = get_number_of_POI();
    std::string name;
    for (unsigned i = 0; i < POI; i++) {
        name = getPointOfInterestType(i);
        POI_type.push_back(name);
        
        if (name == "school" || name == "college" || name== "library" || name == "music_school" || name == "student_union" || name == "tutoring" ||name == "university" ||name == "education" ||name == "preschool" ||name == "kindergarten" ||name == "language_school" ){
           
            type_education.push_back(i);
        }
        
        if (name == "fast_food" ||name == "cafe" ||name == "pub" ||name == "restaurant" || name == "food_court" || name == "bar" || name == "bakery" ||name == "ice_cream" ){
            type_food.push_back(i);
        }
        
        if (name == "bus_station" || name == "subway_station" || name == "taxi" || name == "car_rental" || name == "bike_rental" || name == "car_sharing" ){
            type_transport.push_back(i);
        }
        if (name == "hospital" || name == "pharmacy" || name == "dentist" || name == "massage" || name == "doctors" || name == "orthodontist" || name == "optometrist" ||name == "chiropractor" ||name == "veterinary" ||name == "hair_loss" ||name == "laser_eye_surgery" ||name == "psychologist" ){
            type_med.push_back(i);
        }
        if (name == "hospital" || name == "ambulance station" || name == "ambulance_station" || name == "fire_station" || name == "police" ){
            type_emergency.push_back(i);
        }
        if (name == "night club" ||name == "cinema" || name == "strip club" ||name == "night_club" || name == "billiards" || name == "music_venue" ||name == "comedy_club" ||name == "laser_tag" || name == "internet_cafe" ||name == "dance_studio" ||name == "gym" ||name == "fitness_centre"){
            type_fun.push_back(i);
        }
    }
}


std::vector<unsigned> Map:: get_type_education(){
    return type_education;
}

std::vector<unsigned> Map:: get_type_food(){
    return type_food;
}

std::vector<unsigned> Map:: get_type_trans(){
    return type_transport;
}

std::vector<unsigned> Map:: get_type_med(){
    return type_med;
}

std::vector<unsigned> Map:: get_type_emergency(){
    return type_emergency;
}

std::vector<unsigned> Map:: get_type_fun(){
    return type_fun;
}

std::string Map::get_POI_name(unsigned i) {
    return POI_names[i];
}


std::string Map::get_POI_type(unsigned i){
    return POI_type[i];
}

bool Map::get_Fav_click()
{
    return Fav_click;
}
void Map::set_Fav_click(bool x)
{
    Fav_click =x;
}

bool Map::get_POI_click() {

    return POI_Click;
}

void Map::set_POI_click(bool x) {
    POI_Click = x;
}

bool Map::get_legend_click() {

    return legend_click;
}

void Map::set_legend_click(bool x) {
    legend_click = x;
}

bool Map::get_help_click() {

    return help_click;
}

void Map::set_help_click(bool x) {
    help_click = x;
}

bool Map::get_emergency_click() {

    return emergency_click;
}

void Map::set_emergency_click(bool x) {
    emergency_click = x;
}

bool Map::get_ed_click() {

    return ed_click;
}

void Map::set_ed_click(bool x) {
    ed_click = x;
}

bool Map::get_fun_click() {

    return fun_click;
}

void Map::set_fun_click(bool x) {
    fun_click = x;
}

bool Map::get_sub_click() {

    return sub_click;
}

void Map::set_sub_click(bool x) {
    sub_click = x;
}

bool Map::get_food_click() {

    return food_click;
}

void Map::set_food_click(bool x) {
    food_click = x;
}

bool Map::get_trans_click() {

    return trans_click;
}

void Map::set_trans_click(bool x) {
    trans_click = x;
}

bool Map::get_med_click() {

    return med_click;
}

void Map::set_med_click(bool x) {
    med_click = x;
}
bool Map::get_search_click() {

    return search_click;
}

void Map::set_search_click(bool x) {
    search_click = x;
}

bool Map::get_search_f() {

    return search_f;
}

void Map::set_search_f(bool x) {
    search_f = x;
}

bool Map::get_search_history_click()
{
    return search_history_click;
}
   void Map::set_search_history_click(bool x)
    {
        search_history_click =x;
    }

bool Map::get_search_POI() {

    return search_POI;
}

void Map::set_search_POI(bool x) {
    search_POI = x;
}

bool Map::get_search_street() {

    return search_street;
}

void Map::set_search_street(bool x) {
    search_street = x;
}

bool Map::get_int_click() {
    return int_Click;
}

void Map::set_int_click(bool x) {
    int_Click = x;
}

std::vector<std::string> Map:: get_ask_fav_name(){
    return ask_fav_name;
}
void Map::set_ask_fav_name(std::string x){
    ask_fav_name.push_back(x);
}

std::vector<t_point> Map:: get_ask_fav_loc(){
    return ask_fav_loc;
}
void Map::set_ask_fav_loc(t_point x){
    ask_fav_loc.push_back(x);
}

bool Map::get_load_another_map() {
    return load_another_map;
}

void Map::set_load_another_map(bool x) {
    load_another_map = x;
}

bool Map::get_feat_click() {
    return feat_Click;
}

void Map::set_feat_click(bool x) {
    feat_Click = x;
}

bool Map::get_mouse_click() {
    return mouse_click;
}

void Map::set_mouse_click(bool x) {
    mouse_click = x;
}

float Map::get_mouse_click_x(){
    return mouse_click_x;
}
    
void Map::set_mouse_click_x(float x){
   // std::cout<<"set_mouse_click_x "<<std::endl;
    mouse_click_x = x;
}


float Map::get_mouse_click_y(){
    return mouse_click_y;
}
    
void Map::set_mouse_click_y(float y){
    mouse_click_y = y;
}

//

float Map::get_mouse_move_y(){
    return mouse_move_y;
}
    
void Map::set_mouse_move_y(float y){
    mouse_move_y = y;
}

float Map::get_mouse_move_x(){
    return mouse_move_x;
}
    
void Map::set_mouse_move_x(float x){
    mouse_move_x = x;
}

//aa

bool Map::get_mouse_click2() {
    return mouse_click2;
}

void Map::set_mouse_click2(bool x) {
    mouse_click2 = x;
}

void Map::set_mouse_click_x2(float x){
   // std::cout<<" location x to be saved"<<x<<std::endl;
    mouse_click_x2 = x;
}
float Map::get_mouse_click_x2(){
    return mouse_click_x2;
}
    

void Map::set_mouse_click_y2(float y){
    mouse_click_y2 = y;
}


float Map::get_mouse_click_y2(){
    return mouse_click_y2;
}
void Map::set_search_histor(std::string data)
{
    search_history.push_back(data);
}
std::vector<std::string> Map::get_search_histor()
{
    return search_history;
}
void Map::set_clear_history()
{
    search_history.clear();
}

void Map::set_draw_found_POI(bool x){
    draw_found_POI = x;
}
bool Map::get_draw_found_POI(){
    return draw_found_POI;
}

void Map::set_draw_found_street(bool x){
    draw_found_street = x;
}
bool Map::get_draw_found_street(){
    return draw_found_street;
}

 void Map::set_found_POI_id(std::vector<unsigned> x){
     found_POI_id =x;
 }
std::vector<unsigned>  Map:: get_found_POI_id(){
    return found_POI_id;
}

void Map::set_found_street_id(std::vector<unsigned> x){
     found_street_segment_ids =x;
 }
std::vector<unsigned>  Map:: get_found_street_id(){
    return found_street_segment_ids;
}

void Map::make_intersection_names() {
    std::string name;
    unsigned NOI = number_of_intersections;

    for (unsigned i = 0; i < NOI; i++)
 {
        name = getIntersectionName(i);
           
         
        {   intersection_name.push_back(name);
        intersection_id_from_names.emplace(name,i);
        }
    }

}

std::string Map::get_intersection_name(unsigned i){
    return intersection_name[i];
}

void Map::set_found_intersection(std::string x){
    found_intersection = x;
}
std::string Map::get_found_intersection(){
    return found_intersection;
}



void Map::make_street_length() {
    double length = 0;
    unsigned i;

    
    for (i = 0; i < getNumberOfStreets(); i++) {

        
        std::vector <unsigned> seg_temp = street_street_segment[i];
        for (unsigned j=0; j< seg_temp.size(); j++){
             length += street_seg_length[seg_temp[j]];
            
        }
        
        street_length.push_back(length);
    }
}

double Map::get_street_length(unsigned i){
    return street_length[i];
}

/*NOTE: to get feature type do FeatureType. to choose whatever. there are 11 of them. unknown is index 0,
  stream index 11, you can access directly using index e.g FeatureType(4) =River */
void Map::make_feature_name() {
    std::string name;

    for (unsigned i = 0; i < number_of_features; i++) {
        name = getFeatureName(i);
       
          
        feature_name.push_back(name);
        feature_id_from_name.emplace(name,i);
        
        
    }

}

std::string Map:: get_feature_name(unsigned i){
    return feature_name[i];
}

std::vector<std::string> Map::get_feature_name_vector(){
    return feature_name;
}

unsigned Map:: get_feature_id_from_name(std::string ss){
    return feature_id_from_name[ss];
}

void Map::make_street_ids_from_names() {
    unsigned i;

    for (i = 0; i < getNumberOfStreets(); i++) {
        auto search = street_ids_from_names.find(getStreetName(i));
        if (search != street_ids_from_names.end()) {
            std::vector<unsigned> temp;
            street_ids_from_names.emplace(getStreetName(i), temp);
            street_ids_from_names[getStreetName(i)].push_back(i);
        } else
            street_ids_from_names[getStreetName(i)].push_back(i);
    }
}

std::vector<unsigned> Map::get_street_ids_from_names(std::string name) {
    return street_ids_from_names[name];
}

void Map::make_street_inter_id() {
    unsigned i;
    unsigned x;
    StreetSegmentInfo Seg_Info;
    for (i = 0; i < getNumberOfStreetSegments(); i++) {
        Seg_Info = Seg_Info_v[i];
        x = Seg_Info.streetID;
        std::string name = getStreetName(x);
        std::unordered_map<unsigned, unsigned> check;

        auto search = street_inter_id.find(name);
        if (search == street_inter_id.end()) {
            std::vector <unsigned> temp;

            street_inter_id.emplace(name, temp);
            street_inter_id[name].push_back(Seg_Info.from);
            street_inter_id[name].push_back(Seg_Info.to);


        } else {

            auto search1 = check.find(Seg_Info.from);
            if (search1 == check.end()) {
                check.emplace(Seg_Info.from, 1);
                street_inter_id[name].push_back(Seg_Info.from);
            }

            auto search2 = check.find(Seg_Info.to);
            if (search2 == check.end()) {
                check.emplace(Seg_Info.to, 1);
                street_inter_id[name].push_back(Seg_Info.to);
            }

        }
    }
}

std::vector<unsigned> Map::get_street_inter_id(std::string name) {
    return street_inter_id[name];
}

void Map::make_feature_num_and_points() {
    //std::vector<unsigned> feature_pointnum;
    //std::vector<std::vector<LatLon>> feature_points;

    unsigned i;
    unsigned x;

    for (i = 0; i < getNumberOfFeatures(); ++i) {
        feature_points.push_back(std::vector<LatLon>());
        unsigned numofPoints = getFeaturePointCount(i);
        feature_pointnum.push_back(numofPoints);

        for (x = 0; x < numofPoints; ++x) {
            feature_points[i].push_back(getFeaturePoint(i, x));
        }
    }
}

LatLon Map::get_feature_points(unsigned i, unsigned j) {
    LatLon point = Map::feature_points[i][j];
    return point;
}

unsigned Map::get_feature_pointnum(unsigned j) {
    unsigned num = feature_pointnum[j];
    return num;
}

void Map::make_streetseg_position() {
    // std::cout<<"asdadfasd";
    unsigned totalSeg = getNumberOfStreetSegments();

    for (unsigned id = 0; id < totalSeg; ++id) {
        unsigned numCurve = getStreetSegmentInfo(id).curvePointCount;
        curve_seg_points.push_back(std::vector<t_point>());

        unsigned idfrom = getStreetSegmentInfo(id).from;
        float fromX = LonToCar(getIntersectionPosition(idfrom).lon());
        float fromY = LatToCar(getIntersectionPosition(idfrom).lat());
        t_point fromT = {fromX, fromY};
        seg_from.push_back(fromT);
        //std::cout<<fromX<<getIntersectionPosition(idfrom).lon()<<std::endl;

        unsigned idto = getStreetSegmentInfo(id).to;
        float toX = LonToCar(getIntersectionPosition(idto).lon());
        float toY = LatToCar(getIntersectionPosition(idto).lat());
        t_point toT = {toX, toY};
        seg_to.push_back(toT);

        seg_curve_num.push_back(numCurve);
        for (unsigned c = 0; c < numCurve;) {
            float ypos = LatToCar(getStreetSegmentCurvePoint(id, c).lat());
            float xpos = LonToCar(getStreetSegmentCurvePoint(id, c).lon());
            t_point pos = {xpos, ypos};
            curve_seg_points[id].push_back(pos);
            ++c;
        }
    }
}

t_point Map::get_seg_curve_points(StreetSegmentIndex ID, unsigned c) {
    return curve_seg_points[ID][c];
}

unsigned Map::get_seg_curve_num(unsigned c) {
    return seg_curve_num[c];
}

t_point Map::get_seg_from(unsigned id) {
    return seg_from[id];
}

t_point Map::get_seg_to(unsigned id) {
    return seg_to[id];
}

// m3


void Map::make_from_to(){
    
    unsigned total_segs = getNumberOfStreetSegments();
    for (unsigned i=0; i<total_segs; i++){
        StreetSegmentInfo x = Seg_Info_v[i];
        unsigned from = x.from;
        unsigned to = x.to;
        if (x.oneWay == true){
            
            
            from_to_int_travel_time.emplace (std::make_pair(from,to), travel[i]);
            from_to_int_street_id.emplace (std::make_pair(from,to), x.streetID);
            from_to_int_street_seg_id.emplace (std::make_pair(from,to), i);
            from_to_int_street_seg_id1.emplace (std::make_pair(from,to), i);        
            
        }
        
        else{
               
        
            from_to_int_travel_time.emplace (std::make_pair(from,to), travel[i]);
            from_to_int_travel_time.emplace (std::make_pair(to,from), travel[i]);
            from_to_int_street_id.emplace (std::make_pair(from,to), x.streetID);
            from_to_int_street_id.emplace (std::make_pair(to,from), x.streetID);
            from_to_int_street_seg_id.emplace (std::make_pair(from,to), i);
            from_to_int_street_seg_id.emplace (std::make_pair(to,from), i);
            from_to_int_street_seg_id1.emplace (std::make_pair(to,from), i);       
            }
         
       unsigned j=0;
    if (j>x.speedLimit){
        j= x.speedLimit;
        max_speed = j;
    } 
    }
    
   /*
    for (unsigned i=0; i<total_segs; i++){
        StreetSegmentInfo x = Seg_Info_v[i];
        unsigned from = x.from;
        unsigned to = x.to;
        if (x.oneWay == true){
            
            auto search = from_to_int_travel_time.find(std::make_pair(from,to));
            if (search!= from_to_int_travel_time.end()){
                
                if (travel[i] < (search-> second)){
                    from_to_int_travel_time[std::make_pair(from,to)]=travel[i];
                    from_to_int_street_id[std::make_pair(from,to)]=x.streetID;
                   from_to_int_street_seg_id[std::make_pair(from,to)]=i;
                }
            }
            
            
        }
        
        else{
               
        auto search = from_to_int_travel_time.find(std::make_pair(from,to));
            if (search!= from_to_int_travel_time.end()){
                 if (travel[i] < (search-> second)){
                    from_to_int_travel_time[std::make_pair(from,to)]=travel[i];
                    from_to_int_travel_time[std::make_pair(to,from)]=travel[i];
                    from_to_int_street_id[std::make_pair(from,to)]=x.streetID;
                    from_to_int_street_id[std::make_pair(to,from)]=x.streetID;
                    from_to_int_street_seg_id[std::make_pair(from,to)]=i;
                    from_to_int_street_seg_id[std::make_pair(to,from)]=i;
                }
            }
            
        }
        
    }*/
}


void Map:: make_adjacent_intersections() {
    
    
    for (unsigned x=0; x<number_of_intersections; x++){
         std::vector<unsigned> streets_connected_to_id;
         unsigned i;
         streets_connected_to_id = intersection_street_segment[x];
        StreetSegmentInfo Seg_Info;
        std::vector<unsigned> adj_int_id;
        std::vector<unsigned> adj_int_opp1;
    
        for (i = 0; i < streets_connected_to_id.size(); i++) {
             Seg_Info = Seg_Info_v[streets_connected_to_id[i]];
            if (Seg_Info.oneWay == false) { // if its not one way
                if (Seg_Info.from == x) {
                    adj_int_id.push_back(Seg_Info.to); 
                   adj_int_opp1.push_back(Seg_Info.from);
                }
                else {
                    adj_int_id.push_back(Seg_Info.from);
                    adj_int_opp1.push_back(Seg_Info.to);
                }
            } 
            else if (Seg_Info.oneWay == true) { // if it is one way

                if (Seg_Info.from == x) {
                    adj_int_opp1.push_back(Seg_Info.from);
                    adj_int_id. push_back(Seg_Info.to);
            }
        }
    }
        
        closed_list.push_back(0);
        open_list.push_back(0); 
        travel_time_from_start.push_back(INFINITY);
        parent_street_id .push_back(INFINITY);
        f_node .push_back(INFINITY);

    // get unique vector
    std::vector < unsigned> unique = unique_vector(adj_int_id);
    std::vector < unsigned> unique1 = unique_vector(adj_int_opp1);
    adj_int.push_back(unique);
    adj_int_opp.push_back(unique1);
    
    }
}

void Map::make_nodes_and_edges(){
    for (unsigned ii = 0; ii < number_of_intersections; ii++) {
        Node temp;
        temp.travel_time_from_start  = INFINITY;
        temp.parent_street_id  = INFINITY;
        temp.id = ii;
        temp.parent_node = NULL;
        temp.prev_edge = INFINITY;
        
        nodes.push_back(temp);

    }
    
    for (unsigned j=0; j< number_of_intersections; j++){
        std::vector <unsigned> streets_connected_to_id = intersection_street_segment[j];
        std::vector <Edge> temp_edges;
        
        for (unsigned k = 0; k< streets_connected_to_id.size(); k++){
            Edge seg;
            StreetSegmentInfo Seg_Info = Seg_Info_v[streets_connected_to_id[k]];
            
            if(Seg_Info.oneWay == false){
                seg.street_seg_id= streets_connected_to_id[k];
                seg.travel_time = travel[seg.street_seg_id];
                
                // checking both ends of segment
                if(Seg_Info.from== j){
                    seg.to_node = &nodes[Seg_Info.to];
                    seg.prev_node= &nodes[Seg_Info.from];
                    
                }
                
                else {
                    seg.to_node = &nodes[Seg_Info.from];
                    seg.prev_node= &nodes[Seg_Info.to];
                }
                
                temp_edges.push_back(seg);
            }
            
            else{
                seg.street_seg_id= streets_connected_to_id[k];
                seg.travel_time = travel[seg.street_seg_id];
                
                if(Seg_Info.from== j){
                   seg.to_node = &nodes[Seg_Info.to];
                   seg.prev_node= &nodes[Seg_Info.from]; 
                   temp_edges.push_back(seg);
                }
            }
        }
        
        edges.push_back(temp_edges);
    }
    
    
    
}