#include "json.hpp"
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <stack>
// testing
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_set>

using namespace std;
using json = nlohmann::json;


int main() {
	vector<string> attributes {"business_id","review_id","user_id"};

	unordered_set<string> busSet;
	unordered_set<string> userSet;
    stringstream buffer;
    string line;
    string busline;
    ifstream inFS("user.json");
    ifstream busFS("business1.json");
    ifstream tipFS("yelp_academic_dataset_tip.json");
    ofstream tipOFS("tip.json");

    if(!inFS.is_open()) {
        throw runtime_error("file not found");
    }

    try{
    	while(getline(inFS, line)) {
    		buffer.clear();
    		buffer.str("");
    		auto j = json::parse(line);
    		string str = j["user_id"];
    		userSet.insert(str);
    	}
    } catch(...){}
    
    try{
    	while(getline(busFS, line)) {
    		buffer.clear();
    		buffer.str("");
    		auto j = json::parse(line);
    		string str = j["business_id"];
    		busSet.insert(str);
    	}
    } catch(...){}
    cout << "Finished parsing reviews" << endl;

    int count = 0;
    while(getline(tipFS, busline)) {
    	buffer.clear();
    	buffer.str("");
    	try{
    		auto j = json::parse(busline);
    		//j["friends"] = " ";
    		if (userSet.find(j["user_id"]) != userSet.end() && busSet.find(j["business_id"]) != busSet.end()) {
    			cout << busline << endl;
    			tipOFS << busline << "\n";
    			count++;
    		}
    	} catch(...){}
    	if (count > 10000) {
    		break;
    	}
    }

    inFS.close();
    return 0;
}