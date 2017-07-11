#include "DatabaseLibrary.h"

namespace DatabaseLibrary {
	//Constructor for an empty database
	Database::Database(){}

	// Create an table in Database with argument name
	void Database::addTable(Table table, std::string tableName){}

	// Delete a table in Database with argument name
	void Database::deleteTable(std::string tableName){}

	//List all tables in Database
	std::vector<std::string> Database::getTableNames(){
        std::vector<std::string> testStrings;
    	return testStrings;
	}

	//Return all tables in Database
	std::vector<Table> Database::getTables(){
        std::vector<Table> testTables;
        return testTables;
	}

    //Run query
    Table Database::query(std::string selectArg, std::string fromArg, std::string whereArg){
        Table newTable;
        return newTable;
    }
}

