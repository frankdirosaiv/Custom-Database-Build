#include  "DatabaseLibrary.h"
#include <iostream>
#include <stdio.h>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace DatabaseLibrary;


TEST_CASE("Add table works", "[DatabaseLibrary]") {
	Database db;
	Table tbl;
	db.addTable(tbl, "Table");
	std::vector<std::string> names;
	names = db.getTableNames();
	REQUIRE(names[0] == "Table");
	Table tbl2;
	db.addTable(tbl2, "Table2");
	names = db.getTableNames();
	REQUIRE(names[0] != names[1]);
}

TEST_CASE("Delete table works", "[DatabaseLibrary]") {
	Database db;
	Table tbl1;
	Table tbl2;
	db.addTable(tbl1, "Table1");
	db.addTable(tbl2, "Table2");
	db.deleteTable("Table2");
	std::vector<std::string> names;
	names = db.getTableNames();
	REQUIRE(names[0] == "Table1"); //You can't do this or you will get out of range 
	REQUIRE(names.size() == 1);
}

TEST_CASE("Get table names works", "[DatabaseLibrary]") {
	Database db;
	Table tbl1;
	Table tbl2;
	db.addTable(tbl1, "Tbl1");
	db.addTable(tbl2, "Tbl2");
	std::vector<std::string> names;
	names = db.getTableNames();
	REQUIRE(names[0] == "Tbl1");
	REQUIRE(names[1] == "Tbl2");
}

TEST_CASE("Get tables works", "[DatabaseLibrary]") {
	Database db;
	Table tbl1;
	Table tbl2;
	tbl1.addAttribute("ID");
	tbl2.addAttribute("Name");
	db.addTable(tbl1, "table1"); //Tables must be added if you're going to try and get tables from the database
	db.addTable(tbl2, "table2"); //Tables must be added if you're going to try and get tables from the database
	std::vector<Table> tables = db.getTables();
	REQUIRE(tables[0].getAttributes()[0] == "ID");
	REQUIRE(tables[1].getAttributes()[0] == "Name");
}


TEST_CASE("Query works", "[DatabaseLibrary]") {
	Database db;
	Table tbl1;
	Table tbl2;

	tbl1.addAttribute("Name");
	tbl1.addAttribute("Age");
	tbl2.addAttribute("Random");
	Record rc(2);
	rc.setEntry(0, "Paul");
	rc.setEntry(1, "99");
	tbl1.addRecord(rc);
	Record rc1(2);
	rc1.setEntry(0, "Kyle");
	rc1.setEntry(1, "23");
	tbl1.addRecord(rc1);

	db.addTable(tbl1, "Table 1"); //Tables are pass by value, not reference, so you must add all attributes BEFORE you add them to the database
	db.addTable(tbl2, "Table 2");

	Table tbl3 = db.query("Name", "Table 1", "Age < 50");
	std::vector<std::string> atts = tbl3.getAttributes();
	REQUIRE(atts[0] == "Name");
	//REQUIRE(atts[1] != "Age"); //You cannot have this, you are accessing memory that is out of range
}


TEST_CASE("Add attribute works", "[DatabaseLibrary]") {
	Table tbl;
	tbl.addAttribute("Attribute");
	std::vector<std::string> atts;
	atts = tbl.getAttributes();
	REQUIRE(atts[0] == "Attribute");
}

TEST_CASE("Delete attribute works", "[DatabaseLibrary]") {
	Table tbl;
	tbl.addAttribute("att1");
	tbl.addAttribute("att2");
	tbl.deleteAttribute("att2");
	std::vector<std::string> atts;
	atts = tbl.getAttributes();
	REQUIRE(atts[0] != "att2"); //Table.getattributes will return a vector of size 1, you cannot access this memory [1] or you will go out of range
	REQUIRE(atts.size() == 1);
}

TEST_CASE("Add record works", "[DatabaseLibrary]") {
	Table tbl;
	Record rc1(2);
	tbl.addAttribute("Name");
	rc1.setEntry(0, "Bill");
	tbl.addRecord(rc1); //you cannot add a record with more entries than the table has attribtues
	REQUIRE(tbl.getNumRecords() == 0);
}

TEST_CASE("Get attributes works", "[DatabaseLibrary]") {
	Table tbl;
	tbl.addAttribute("att1");
	tbl.addAttribute("att2");
	std::vector<std::string> atts;
	atts = tbl.getAttributes();
	REQUIRE(atts[0] == "att1");
	REQUIRE(atts[1] == "att2");
}

TEST_CASE("Get number of records works", "[DatabaseLibrary]") {
	Table tbl;
	Record rc1(2);
	Record rc2(2);
	tbl.addRecord(rc1); //you cannot add a record with more entries than the table has attribtues
	tbl.addRecord(rc2); //you cannot add a record with more entries than the table has attribtues
	REQUIRE(tbl.getNumRecords() == 0);
}

TEST_CASE("Add key works", "[DatabaseLibrary]") {
	Table tbl;
	tbl.addAttribute("Name");
	tbl.addKey("Name");
	tbl.addAttribute("Date");
	Record rc(2);
	Record rc1(2);
	rc.setEntry(0, "Paul");
	rc1.setEntry(0, "Paul");
	rc.setEntry(1, "May");
	rc1.setEntry(1, "June");
	tbl.addRecord(rc);
	tbl.addRecord(rc1);
	REQUIRE(tbl.getNumRecords() == 1);
}

TEST_CASE("Set entry and get entry works", "[DatabaseLibrary]") {
	Record rc1(2);
	rc1.setEntry(0, "Chad");
	rc1.setEntry(1, "Carl");
	REQUIRE(rc1.getEntry(0) == "Chad");
	REQUIRE(rc1.getEntry(1) == "Carl");
}

TEST_CASE("Record iterator works", "[DatabaseLibrary]") {
	Table tbl;
	Record rc1(0);//you cannot add a record with more entries than the table has attribtues
	Record rc2(3);
	tbl.addRecord(rc1);
	tbl.addRecord(rc2);
	std::vector<Record>::iterator it;
	it = tbl.recordIterator(0);
	REQUIRE(it->numEntries() == rc1.numEntries());
}

TEST_CASE("Cross join tables works", "[DatabaseLibrary]") {
	Table tbl1;
	Table tbl2;
	tbl1.addAttribute("ID");
	tbl1.addAttribute("Name");
	tbl2.addAttribute("ID");
	tbl2.addAttribute("Course");
	Record rc(2);
	Record rc1(2);
	rc.setEntry(0, "23");
	rc.setEntry(1, "Jill");
	rc1.setEntry(0, "32");
	rc1.setEntry(1, "CSCE");
	tbl1.addRecord(rc);
	tbl2.addRecord(rc1);
	Table tbl3;
	tbl3 = tbl1.crossJoinTables(tbl2);
	std::vector<Record>::iterator it = tbl3.recordIterator(0);
	REQUIRE(it->getEntry(0) == "23");
	REQUIRE(it->getEntry(1) == "Jill");
	REQUIRE(it->getEntry(2) == "32");
	REQUIRE(it->getEntry(3) == "CSCE");
}

TEST_CASE("Natural join tables works", "[DatabaseLibrary]") {
	Table tbl1;
	Table tbl2;
	tbl1.addAttribute("ID");
	tbl1.addAttribute("Name");
	tbl2.addAttribute("ID");
	tbl2.addAttribute("Course");
	tbl2.addKey("ID");
	Record rc(2);
	Record rc1(2);
	rc.setEntry(0, "23");
	rc.setEntry(1, "Jill");
	rc1.setEntry(0, "32");
	rc1.setEntry(1, "CSCE");
	tbl1.addRecord(rc);
	tbl2.addRecord(rc1);
	Table tbl3;
	tbl3 = tbl1.naturalJoinTables(tbl2);
	std::vector<Record>::iterator it = tbl3.recordIterator(0);
	std::vector<Record>::iterator it2 = tbl3.recordIterator(1);
	REQUIRE(it->getEntry(0) == "23");
	REQUIRE(it->getEntry(1) == "Jill");
	REQUIRE(it->getEntry(2) == "");
	REQUIRE(it2->getEntry(0) == "32");
	REQUIRE(it2->getEntry(1) == "");
	REQUIRE(it2->getEntry(2) == "CSCE");
}

TEST_CASE("Number of Records with Attr Works", "[DatabaseLibrary]") {
	Table tbl;
	tbl.addAttribute("Name");
	Record rc1(1);//you cannot add a record with more entries than the table has attribtues
	Record rc2(1);//you cannot add a record with more entries than the table has attribtues
	Record rc3(1);//you cannot add a record with more entries than the table has attribtues
	rc1.setEntry(0, "entry0");
	rc3.setEntry(1, "entry1");
	tbl.addRecord(rc1);
	tbl.addRecord(rc2);
	tbl.addRecord(rc3);

	REQUIRE(tbl.numRecordsWithAttributes("Name") == 1);
}

TEST_CASE("Min Works", "[DatabaseLibrary]") {
	Table tbl;
	Record rc1(2);
	Record rc2(2);
	Record rc3(2);
	tbl.addAttribute("attr1");
	tbl.addAttribute("attr2");
	rc1.setEntry(0, "0");
	rc2.setEntry(0, "1");
	rc3.setEntry(0, "2");
	rc1.setEntry(1, "some");
	rc2.setEntry(1, "test");
	rc3.setEntry(1, "text");
	tbl.addRecord(rc1);
	tbl.addRecord(rc2);
	tbl.addRecord(rc3);

	REQUIRE(tbl.minEntryOfAttribute("attr1") == "0");
	REQUIRE(tbl.minEntryOfAttribute("attr2") == "some");
}

TEST_CASE("Max Works", "[DatabaseLibrary]") {
	Table tbl;
	Record rc1(2);
	Record rc2(2);
	Record rc3(2);
	tbl.addAttribute("attr1");
	tbl.addAttribute("attr2");
	rc1.setEntry(0, "0");
	rc2.setEntry(0, "1");
	rc3.setEntry(0, "2");
	rc1.setEntry(1, "some");
	rc2.setEntry(1, "test");
	rc3.setEntry(1, "text");
	tbl.addRecord(rc1);
	tbl.addRecord(rc2);
	tbl.addRecord(rc3);

	REQUIRE(tbl.maxEntryOfAttribute("attr1") == "2");
	REQUIRE(tbl.maxEntryOfAttribute("attr2") == "text");
}

TEST_CASE("Number of entries works", "[DatabaseLibrary]") {
	Record rc(2);
	REQUIRE(rc.numEntries() == 0); //Num Entries is supposed to retur the number of NON EMPTY entries, not the size
}
