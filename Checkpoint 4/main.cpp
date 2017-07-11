
#include "record.h"
#include "table.h"
#include "database.h"
#include "json.hpp"
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <stack>
#include <iomanip>
#include <algorithm>
// testing
#include <iostream>
#include <sstream>
#include <fstream>
// end testers
using namespace std;
using json = nlohmann::json;

//Helper function returns lowercase version of input string
string makeLower(string &str) {
    locale loc;

    //loop through every char to make it lowercase
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i], loc);
    }
    return str;
}

//formats user input when user input requries an operator
string addComparisonSpacing(string str) {
	int operatorBegin = -1;
	int i = 0;

	for (i; i < str.length(); i++) {
		if (!isspace(str[i]) && operatorBegin == -1) {
			operatorBegin = i;
			break;
		}
	}

    // no space between operator and number, operator is one character
	if (isdigit(str[i+1])) { 
		string newStr = str.substr(0, i+1);
		newStr += " ";
		newStr += str.substr(i+1, str.length()-1);
		return newStr;
	}

    // no space between operator and number, operator is two characters
	else if (!isdigit(str[i+1]) && !isspace(str[i+1]) && isdigit(str[i+2])) { 
		string newStr = str.substr(0, i+2);
		newStr += " ";
		newStr += str.substr(i+2, str.length()-1);
		return newStr;
	}

    // space between operator and number
	else { 
		return str;
	}
}

//Create local tables for yelp data
table createJsonTable(vector<string> attributes, string path) {
    table userTable(attributes);

    //open Filestreams
    stringstream buffer;
    string line;
    ifstream inFS(path);
    if(!inFS.is_open()) {
        throw runtime_error("file " + path + " not found");
    }
    
    //Get each line containing a json object
    while(getline(inFS, line)) {
        vector<string> data;
        buffer.clear();
        buffer.str("");
        auto j = json::parse(line);

        //Look through all attributes contained in json object
        for (string attribute : attributes) {
            attribute = makeLower(attribute);

            //If attribute is float, convert to string then push into data
            if (j[attribute].is_number_float()) {
                float number = j[attribute];
                data.push_back(to_string(number));
            }

            //If attribute is integer, convert to string then push into data
            else if (j[attribute].is_number_integer()) {
                int number = j[attribute];
                string numToString = to_string(number);
                if(attribute == "review_count" || attribute == "useful"|| attribute == "funny" || attribute == "cool" || attribute == "fans"){
                    while(numToString.length() < 6){
                        numToString = "0" + numToString;
                    }
                }
                data.push_back(numToString);
            }

            //If attribute is bool, convert to string then push into data
            else if (j[attribute].is_boolean()) {
                bool boolean = j[attribute];
                if (boolean) {
                    data.push_back("true");
                }
                else {
                    data.push_back("false");
                }
            }

            //If attribute is strin, fill whitespace with underscore user's written reviews
            else if (j[attribute].is_string()) {
                string name = j[(attribute)];
                if (attribute != "text" ){
                    replace(name.begin(), name.end(), ' ', '_');
                }
                if (attribute == "business_id" || attribute == "user_id" || attribute == "review_id") {
                    name = "_" + name;
                }
                data.push_back(name);
            }

            //find shopping in json categories array
            else if(attribute == "shopping"){
                bool found = false;
                if(j["categories"].is_array()){
                    vector<string> vect = j["categories"];
                    for(int i = 0; i < vect.size(); ++i){
                        if(vect[i] == "Shopping"){
                            data.push_back("true");
                            found = true;
                            break;
                        }
                    }
                }
                
                if(!found){
                    data.push_back("false");
                }
            }

            //find food in json categories array
            else if(attribute == "food"){
                bool found = false;
                if(j["categories"].is_array()){
                    vector<string> vect = j["categories"];
                    for(int i = 0; i < vect.size(); ++i){
                        if(vect[i] == "Food" || vect[i] == "Restraunts"){
                            data.push_back("true");
                            found = true;
                            break;
                        }
                    }
                }
                
                if(!found){
                    data.push_back("false");
                }
            }

            //find hotels and travel in json categories array
            else if(attribute == "hotels_&_travel"){
                bool found = false;
                if(j["categories"].is_array()){
                    vector<string> vect = j["categories"];
                    for(int i = 0; i < vect.size(); ++i){
                        if(vect[i] == "Hotels & Travel"){
                            data.push_back("true");
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //find homes services in json categories
            else if(attribute == "home_services"){
                bool found = false;
                if(j["categories"].is_array()){
                    vector<string> vect = j["categories"];
                    for(int i = 0; i < vect.size(); ++i){
                        if(vect[i] == "Home Services"){
                            data.push_back("true");
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //find automotive in json categories
            else if(attribute == "automotive"){
                bool found = false;
                if(j["categories"].is_array()){
                    vector<string> vect = j["categories"];
                    for(int i = 0; i < vect.size(); ++i){
                        if(vect[i] == "Automotive"){
                            data.push_back("true");
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //find monday hours in json hours array
            else if(attribute == "monday_hours"){
                bool found = false;
                if(j["hours"].is_array()){
                    vector<string> vect = j["hours"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t dayFind = vect[i].find("Monday");
                        if(dayFind != std::string::npos){
                            data.push_back(vect[i].substr(dayFind+7));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find tuesday hours in json hours array
            else if(attribute == "tuesday_hours"){
                bool found = false;
                if(j["hours"].is_array()){
                    vector<string> vect = j["hours"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t dayFind = vect[i].find("Tuesday");
                        if(dayFind != std::string::npos){
                            data.push_back(vect[i].substr(dayFind+8));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find wednsday hours in json hours array
            else if(attribute == "wednesday_hours"){
                bool found = false;
                if(j["hours"].is_array()){
                    vector<string> vect = j["hours"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t dayFind = vect[i].find("Wednesday");
                        if(dayFind != std::string::npos){
                            data.push_back(vect[i].substr(dayFind+10));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find thursday hours in json hours array
            else if(attribute == "thursday_hours"){
                bool found = false;
                if(j["hours"].is_array()){
                    vector<string> vect = j["hours"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t dayFind = vect[i].find("Thursday");
                        if(dayFind != std::string::npos){
                            data.push_back(vect[i].substr(dayFind+9));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find friday hours in json hours array
            else if(attribute == "friday_hours"){
                bool found = false;
                if(j["hours"].is_array()){
                    vector<string> vect = j["hours"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t dayFind = vect[i].find("Friday");
                        if(dayFind != std::string::npos){
                            data.push_back(vect[i].substr(dayFind+7));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find saturday hours in json hours array
            else if(attribute == "saturday_hours"){
                bool found = false;
                if(j["hours"].is_array()){
                    vector<string> vect = j["hours"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t dayFind = vect[i].find("Saturday");
                        if(dayFind != std::string::npos){
                            data.push_back(vect[i].substr(dayFind+9));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find sunday hours in json hours array
            else if(attribute == "sunday_hours"){
                bool found = false;
                if(j["hours"].is_array()){
                    vector<string> vect = j["hours"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t dayFind = vect[i].find("Sunday");
                        if(dayFind != std::string::npos){
                            data.push_back(vect[i].substr(dayFind+7));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find year in json 'yelping since' date
            else if (attribute == "year") {
                bool found = false;
                if (j["yelping_since"].is_string()) {
                    string start_date = j["yelping_since"];
                    data.push_back(start_date.substr(0, 4));
                    found = true;
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find month in json 'yelping since' date
            else if (attribute == "month") {
                bool found = false;
                if (j["yelping_since"].is_string()) {
                    string start_date = j["yelping_since"];
                    data.push_back(start_date.substr(5, 2));
                    found = true;
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find day in json 'yelping since' date
            else if (attribute == "day") {
                bool found = false;
                if (j["yelping_since"].is_string()) {
                    string start_date = j["yelping_since"];
                    data.push_back(start_date.substr(8, 2));
                    found = true;
                }
                if(!found){
                    data.push_back("N/A");
                }
            }

            //find if business accepts credit cards
            else if(attribute == "credit_cards"){
                bool found = false;
                if(j["attributes"].is_array()){
                    vector<string> vect = j["attributes"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t attributeFind = vect[i].find("BusinessAcceptsCreditCards");
                        if(attributeFind != std::string::npos){
                            data.push_back(vect[i].substr(attributeFind+28));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //find if business is 'good for kids'
            else if(attribute == "good_for_kids"){
                bool found = false;
                if(j["attributes"].is_array()){
                    vector<string> vect = j["attributes"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t attributeFind = vect[i].find("GoodForKids");
                        if(attributeFind != std::string::npos){
                            data.push_back(vect[i].substr(attributeFind+13));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //find if business has bike parking
            else if(attribute == "bike_parking"){
                bool found = false;
                if(j["attributes"].is_array()){
                    vector<string> vect = j["attributes"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t attributeFind = vect[i].find("BikeParking");
                        if(attributeFind != std::string::npos){
                            data.push_back(vect[i].substr(attributeFind+13));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //find if business is by appointment only
            else if(attribute == "appointment_only"){
                bool found = false;
                if(j["attributes"].is_array()){
                    vector<string> vect = j["attributes"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t attributeFind = vect[i].find("ByAppointmentOnly");
                        if(attributeFind != std::string::npos){
                            data.push_back(vect[i].substr(attributeFind+19));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //find if business is wheelchair accesible 
            else if(attribute == "wheelchair_access"){
                bool found = false;
                if(j["attributes"].is_array()){
                    vector<string> vect = j["attributes"];
                    for(int i = 0; i < vect.size(); ++i){
                        size_t attributeFind = vect[i].find("WheelchairAccessible");
                        if(attributeFind != std::string::npos){
                            data.push_back(vect[i].substr(attributeFind+22));
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    data.push_back("false");
                }
            }

            //If attribute is unheard of, just push back a null star
            else{
                data.push_back("*");
                
            }
        }

        //add record with data into table
        userTable.insert(record(userTable.getAttributes(), data));
    }

    //close file stream and return table
    inFS.close();
    return userTable;
}


/////////////////// SUPPPORT FUNCTIONS /////////////////

//Print full table with all data
void print(const table& t) {
    for (auto it = t.begin(); it != t.end(); ++it) {
        for (auto i = 0; i < t.getAttributes().size(); ++i) {
            cout << left << setw(15) << t.getAttributes().at(i);
            string output = it->getData(t.getAttributes().at(i));
            output.erase(0, min(output.find_first_not_of('0'), output.size()-1));
            cout << ": " << output << endl;
        }
        cout << endl;
    }
    cout << "\n\n";
}

//Print functino only used for printing during queryReviews
void printReviews(const table& t) {
    for (auto it = t.begin(); it != t.end(); ++it) {
    //Print Review ID
        cout << left << setw(15) << t.getAttributes().at(0);
        cout << ": " << it->getData(t.getAttributes().at(0)) << endl;

    //Print Reviewer Name
        cout << left << setw(15) << t.getAttributes().at(6);
        cout << ": " << it->getData(t.getAttributes().at(6)) << endl;

    //Print Stars given
        cout << left << setw(15) << t.getAttributes().at(3);
        cout << ": " << it->getData(t.getAttributes().at(3)) << endl;

    //Print Usefulness
        cout << left << setw(15) << t.getAttributes().at(5);
        cout << ": " << it->getData(t.getAttributes().at(5)) << endl;

    //Print Review
        cout << left << setw(15) << t.getAttributes().at(4);
        cout << ": \n" << it->getData(t.getAttributes().at(4)) << endl;
        cout << "\n\n";
    }
}


//Print function only used for printing during queryUserReviews
void printUserReviews(const table& t) {
    cout << "\n\n";
    cout << "REVIEWS BY: " << t.begin()->getData(t.getAttributes().at(6)) << endl;
    cout << "USER AVERAGE STARS: " << t.begin()->getData(t.getAttributes().at(7)) << endl;
    cout << "\n\n";
    
    for (auto it = t.begin(); it != t.end(); ++it) {
    //Print Review ID
        cout << left << setw(15) << t.getAttributes().at(0);
        cout << ": " << it->getData(t.getAttributes().at(0)) << endl;

    //Print Reviewer Name
        cout << left << setw(15) << "Business Name";
        cout << ": " << it->getData(t.getAttributes().at(8)) << endl;

    //Print Stars given
        cout << left << setw(15) << t.getAttributes().at(3);
        cout << ": " << it->getData(t.getAttributes().at(3)) << endl;

    //Print Usefulness
        cout << left << setw(15) << t.getAttributes().at(5);
        cout << ": " << it->getData(t.getAttributes().at(5)) << endl;

    //Print Review
        cout << left << setw(15) << t.getAttributes().at(4);
        cout << ": \n" << it->getData(t.getAttributes().at(4)) << endl;
        cout << "\n\n";
    }
}

//Print function only used for printing during queryTips
void printTips(const table& t) {
    cout << "\n\n";
    cout << "TIP FOR: " << t.begin()->getData(t.getAttributes().at(14)) << endl;
    cout << "         " << t.begin()->getData(t.getAttributes().at(15)) << endl;
    cout << "         " << t.begin()->getData(t.getAttributes().at(16)) << ",  ";
    cout << t.begin()->getData(t.getAttributes().at(17)) << endl;
    cout << "Stars:   " << t.begin()->getData(t.getAttributes().at(19)) << endl;
    cout << "\n";

    for (auto it = t.begin(); it != t.end(); ++it) {
        //Print user name
        cout << left << setw(15) << t.getAttributes().at(4);
        cout << ": " << it->getData(t.getAttributes().at(4)) << endl;

        //print tip
        cout << left << setw(15) << "TIP for others";
        cout << ": " << it->getData(t.getAttributes().at(2)) << endl;
    }
    cout << "\n\n";
}

//Compute statistics during user or business search
void computeStatistics(table filteredTable, table originalTable) {
    cout << "---------------------------------------------------------------------\n";
    cout << "STATISTICS OF FILTERED TABLE:\n";
    cout << "---------------------------------------------------------------------\n\n";

    //Do not execute statistics if there are none to be found
    if (filteredTable.getSize() == 0) {
        cout << "There are no statistics to display.\n\n\n";
        return;
    }
    
    //All possible categories to gain stats
    vector<string> numericStatisticAttributes = {"AVERAGE_STARS", "REVIEW_COUNT", "USEFUL", "FUNNY", "COOL", "FANS"};
    vector<vector<double>> stats;
    
    //Declare stat vectors and variables
    vector<double> meanValues;
    vector<double> minimumValues;
    vector<double> maximumValues;
    
    double currSum; double meanVal;
    double currMin;
    double currMax;
    
    double numOfFilteredRecords;
    double numOfOriginalRecords;
    
    string currAttribute;
    double currData;
    
    bool numericAttribute = false;
    bool anyNumericAttribute = false;
	
	//Iterate through each attribute in the filtered table and check to see if it's an attribute containing numeric values
    for (int i = 0; i != filteredTable.getAttributes().size(); ++i) {
        for (int j = 0; j < numericStatisticAttributes.size(); ++j) {
            if (numericStatisticAttributes[j] == filteredTable.getAttributes().at(i)) {
                numericAttribute = true;
                anyNumericAttribute = true;
            }
        }
        
        currAttribute = filteredTable.getAttributes().at(i);
        numOfFilteredRecords = (double) (filteredTable.count(currAttribute));
        
		//In the case that the attribute contains numeric values, stats will be displayed about the mean, maximum, and minimum values of
		//this attribute in the filtered table
        if (numericAttribute) {
            cout << "Attribute: " << currAttribute << "\n";
			
			//Use the built-in max and min functions provided by the "table" class to find the min and max values
            currMin = stod(filteredTable.min(currAttribute).getData(currAttribute));
            currMax = stod(filteredTable.max(currAttribute).getData(currAttribute));
            currSum = 0;
            
			//The column will have to be iterated through to find the mean value
            for (auto it = filteredTable.begin(); it != filteredTable.end(); ++it) {
                currData = stod(it->getData(currAttribute));
                currSum += currData; //Accumulating a sum which will be used to calculate the mean
            }
            meanVal = currSum/numOfFilteredRecords;
			
			//Print results to the user
            cout << " Minimum value: " << currMin << "\n";
            cout << " Maximum value: " << currMax << "\n";
            cout << " Mean value: " << meanVal << "\n\n";
        }
        
        numericAttribute = false; //Reset for next loop
    }
    
	//Determining the percentage of records which passed all conditions set by the filters and print this value
    numOfFilteredRecords = (double) filteredTable.getSize();
    numOfOriginalRecords = (double) originalTable.getSize();
    double percentagePassed = 100*(numOfFilteredRecords/numOfOriginalRecords);
    
    cout << "Number of records satisfying the filters provided:\n";
    cout << numOfFilteredRecords << "\n\n";
    cout << "Percentage of records satisfying the filters provided:\n";
    cout << percentagePassed << "%\n\n";
    
    cout << "---------------------------------------------------------------------\n";
    cout << "\n";
}

/* Natural Join Table. */
table naturalJoinTables(table table1, table table2) {
    table resultTable;
    std::vector<std::string>::iterator iter;
    std::vector<std::pair<std::string, std::string>> uniqueTable2Attr;
    std::vector<std::string> table1Attributes = table1.getAttributes();
    std::vector<std::string> table2Attributes = table2.getAttributes();
    
    //Make sure that othertable has a key
    if (!table2.keyIsSet()) {
        std::cout << "Table 2 must have a key." << std::endl;
        return resultTable;
    }
    else {
        //If the otherTable's key attribute is not in current table, then send an empty table back
        iter = std::find(table1Attributes.begin(), table1Attributes.end(), table2.getKey());
        if (iter == table1Attributes.end()) {
            std::cout << "Table 1 must have a matching attibute to the key of Table 2." << std::endl;
            return resultTable;
        }
    }
    
    //Add all non key attributes from otherTable to result table
    resultTable = table1;
    for (size_t i = 0; i < table2Attributes.size(); ++i) {
        if (table2Attributes.at(i) != table2.getKey()) {
            std::string originalAttribute = table2Attributes.at(i);
            int count = 1;
            while(std::find(table1Attributes.begin(), table1Attributes.end(), table2Attributes.at(i)) != table1Attributes.end()){
                table2Attributes.at(i) = originalAttribute + "(" + std::to_string(count) + ")";
                ++count;
            }
            resultTable.addAttribute(table2Attributes.at(i));
            uniqueTable2Attr.push_back(make_pair(originalAttribute, table2Attributes.at(i)));
        }
    }
    
    //Update data in result Table
    for (auto it = table2.begin(); it != table2.end(); ++it) {
        for (auto it2 = resultTable.begin(); it2 != resultTable.end(); ++it2) {
            if (it2->getData(table2.getKey()) == it->getData(table2.getKey())) {
                //add info to result Table at matching entries
                for (size_t k = 0; k < uniqueTable2Attr.size(); ++k) {
                    it2->addData(uniqueTable2Attr.at(k).second, it->getData(uniqueTable2Attr.at(k).first));
                }
            }
        }
    }
    
    return resultTable;
}


string numberComparison(string input){
    istringstream iss(input);
    string compOperator;
    string number;
    iss >> compOperator;
    iss >> number;
    while(number.length() < 6){
        number = "0" + number;
    }
    return compOperator + " " + number;
}

/////////////////////////////////////////////////////////////////////


/////////////////// SKELETON FUNCTIONS //////////////////////////////

// how to query the business table
void queryBusinesses(string inputString, database &db) {
    bool showStatistics = false;
    istringstream iss(inputString);
    string input;
    string filter;
    vector<string> whereVector (6, "");

    //parse user input
    while(iss >> input){
        //have user enter options they want to filter by
        if(input == "1"){
            cout << "Enter Name of Business" << endl;
            getline(cin, filter);
            replace(filter.begin(), filter.end(), ' ', '_');
            whereVector[0] = filter;
        }
        else if(input == "2"){
            string state;
            cout << "Enter City" << endl;
            getline(cin, filter);
            replace(filter.begin(), filter.end(), ' ', '_');
            cout << "Enter State" << endl;
            getline(cin, state);
            replace(state.begin(), state.end(), ' ', '_');
            whereVector[1] = filter + " and state = " + state;
        }
        else if(input == "3"){
            cout << "Enter Postal Code" << endl;
            getline(cin, filter);
            replace(filter.begin(), filter.end(), ' ', '_');
            whereVector[2] = filter;
        }
        else if(input == "4"){
            cout << "Enter Number of Rating Stars With Operator. Ex: > 3 , = 5, <= 4" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[3] = filter;
        }
        else if(input == "5"){
            cout << "Choose from the list of categories. Only one can be selected." << endl;
            cout << "1. Shopping" << endl;
            cout << "2. Food" << endl;
            cout << "3. Hotels & Travel" << endl;
            cout << "4. Home Services" << endl;
            cout << "5. Automotive" << endl;
            cout << "\n\n";
            getline(cin, filter);
            if(filter == "1"){whereVector[4] = "Shopping";}
            else if(filter == "2"){whereVector[4] = "Food";}
            else if(filter == "3"){whereVector[4] = "Hotels_&_Travel";}
            else if(filter == "4"){whereVector[4] = "Home_Services";}
            else if(filter == "5"){whereVector[4] = "Automotive";}
            else{cout << "Invalid Input" << endl;}
        }
        else if(input == "6"){
            cout << "Choose from the list of attributes. Only one can be selected." << endl;
            cout << "1. Business Accepts Credit Cards" << endl;
            cout << "2. Good For Kids" << endl;
            cout << "3. Bike Parking Available" << endl;
            cout << "4. By Appointment Only" << endl;
            cout << "5. Wheelchair Accessible" << endl;
            cout << "\n\n";
            getline(cin, filter);
            if(filter == "1"){whereVector[5] = "Credit_Cards";}
            else if(filter == "2"){whereVector[5] = "Good_For_Kids";}
            else if(filter == "3"){whereVector[5] = "Bike_Parking";}
            else if(filter == "4"){whereVector[5] = "Appointment_Only";}
            else if(filter == "5"){whereVector[5] = "Wheelchair_Access";}
            else{cout << "Invalid Input" << endl;}
        }
        else{
            cout << "Invalid Input" << endl;
            return;
        }
    }

    cout << endl << endl;
    cout << "REVIEW FILTERS: " << endl;
    cout << "Name: " << whereVector[0] << endl;
    cout << "City, State: ";
    if(whereVector[1].size() > 2){
        cout << whereVector[1].substr(0, whereVector[1].find(' ')) << ", " << whereVector[1].substr(whereVector[1].find('=')+2) << endl;
    }
    cout << "Zip Code: " << whereVector[2] << endl;
    cout << "Rating Stars: " << whereVector[3] << endl;
    cout << "Categories: " << whereVector[4] << endl;
    cout << "Attributes: " << whereVector[5] << endl;
    cout << "Are the filters correct?(Y/N)" << endl;
    cout << "\n\n";

    cin >> input;
    if(input == "Y"){
        cout << "\nDo you want to display statistics of the filtered data?(Y/N)\n\n" << endl;
        cin >> input;
        if (input == "Y") {
            showStatistics = true;
        }
        

        // Build query strings before sent into query database
        bool firstEntry = true;
        string whereInput = "";
        if(whereVector[0] != ""){
            if(firstEntry){whereInput = whereInput + " name = " + whereVector[0];}
            else{whereInput = whereInput + "and name = " + whereVector[0];}
            firstEntry = false;
        }
        if (whereVector[1] != ""){
            if(firstEntry){whereInput = " city = " + whereVector[1]; }
            else {whereInput = whereInput + " and city = " + whereVector[1]; }
            firstEntry = false;
        }
        if (whereVector[2] != ""){
            if(firstEntry){whereInput = " postal_code = " + whereVector[2]; }
            else {whereInput = whereInput + " and postal_code = " + whereVector[2]; }
            firstEntry = false;
        }
        if (whereVector[3] != ""){
            if(firstEntry){whereInput = " stars " + whereVector[3]; }
            else {whereInput = whereInput + " and stars " + whereVector[3]; }
            firstEntry = false;
        }
        if (whereVector[4] != ""){
            if(firstEntry){whereInput = whereVector[4] + " = " + "true"; }
            else {whereInput = whereInput + " and " + whereVector[4] + " = " + "true"; }
            firstEntry = false;
        }
        if (whereVector[5] != ""){
            if(firstEntry){whereInput = whereVector[5] + " = " + "true"; }
            else {whereInput = whereInput + " and " + whereVector[5] + " = " + "true"; }
            firstEntry = false;
        }

        //Print out final query created and execute query
        cout << whereInput << endl;
        table out = db.query("*", "BUSINESS", whereInput);
        print(out);

        //Show statistics after output has been printed
        if (showStatistics) {
            computeStatistics(out, db.getTables()[2]);
        }
    }
    else{
        cin.ignore();
        queryBusinesses(inputString, db);
        return;
    }

}


// how to query the user table
void queryUsers(string inputString, database &db) {
    bool showStatistics = false;
    istringstream iss(inputString);
    string input;
    string filter;
    vector<string> whereVector (8, "");

    //parse user input
    while(iss >> input){
        //have user enter options they want to filter by
        if(input == "1"){
            cout << "Enter First Name of User" << endl;
            getline(cin, filter);
            replace(filter.begin(), filter.end(), ' ', '_');
            whereVector[0] = filter;
        }
        else if(input == "2"){
            cout << "Enter Total Number of Reviews With Operator. Ex: > 300 , = 500, <= 400" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[1] = filter;
        }
        else if(input == "3"){
            cout << "Enter Account Creation Date With Operator. Ex: > 2010-08-17 , = 2008-04-01, <= 2017-02-21" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[2] = filter;
        }
        else if(input == "4"){
            cout << "Enter Number of Useful Votes With Operator. Ex: > 30 , = 50, <= 40" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[3] = filter;
        }
        else if(input == "5"){
            cout << "Enter Number of Funny Votes With Operator. Ex: > 30 , = 50, <= 40" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[4] = filter;
        }
        else if (input == "6") {
            cout << "Enter Number of Cool Votes With Operator. Ex: > 30 , = 50, <= 40" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[5] = filter;
        }
        else if (input == "7") {
            cout << "Enter Number of Fans With Operator. Ex: > 30 , = 50, <= 40" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[6] = filter;
        }
        else if (input == "8") {
            cout << "Enter Average Star Rating With Operator. Ex: > 3 , = 5, <= 4" << endl;
            getline(cin, filter);
			filter = addComparisonSpacing(filter);
            whereVector[7] = filter;
        }
        else{
            cout << "Invalid Input" << endl;
            return;
        }
    }

    cout << endl << endl;
    cout << "REVIEW FILTERS: " << endl;
    cout << "Name: " << whereVector[0] << endl;
    cout << "Total Reviews: " << whereVector[1] << endl;
    cout << "Account Creation Date: " << whereVector[2] << endl;
    cout << "Useful Votes: " << whereVector[3] << endl;
    cout << "Funny Votes: " << whereVector[4] << endl;
    cout << "Cool Votes: " << whereVector[5] << endl;
    cout << "Fans: " << whereVector[6] << endl;
    cout << "Average Star Rating: " << whereVector[7] << endl;
    cout << "Are the filters correct?(Y/N)" << endl;
    cout << "\n\n";

    cin >> input;
    if(input == "Y"){
        cout << "\nDo you want to display statistics of the filtered data?(Y/N)\n\n" << endl;
        cin >> input;
        if (input == "Y") {
            showStatistics = true;
        }

        bool firstEntry = true;
        string whereInput = "";
        if(whereVector[0] != ""){
            if(firstEntry){whereInput = " name = " + whereVector[0];}
            else {whereInput = whereInput + "and name = " + whereVector[0];}
            firstEntry = false;
        }
        if (whereVector[1] != ""){
            if(firstEntry){whereInput = " review_count " + numberComparison(whereVector[1]); }
            else {whereInput = whereInput + " and review_count " + numberComparison(whereVector[1]); }
            firstEntry = false;
        }
        if (whereVector[2] != ""){
            string compOperator;
            string year;
            string month;
            string day;
            
            bool finished = false;
			
			//Splitting date input provided by the user in format YYYY-MM-DD into 3 separate values
            for (int i = 0; i < whereVector[2].size(); i++) {
                if (isspace(whereVector[2][i]) == false) {
                    compOperator = whereVector[2][i];
                    if (whereVector[2][i+1] == '>') {
                        compOperator = "<>";
                    }
                    else if (whereVector[2][i+1] == '=') {
                        compOperator += "=";
                    }
                    for (int j = i; i + j < whereVector[2].size(); j++) {
                        if (isdigit(whereVector[2][i+j]) == true) {
                            year = whereVector[2].substr(i+j, 4);
                            month = whereVector[2].substr(i+j+5, 2);
                            day = whereVector[2].substr(i+j+8, 2);
                            finished = true;
                            break;
                        }
                    }
                }
                if (finished == true) {
                    break;
                }
            }
            
            if (compOperator != "<>" && compOperator != "=") {
                if(firstEntry){whereInput = " year" + compOperator + year + " or (year = " + year + " and month " + compOperator + month + ") or (year = " + year + " and month = " + month + " and day " + compOperator + day + ")";}
                else {whereInput = whereInput + " and (year" + compOperator + year + " or (year = " + year + " and month " + compOperator + month + ") or (year = " + year + "and month = " + month + " and day " + compOperator + day + "))"; }
            }
            else if (compOperator == "=") {
                if(firstEntry){whereInput = "year = " + year + " and month = " + month + " and day = " + day;}
                else {whereInput = "and (year = " + year + " and month = " + month + " and day = " + day + ")";}
            }
            else if (compOperator == "<>") {
                if(firstEntry){whereInput = "year <> " + year + " or month <> " + month + " and day <> " + day;}
                else {whereInput = "and (year <> " + year + " or month <> " + month + " or day <> " + day + ")";}
            }
            
            firstEntry = false;
        }
        if (whereVector[3] != ""){
            if(firstEntry){whereInput = " useful " + numberComparison(whereVector[3]); }
            else {whereInput = whereInput + " and useful " + numberComparison(whereVector[3]); }
            firstEntry = false;
        }
        if (whereVector[4] != ""){
            if(firstEntry){whereInput = " funny " + numberComparison(whereVector[4]); }
            else {whereInput = whereInput + " and funny " + numberComparison(whereVector[4]); }
            firstEntry = false;
        }
        if (whereVector[5] != ""){
            if(firstEntry){whereInput = " cool " + numberComparison(whereVector[5]); }
            else {whereInput = whereInput + " and cool " + numberComparison(whereVector[5]); }
            firstEntry = false;
        }
        if (whereVector[6] != ""){
            if(firstEntry){whereInput = " fans " + numberComparison(whereVector[6]); }
            else {whereInput = whereInput + " and fans " + numberComparison(whereVector[6]); }
            firstEntry = false;
        }
        if (whereVector[7] != ""){
            if(firstEntry){whereInput = " average_stars " + whereVector[7]; }
            else {whereInput = whereInput + " and average_stars " + whereVector[7]; }
            firstEntry = false;
        }
        
        cout << whereInput << endl;
        table out = db.query("*", "USER", whereInput);
        print(out);

        if (showStatistics) {
            computeStatistics(out, db.getTables()[3]);
        }
    }
    else{
        cin.ignore();
        queryUsers(inputString, db);
        return;
    }
}


// how to query review table
void queryReviews(table &business, table &reviews, table &user, database &db) {
    cout << "Enter a Business ID. Ex: _3wJ4kJsJhayyfJ30HsS04g" << endl;
    string business_id = "";
    string where = "";
    getline(cin, business_id);

    //create query string
    where = where + "business_id = " + business_id;
    table exactReviews = db.query("*", "REVIEW", where);
    table exactBusiness = db.query("name stars", "BUSINESS", where);

    //make sure that querys return something so that natural join will not create an error
    if (exactReviews.getSize() == 0 || exactBusiness.getSize() == 0){
        cout << "\nI'm sorry but the query " << where << " did not have any results for us to show you :(" << endl;
        cout << "Perhaps the business_id entered has a typo? \n" <<  endl;
        return;
    }

    //natural join
    table userReviews = naturalJoinTables(exactReviews, user);

    //output results
    cout << "\n\n";
    cout << "REVIEWS FOR: " << exactBusiness.begin()->getData(exactBusiness.getAttributes().at(0)) << endl;
    cout << "AVERAGE RATING: " << exactBusiness.begin()->getData(exactBusiness.getAttributes().at(1)) << endl;
    cout << "\n\n";

    printReviews(userReviews);

}

// how to query the user table
void queryUserReviews(table &business, table &reviews, table &user, database &db) {
    cout << "Enter a User ID. Ex: _q6eks8g-8ve1AXMh1MTKJA" << endl;
    string user_id = "";
    string where = "";
    getline(cin, user_id);

    //develop query string 
    where = where + "user_id = " + user_id;
    table exactReviews = db.query("*", "REVIEW", where);

    //Make sure that something was returned in the query
    if (exactReviews.getSize() == 0){
        cout << "\nI'm sorry but the query " << where << " did not have any results for us to show you :(" << endl;
        cout << "Perhaps the business_id entered has a typo? \n" <<  endl;
        return;
    }

    //execute joins
    table userReviews = naturalJoinTables(exactReviews, user);
    table userBusinessReviews = naturalJoinTables(userReviews, business);

    //print output of the join junction
    printUserReviews(userBusinessReviews);

}



void queryTips(table &business, table &tip, table &user, database &db) {
    cout << "Enter a Business ID. Ex: _gRTEQPvX3s1WzFhOPNbVeg" << endl;
    string business_id = "";
    string where = "";
    getline(cin, business_id);

    //create query string
    where = where + "business_id = " + business_id;
    table exactTips = db.query("*", "TIP", where);
    table exactBusiness = db.query("*", "BUSINESS", where);
    exactBusiness.makeKey("BUSINESS_ID");

    //Make sure query produces a non empty table
    if (exactTips.getSize() == 0 || exactBusiness.getSize() == 0){
        cout << "\nI'm sorry but the query " << where << " did not have any results for us to show you" << endl;
        cout << "Perhaps the business_id entered has a typo? \n" <<  endl;
        return;
    }

    //Join the non-empty tables
    table userTips = naturalJoinTables(exactTips, user);
    table businessTips = naturalJoinTables(userTips, exactBusiness);

    //print the output of the natural join
    printTips(businessTips);
}


void queryUserAccuracy(table &business, table &reviews, table &user, database &db) {
    cout << "Some people are just grumpy, with this tool, you can search to see if a user is an accurate yelp reviewer or a grump" << endl;
    cout << "Enter a User ID. Ex: _blnXCVRWipBKbpz3G9fqUQ" << endl;
    string user_id = "";
    string where = "";
    getline(cin, user_id);

    //create query string
    where = where + "user_id = " + user_id;
    table exactReviews = db.query("review_id user_id business_id stars", "REVIEW", where);
    table exactUser = db.query("name useful fans", "USER", where);

    //make sure the query returns a table that is not empty
    if (exactReviews.getSize() == 0 || exactUser.getSize() == 0){
        cout << "\nI'm sorry but the query " << where << " did not have any results for us to show you :(" << endl;
        cout << "Perhaps the business_id entered has a typo? \n" <<  endl;
        return;
    }

    //join appropriate tables
    table userBusinessReviews = naturalJoinTables(exactReviews, business);
    size_t numReviews = 0;
    float diff = 0;
    float sum = 0;

    //perform analytics on data returned
    for (auto it = userBusinessReviews.begin(); it != userBusinessReviews.end(); ++it) {
        diff = stof(it->getData(userBusinessReviews.getAttributes().at(3))) - stof(it->getData(userBusinessReviews.getAttributes().at(9)));
        sum += abs(diff);
        ++numReviews;
    }

    //Print output of analytics
    cout << "ACCURACY OF " << exactUser.begin()->getData(exactUser.getAttributes().at(0)) << "'S REVIEWS:" << endl;
    cout << "This user is on average " << (sum/numReviews/5)*100 << "% off of the average star rating for a business.\n\n" << endl;

}

/////////////////////////////////////////////////////////////////////



int main() {

    //create all tables the application will work with
    database db;
    table user = createJsonTable({"user_id", "name", "average_stars", "year", "month", "day", "review_count", "useful", "funny", "cool", "fans"}, "user.json");
    table reviews = createJsonTable({"REVIEW_ID", "USER_ID", "BUSINESS_ID", "STARS", "TEXT", "USEFUL"}, "review1.json");
    table business = createJsonTable({"BUSINESS_ID", "NAME", "ADDRESS", "CITY", "STATE", "POSTAL_CODE", "STARS", "REVIEW_COUNT", "SHOPPING", "FOOD", "HOTELS_&_TRAVEL", 
        "HOME_SERVICES", "AUTOMOTIVE", "CREDIT_CARDS", "GOOD_FOR_KIDS", "BIKE_PARKING", "APPOINTMENT_ONLY", "WHEELCHAIR_ACCESS", "MONDAY_HOURS", "TUESDAY_HOURS", 
        "WEDNESDAY_HOURS", "THURSDAY_HOURS", "FRIDAY_HOURS", "SATURDAY_HOURS", "SUNDAY_HOURS"}, "business1.json");
    table tip = createJsonTable({"business_id", "user_id", "text", "likes"}, "tip.json");

    //add keys to all tables that need keys
    business.makeKey("BUSINESS_ID");
    user.makeKey("USER_ID");

    //add all tables created to database
    db.addTable(user, "USER");
    db.addTable(reviews, "REVIEW");
    db.addTable(business, "BUSINESS");
    db.addTable(tip, "TIP");

    //get user input to determine what kind of program they want to run
    string userInput = "N";
    string inputString = "";
    bool loop = true;
    while (loop) {
        cout << "Welcome to Yelp Analytics!" << endl;
        cout << "What can I help you find today?" << endl;
        cout << "Enter '1': Search for Users" << endl;
        cout << "Enter '2': Search for Businesses" << endl;
        cout << "Enter '3': Get Reviews for Business" << endl;
        cout << "Enter '4': Get Reviews By User" << endl;
        cout << "Enter '5': Get Tips for a Business" << endl;
        cout << "Enter '6': Accuracy of Users Reviews" << endl;
        cout << "Enter 'Q' to Quit" << endl;
        cout << "\n\n";
        
        //Q exits program
        cin >> userInput;
        if(userInput == "Q") {
            loop = false;
            break;
        }

        //Determine filters for user search
        if (userInput == "1") {
            cout << "Searching User. Enter all of the filters you want to search by separated by spaces. Ex. 1 2 3" << endl;
            cout << "1. Filter by First Name" << endl;
            cout << "2. Filter by Total Reviews" << endl;
            cout << "3. Filter by Account Creation Date" << endl;
            cout << "4. Filter by Useful Votes" << endl;
            cout << "5. Filter by Funny Votes" << endl;
            cout << "6. Filter by Cool Votes" << endl;
            cout << "7. Filter by Number of Fans" << endl;
            cout << "8. Filter by Average Star Rating" << endl;
            cout << "\n\n";

            cin.ignore();
            getline(cin, inputString);
            queryUsers(inputString, db);       
        }

        //Determine filters for business search
        else if (userInput == "2") {
            cout << "Searching Business. Enter all of the filters you want to search by separated by spaces. Ex. 1 2 3" << endl;
            cout << "1. Filter by Name" << endl;
            cout << "2. Filter by City and State" << endl;
            cout << "3. Filter by Postal Code" << endl;
            cout << "4. Filter by Rating Stars" << endl;
            cout << "5. Filter by Categories" << endl;
            cout << "6. Filter by Attributes" << endl;
            cout << "\n\n";

            cin.ignore();
            getline(cin, inputString);
            queryBusinesses(inputString, db);
        }

        //Send user to query Reviews function
        else if (userInput == "3") {
            cout << "You need a business id to search for reviews on a particular business" << endl;
            cout << "Enter '1': I am aware, I am ready to type in my business_id" << endl;
            cout << "Enter '2': I need to go back to the main menu to search for business to find the desired business id" << endl;
            cout << "\n\n";

            cin.ignore();
            getline(cin, inputString);
            if (inputString == "1") {
                queryReviews(business, reviews, user, db);
            }
        }

        //Send user to query user reviews function
        else if (userInput == "4") {
            cout << "You need a user id to search for reviews by a particular user" << endl;
            cout << "Enter '1': I am aware, I am ready to type in my user_id" << endl;
            cout << "Enter '2': I need to go back to the main menu to search for user to find the desired user id" << endl;
            cout << "\n\n";

            cin.ignore();
            getline(cin, inputString);
            if (inputString == "1") {
                queryUserReviews(business, reviews, user, db);
            }
        }

        //Send user to query Tips function
        else if (userInput == "5") {
            cout << "You need a business id to search for reviews by a particular business" << endl;
            cout << "Enter '1': I am aware, I am ready to type in my business_id" << endl;
            cout << "Enter '2': I need to go back to the main menu to search for business to find the desired business id" << endl;
            cout << "\n\n";

            cin.ignore();
            getline(cin, inputString);
            if (inputString == "1") {
                queryTips(business, tip, user, db);
            }
        }

        //Send user to query accuracy of user function
        else if (userInput == "6") {
            cout << "You need a user id to search for reviews by a particular user" << endl;
            cout << "Enter '1': I am aware, I am ready to type in my user_id" << endl;
            cout << "Enter '2': I need to go back to the main menu to search for user to find the desired user id" << endl;
            cout << "\n\n";

            cin.ignore();
            getline(cin, inputString);
            if (inputString == "1") {
                queryUserAccuracy(business, reviews, user, db);
            }
        }
        else {
            cout << "Invalid Input" << endl;
        }
    }   
}