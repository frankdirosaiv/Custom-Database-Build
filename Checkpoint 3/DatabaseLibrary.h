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
#include <sstream>
#include <unordered_set>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <stack>

namespace DatabaseLibrary
{
	class Table;

	class Record {
		friend class Table;
	private:
		/* Contains the entries of the record. */
		std::vector<std::string> entries;

		/*If a record is stored in a table, the pointer to that table is stored here. */
		Table* table;

	public:
		/* Record Constructor. Initializes record to size number of blank string entires. */
		DATABASELIBRARY_API Record(size_t size) : entries(size, "") {};

		/*Return number of declared entries for the record. */
		DATABASELIBRARY_API size_t numEntries() const;

		/* Sets entry i for the record. */
		DATABASELIBRARY_API void setEntry(size_t index, std::string entry);
        
		/* Returns entry i for the record. */
        DATABASELIBRARY_API std::string getEntry(size_t index) const;
        
		/* Print all entries in a record. */
        DATABASELIBRARY_API void printRecord() const;

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
        
        /* Record key entires. */
        std::unordered_set<std::string> recordKeys;
        
        /* Key Index */
        size_t keyIndex;

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
		DATABASELIBRARY_API std::vector<std::string> getAttributes() const;

		/* Returns a size_t with the amount of records in the table. */
		DATABASELIBRARY_API size_t getNumRecords() const;

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

		/* Print all attributes and records in a table. */
        DATABASELIBRARY_API void printTable();
	};

	class Database {
	private:
		/* Contains all of the Database's tables. */
		std::vector<Table> tables;

		/* Contains the names of all the Database's tables. */
		std::vector<std::string> tableNames;

		/* Returns the index of the records in accordance with the basic query operation (attribute) (opeartor) (data), For example LHS = RHS.*/
		std::vector<int> evaluateOperator(Table fromTable, std::string attribute, std::string data, std::string operators);

		/* Return the index of the records in accordance with logical query of two tables. */
		std::vector<int> evaluateAndOr(Table fromTable, std::vector<int> table1, std::vector<int> table2, std::string operators);

		/* Return the of the records in fromTable that are not in table1. */
		std::vector<int> evaluateNot(Table fromTable, std::vector<int> table1);

		/* Takes in a query string and parses it into an easy to manipulate vector of strings. */
		std::vector<std::string> ParseForSyntax(std::string str);

		/* Returns an all upper case version of the input string. */
		std::string stringToUpper(std::string inputString);

		/* Returns true if str is float. */
		bool isFloat(const std::string &str);

		/* Returns true if str is int. */
		bool isDigits(const std::string &str);

		/* Parse the filtered query for operators to evaluate. */
		std::stack<std::string> ParseForOperators(Table fromTable, std::vector<std::string> parsedString, std::vector<std::vector<int>> &lookupExpression);

		/* Parse the filtered query for logical operators to evaluate. */
		std::stack<std::string> ParseForLogic(Table fromTable, std::stack<std::string> &resultStack, std::vector<std::vector<int>> &lookupExpression);
		

	public:
		/* Default Database Constructor. Initialize a blank database. */
		DATABASELIBRARY_API Database();

		/* Add table to the database with name 'tableName'. If the table name already exists in the database, an error message will be dispalyed. */
		DATABASELIBRARY_API void addTable(Table table, std::string tableName);

		/* Delete table with name 'tableName' from the database. If a table with name 'tableName' does not exist, an error message will be displayed. */
		DATABASELIBRARY_API void deleteTable(std::string tableName);

		/* Returns a vector of strings containing the names of the tables in the database. */
		DATABASELIBRARY_API std::vector<std::string> getTableNames() const;

		/* Returns a vector of tables containing the tables in the database. */
		DATABASELIBRARY_API std::vector<Table> getTables() const;

		/* Run a query on the table. The SELECT parameter determines which attributes should be kept. Multiple attributes can be selected with a comma separated string or use a "*" to select all of the attributes. The FROM parameter is a single table name for which the query should be run. The WHERE parameter determines the action of the query. */
		DATABASELIBRARY_API Table query(std::string selectArg, std::string fromArg, std::string whereArg);
        
		/* Print all tables in a database. */
        DATABASELIBRARY_API void printDatabase();
	};
}
