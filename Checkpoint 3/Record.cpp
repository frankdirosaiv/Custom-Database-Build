#include "DatabaseLibrary.h"


namespace DatabaseLibrary {

    void Record::setEntry(size_t index, std::string entry){
        if(index < entries.size()){
            if(table != NULL && table->tableKey != "" && table->keyIndex == index){
                std::pair<std::unordered_set<std::string>::iterator, bool> insertCheck = table->recordKeys.insert(entry);
                if(insertCheck.second == false){
                    std::cout << "Entry could not be changed because it has a duplicated key of " << *(insertCheck.first) << "." << std::endl;
                    return;
                } else if(entries[index] != ""){
                    table->recordKeys.erase(entries[index]);
                }
            }
            entries[index] = entry;
        } else {
            std::cout << "Enter a valid index for the record. " << std::endl;
        }
    }
    
    std::string Record::getEntry(size_t index) const {
        if(index < entries.size()){
            return entries[index];
        } else {
            std::cout << "Enter a valid index for the record. " << std::endl;
            return "";
        }
    }
    
    size_t Record::numEntries() const {
        size_t numEntries = 0;
        for(size_t i = 0; i < entries.size(); ++i){
            if(entries[i] != ""){
                ++numEntries;
            }
        }
        return numEntries;
    }
    
    void Record::printRecord() const {
        for(size_t i = 0 ; i < entries.size(); ++i){
            if(entries[i] != ""){
                std::cout << std::setw(10) << entries[i];
                std::cout << " ";
            } else{
                std::cout << std::setw(10) << "*";
                std::cout << " ";
            }
            
        }
        std::cout << std::endl;
    }


}

