#include "DatabaseLibrary.h"


namespace DatabaseLibrary {
    
    Table::Table() {
        tableKey = "";
    }
    
    /* Constructor that takes in attributes in a comma separated string. */
    Table::Table(std::string attributes) { //Need to make sure we are checking for duplicates
        tableKey = "";
        std::istringstream iss(attributes);
        std::string attribute;
        //Parse the different attributes from the input string
        while(std::getline(iss, attribute, ',') || std::getline(iss, attribute)){
            //Check to see if there are white spaces before the next attribute and delete them
            while(iss.peek() == ' '){
                iss.ignore();
            }
            // Check to make sure there are no extra white spaces at the end of the attribute
            std::string::iterator it = attribute.end() - 1;
            while(*it == ' '){
                attribute.erase(it);
                --it;
            }
            //Make sure a duplicated value is not being inserted. If it is, append a number to change the name.
            std::string originalAttribute = attribute;
            int count = 1;
            while(std::find(attributeColumns.begin(), attributeColumns.end(), attribute) != attributeColumns.end()){
                attribute = originalAttribute + "(" + std::to_string(count) + ")";
                ++count;
            }
            
            //Add the attribute to the table
            attributeColumns.push_back(attribute);
        }
    }
    
    /* Add Attribute to the table. Make sure that the attribute has not already been defined. */
    void Table::addAttribute(std::string attribute) {
        std::vector<std::string>::iterator it = std::find(attributeColumns.begin(), attributeColumns.end(), attribute);
        //Check if the attribute was found
        if(it == attributeColumns.end()){
            attributeColumns.push_back(attribute);
            //For each record, fill the new attribute with an empty string.
            for (size_t i = 0; i < recordRows.size(); ++i) {
                recordRows.at(i).entries.push_back("");
            }
        } else{
            //Append a number to the attribtue since it is a duplicated entry
            std::string originalAttribute = attribute;
            int count = 1;
            while(std::find(attributeColumns.begin(), attributeColumns.end(), attribute) != attributeColumns.end()){
                attribute = originalAttribute + "(" + std::to_string(count) + ")";
                ++count;
            }
            attributeColumns.push_back(attribute);
            for (size_t i = 0; i < recordRows.size(); ++i) {
                recordRows.at(i).entries.push_back("");
            }
        }
    }
    
    /* Delete attribute from the table. Make sure that the attribute exists before deleting and if the attribute was the key, make sure the key is reset. */
    void Table::deleteAttribute(std::string attribute) {
        std::vector<std::string>::iterator iter = std::find(attributeColumns.begin(), attributeColumns.end(), attribute);
        size_t distance = std::distance(attributeColumns.begin(), iter);
        //Check if the attribute was found
        if(distance != attributeColumns.size()){
            if(distance < keyIndex){
                --keyIndex;
            }
            //Reset the key if needed
            if(tableKey == attribute){
                tableKey = "";
                recordKeys.clear();
            }
            //Erase the attribute and the column for the attribute in the records
            attributeColumns.erase(attributeColumns.begin() + distance);
            for(size_t i = 0; i < recordRows.size(); ++i){
                recordRows.at(i).entries.erase(recordRows.at(i).entries.begin() + distance);
            }
        } else{
            std::cout << "Attribute was not found" << std::endl;
        }
    }
    
    /* Add Record. If a key is defined, make sure that there are no duplicate values. */
    void Table::addRecord(Record tableRecord) {
        //Make sure the record is the appropriate size of the table.
        if(attributeColumns.size() > tableRecord.entries.size()){
            //Sometimes has to be plus 1. Figure out why.
            for(size_t i = 0; i < (attributeColumns.size() - tableRecord.entries.size() + 1); ++i){
                tableRecord.entries.push_back("");
            }
        } else if(attributeColumns.size() < tableRecord.entries.size()){
            std::cout << "Record has too many attributes. Additional attributes must be first added to the table. " << std::endl;
            return;
        }
        
        //If table has a key make sure that it does not contain a duplicate value
        if(tableKey != ""){
            std::pair<std::unordered_set<std::string>::iterator, bool> insertCheck = recordKeys.insert(tableRecord.entries[keyIndex]);
            if(insertCheck.second == false){
                std::cout << "Record could not be inserted because it has a duplicated key of " << *(insertCheck.first) << "." << std::endl;
                return;
            }
        }
        tableRecord.table = this;
        recordRows.push_back(tableRecord);
    }
    
    /* Get Table Attribtues. */
    std::vector<std::string> Table::getAttributes() const {
        return attributeColumns;
    }
    
    /* Get Number of Table Records. */
    size_t Table::getNumRecords() const {
        return recordRows.size();
    }
    
    /* Return record iterator. */
    std::vector<Record>::iterator Table::recordIterator(int index) {
        if(index < recordRows.size()){
            return recordRows.begin() + index;
        } else{
            return recordRows.end();
        }
    }
    
    /* Add or Change Table Key. */
    void Table::addKey(std::string attribute) {
        std::unordered_set<std::string> set;
        std::vector<std::string>::iterator iter = std::find(attributeColumns.begin(), attributeColumns.end(), attribute);
        size_t distance = std::distance(attributeColumns.begin(), iter);
        //Check if the attribute exits
        if(distance != attributeColumns.size()){
            for(size_t i = 0; i < recordRows.size(); ++i){
                std::pair<std::unordered_set<std::string>::iterator, bool> insertCheck = set.insert(recordRows[i].entries[distance]);
                if(insertCheck.second == false && *(insertCheck.first) != ""){
                    std::cout << "Record at index " << i << " could not be inserted because it has a duplicated key of " << *(insertCheck.first) << "." << std::endl;
                    return;
                }
            }
            recordKeys = set;
            tableKey = attribute;
            keyIndex = distance;
        } else{
            std::cout << "Attribute needs to be added before it can declared the key." << std::endl;
        }
        
    }
    
    /* Cross Join Table. */
    Table Table::crossJoinTables(Table otherTable) {
        Table resultTable;
        resultTable.attributeColumns = this->attributeColumns;
        for(size_t i = 0; i < otherTable.attributeColumns.size(); ++i){
            resultTable.addAttribute(otherTable.attributeColumns[i]);
        }
        for(size_t i = 0; i < recordRows.size(); ++i){
            for(size_t j = 0; j < otherTable.recordRows.size(); ++j){
                Record newRecord = recordRows[i];
                for(size_t k = 0; k < otherTable.recordRows[j].entries.size(); ++k){
                    newRecord.entries.push_back(otherTable.recordRows[j].entries[k]);
                }
                resultTable.addRecord(newRecord);
            }
        }
        
        return resultTable;
    }
    
    /* Natural Join Table. */
    Table Table::naturalJoinTables(Table otherTable) {
        Table resultTable;
        std::vector<std::string>::iterator iter;
        size_t indexOfMatchAttribute = 0;
        bool matchingEntries = false;
        std::vector<size_t> uniqueOtherTableAttr;
        
        //Make sure that othertable has a key
        if (otherTable.tableKey == "") {
            std::cout << "Argument table must have a valid primary key, otherwise blank table will be returned" << std::endl;
            return resultTable;
        }
        else {
            //If the otherTable's key attribute is not in current table, then send an empty table back
            iter = std::find(this->attributeColumns.begin(), this->attributeColumns.end(), otherTable.tableKey);
            if (iter == this->attributeColumns.end()) {
                std::cout << "Current table must have an attribute that is the same as argument table's key, otherwise a blank table will be returned" << std::endl;
                return resultTable;
            }
            else {
                indexOfMatchAttribute = std::distance(this->attributeColumns.begin(), iter);
            }
        }
        
        //Add all non key attributes from otherTable to result table
        resultTable = *this;
        resultTable.tableKey = "";
        for (size_t i = 0; i < otherTable.attributeColumns.size(); ++i) {
            if (otherTable.attributeColumns.at(i) != otherTable.tableKey) {
                resultTable.addAttribute(otherTable.attributeColumns.at(i));
                uniqueOtherTableAttr.push_back(i);
            }
        }
        
        //Update data in result Table
        for (size_t i = 0; i < otherTable.recordRows.size(); ++i) {
            for (size_t j = 0; j < resultTable.recordRows.size(); j++) {
                if (resultTable.recordRows.at(j).entries.at(indexOfMatchAttribute) == otherTable.recordRows.at(i).entries.at(otherTable.keyIndex)) {
                    //add info to result Table at matching entries
                    for (size_t k = 0; k < uniqueOtherTableAttr.size(); ++k) {
                        resultTable.recordRows.at(j).entries.at(this->attributeColumns.size() + k) = otherTable.recordRows.at(i).entries.at(uniqueOtherTableAttr.at(k));
                    }
                    matchingEntries = true;
                }
            }
            if (!matchingEntries) {
                //add record from otherTable to result table
                Record record(resultTable.attributeColumns.size());
                record.entries.at(indexOfMatchAttribute) = otherTable.recordRows.at(i).entries.at(otherTable.keyIndex);
                for (size_t k = 0; k < uniqueOtherTableAttr.size(); ++k) {
                    record.entries.at(this->attributeColumns.size() + k) = otherTable.recordRows.at(i).entries.at(uniqueOtherTableAttr.at(k));
                }
                resultTable.addRecord(record);
            }
            matchingEntries = false;
        }
        
        return resultTable;
    }
    
    /* Number of Records with a Specific Attribute */
    size_t Table::numRecordsWithAttributes(std::string attribute) {
        std::vector<std::string>::iterator iter = std::find(attributeColumns.begin(), attributeColumns.end(), attribute);
        size_t distance = std::distance(attributeColumns.begin(), iter);
        size_t count = 0;
        if(distance != attributeColumns.size()){
            for(size_t i = 0; i < recordRows.size(); ++i){
                if(recordRows[i].entries[distance] != ""){
                    ++count;
                }
            }
        } else{
            return 0;
        }
        
        return count;
    }
    
    /* Min Entry of an Attribute*/
    std::string Table::minEntryOfAttribute(std::string attribute) {
        std::vector<std::string>::iterator iter = std::find(attributeColumns.begin(), attributeColumns.end(), attribute);
        size_t distance = std::distance(attributeColumns.begin(), iter);
        std::string minEntry = "";
        
        if(distance != attributeColumns.size()){
            minEntry = recordRows[0].entries[distance];
            for(size_t i = 1; i < recordRows.size(); ++i){
                if((minEntry == "" && recordRows[i].entries[distance] != "") || (recordRows[i].entries[distance] != "" && recordRows[distance].entries[distance] < minEntry)){
                    minEntry = recordRows[i].entries[distance];
                }
            }
        }
        return minEntry;
    }
    
    /* Max Entry of an Attribute*/
    std::string Table::maxEntryOfAttribute(std::string attribute) {
        std::vector<std::string>::iterator iter = std::find(attributeColumns.begin(), attributeColumns.end(), attribute);
        size_t distance = std::distance(attributeColumns.begin(), iter);
        std::string maxEntry = "";
        
        if(distance != attributeColumns.size()){
            maxEntry = recordRows[0].entries[distance];
            for(size_t i = 1; i < recordRows.size(); ++i){
                if(recordRows[i].entries[distance] > maxEntry){
                    maxEntry = recordRows[i].entries[distance];
                }
            }
        }
        return maxEntry;
    }
    
	/*Print all records and attributes contained in table*/
    void Table::printTable(){
        std::cout << "    ";
        for(size_t i = 0; i < attributeColumns.size(); ++i){
            if(attributeColumns[i] == tableKey){
                std::cout << "->" << std::setw(8) << attributeColumns[i] << " ";
            } else{
                std::cout << std::setw(10) << attributeColumns[i] << " ";
            }
        }
        std::cout << std::endl;
        for(size_t i = 0; i < recordRows.size(); ++i){
            std::cout << std::setw(3) << i << ": ";
            recordRows[i].printRecord();
        }
    }

}
