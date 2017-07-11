// DatabaseLibrary.h - Contains declaration of Function class  
#pragma once  

#ifdef DATABASELIBRARY_EXPORTS  
#define DATABASELIBRARY_API __declspec(dllexport)   
#else  
#define DATABASELIBRARY_API __declspec(dllimport)   
#endif  

#include <iostream>
#include <string>
#include <vector>

namespace DatabaseLibrary
{

	class Record {
    friend class Table;
	private:
		/* Contains the entires of the record. */
		std::vector<std::string> entries;

	public:
        /* Record Constructor. Initializes record to size number of blank string entires. */
		DATABASELIBRARY_API Record(size_t size) : entries(size, "") {};

		/*Return number of declared entries for the record. */
		DATABASELIBRARY_API size_t numEntries();

		/* Returns entry i for the record. */
		DATABASELIBRARY_API std::string& operator[](size_t i) { return entries.at(i); }
	};


	class Table {
    friend class Record;
	private:
		/* Contains the table's attributes. */
		std::vector<std::string> attributeColumns;

		/* Contains the table's records. */
		std::vector<Record> recordRows;

        /* Table key.*/
        std::string tableKey;
        
	public:
		/* Default Table Contructor. Initializes a blank table. */
		DATABASELIBRARY_API Table();

		/* Table Constructor with attributes. Initialize table with attributes listed in a comma separated string. Ex. Table("ID, First name, Last name") */
		DATABASELIBRARY_API Table(std::string attributes);

		/* Add attribute to the table. If the attribute already exists, it will not be added. */
		DATABASELIBRARY_API void addAttribute(std::string attribute);

		/* Delete attribute. If the attribute does not exist, an error message will be displayed. */
		DATABASELIBRARY_API void deleteAttribute(std::string attribute);

		/* Insert Record. If the record has less entires than the number of attributes in the table, blank entries will be added to the record. If the record has more entries than the table has attributes, a error message will be displayed. */
		DATABASELIBRARY_API void addRecord(Record tableRecord);

		/* Returns a vector containing the attribute names. */
		DATABASELIBRARY_API std::vector<std::string> getAttributes();

		/* Returns a size_t with the amount of records in the table. */
		DATABASELIBRARY_API size_t getNumRecords();

		/* Returns an iterator to the record at the appropriate index. If a record with that index does not exist, an error is thrown. */
		DATABASELIBRARY_API std::vector<Record>::iterator recordIterator(int index);

		/* Add or change the primary key of the table to the input key. If key has duplicate entries, an error message will inform the user of all duplicates that must be resolved before the key can be added. */
		DATABASELIBRARY_API void addKey(std::string primaryKey);

		/* Returns a table that is a cross join of this table and the passed in "otherTable". If otherTable contains an attribute that exists in this table, the attributes are combined into one single attribute of the same name. */
		DATABASELIBRARY_API Table crossJoinTables(Table otherTable);

		/* Returns a table that is a natural join of this table and the passed in "otherTable". The otherTable must have a key that matches an attribute in this table. If there is no matching attribute, an error message will be printed. The new table will combine the attributes from this table and 'otherTable'. Records from both tables will be added to the new table. Matching records will be assimalated. */
		DATABASELIBRARY_API Table naturalJoinTables(Table otherTable);

		/* Returns an size_t for the number of records that have a defined entry for the attribute. */
		DATABASELIBRARY_API size_t numRecordsWithAttributes(std::string attribute);

        /* Returns the minimum entry of an attribute. If none of the records have a defined value for the attribute or if the attribute does not exist, a blank string is returned. */
        DATABASELIBRARY_API std::string minEntryOfAttribute(std::string attribute);

        /* Returns the maximum entry of an attribute. If none of the records have a defined value for the attribute or if the attribute does not exist, a blank string is returned. */
        DATABASELIBRARY_API std::string maxEntryOfAttribute(std::string attribute);
	};

	class Database {
	private:
        /* Contains all of the Database's tables. */
        std::vector<Table> tables;
        
        /* Contains the names of all the Database's tables. */
        std::vector<std::string> tableNames;

	public:
		/* Default Database Constructor. Initialize a blank database. */
		DATABASELIBRARY_API Database();

		/* Add table to the database with name 'tableName'. If the table name already exists in the database, an error message will be dispalyed. */
		DATABASELIBRARY_API void addTable(Table table, std::string tableName);

		/* Delete table with name 'tableName' from the database. If a table with name 'tableName' does not exist, an error message will be displayed. */
		DATABASELIBRARY_API void deleteTable(std::string tableName);

		/* Returns a vector of strings containing the names of the tables in the database. */
		DATABASELIBRARY_API std::vector<std::string> getTableNames();

		/* Returns a vector of tables containing the tables in the database. */
		DATABASELIBRARY_API std::vector<Table> getTables();
        
        /* Run a query on the table. The SELECT parameter determines which attributes should be kept. Multiple attributes can be selected with a comma separated string or use a "*" to select all of the attributes. The FROM parameter is a single table name for which the query should be run. The WHERE parameter determines the action of the query. */
        DATABASELIBRARY_API Table query(std::string selectArg, std::string fromArg, std::string whereArg);
	};
}
