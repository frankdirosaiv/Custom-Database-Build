#include "DatabaseLibrary.h"


namespace DatabaseLibrary {
    //constructor of empty table
    Table::Table() {}
    
    //Constructor that takes in attributes
    Table::Table(std::string attributes) {}
    
    //Add column to table titled attribute
    void Table::addAttribute(std::string attribute) {}
    
    //Delete attribute
    void Table::deleteAttribute(std::string attribute) {}
    
    //Insert Record
    void Table::addRecord(Record tableRecord) {}
    
    //Get attributes
    std::vector<std::string> Table::getAttributes() {
        std::vector<std::string> testAttributes;
        return testAttributes;
    }
    
    //Get num records
    size_t Table::getNumRecords() {
        return 0;
    }
    
    //return iterator
    std::vector<Record>::iterator Table::recordIterator(int index) {
        std::vector<Record> testRecords;
        return testRecords.begin();
    }
    
    //addkey
    void Table::addKey(std::string primaryKey) {}
    
    //join table
    Table Table::crossJoinTables(Table otherTable) {
        Table newTable;
        return newTable;
    }
    
    //natural join
    Table Table::naturalJoinTables(Table otherTable) {
        Table newTable;
        return newTable;
    }
    
    //count
    size_t Table::numRecordsWithAttributes(std::string attribute) {
        return 0;
    }
    
    //Minimum entry for a specific attribute
    std::string Table::minEntryOfAttribute(std::string attribute){
        std::string testString;
        return testString;
    }
    
    //Maximum entry for a specific attribute
    std::string Table::maxEntryOfAttribute(std::string attribute){
        std::string testString;
        return testString;
    }

}

