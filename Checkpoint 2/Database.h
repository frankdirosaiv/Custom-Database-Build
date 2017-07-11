#pragma once

//Include Guard
#ifdef DATABASE_EXPORTS
#define DATABASE_API __declspec(dllexport)
#else
#define DATABASE_API __declspec(dllimport)
#endif

//Includes
#include <string>
#include <vector>

namespace Database {
	//Class Functions
	class Record {
		//Private Variables
	private:
		std::string vect[10];
		//Public Functions
	public:
		/*Constructor*/
		DATABASE_API Record(int size);

		/*Size of Record Function*/
		DATABASE_API int getSize();

		/*Accesor Function*/
		DATABASE_API std::string& operator [] (int i);
		DATABASE_API const std::string& operator [] (int i) const;
	};

	class Table {
		//Private Variables
	private:
		std::vector<Record> vect;
		//Public Functions
	public:
		/*Constructors*/
		DATABASE_API Table();

		DATABASE_API Table(std::vector<std::string> att_names);

		/*Add New Attribute Function*/
		DATABASE_API void addNewAtt(std::string att_name);

		/*Delete Attribute Function*/
		DATABASE_API void deleteAtt(std::string att_name);

		/*Insert Record Function*/
		DATABASE_API void insertRecord(Record new_record);

		/*Get Attributes Function*/
		DATABASE_API std::vector<std::string> getAtts();

		/*Get Size Function*/
		DATABASE_API int getSize();

		/*Get iterator*/
		DATABASE_API std::vector<Record>::iterator recordIterator(int index);

		/*Set Key Function*/
		DATABASE_API void setKey(std::string att_name);

		/*Cross Table Function*/
		DATABASE_API Table crossTables(Table table1, Table table2);

		/*Natural Join Function*/
		DATABASE_API Table natJoinTables(Table table1, Table table2);

		/*Attribute Count Function*/
		DATABASE_API int count(std::string att_name);

		/*Attribute Min Function*/
		DATABASE_API int minimum(std::string att_name);

		/*Attribute Max Function*/
		DATABASE_API int maximum(std::string att_name);
	};

	class Database {
		//Private Variables
	private:
		std::vector<Table> tables;
		//Public Functions
	public:
		/*Constructor*/
		DATABASE_API Database();

		/*Add Table Function*/
		DATABASE_API void addTable(Table new_table, std::string name);

		/*Drop Table Function*/
		DATABASE_API void dropTable(std::string table_name);

		/*List Tables Function*/
		DATABASE_API std::vector<std::string> listTables();

		/*Get Tables Function*/
		DATABASE_API std::vector<Table> getTables();

		/*Query Function*/
		DATABASE_API Table query(std::string select, std::string from, std::string wherel);
	};

}