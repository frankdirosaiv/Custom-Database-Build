#include <iostream>
#include <stdio.h>
#include "Database.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


	//-----------------------------------------------
	//Record
	//-----------------------------------------------
	//Declaration of Record variables
	
	Database::Record r0(4);
	Database::Record r1(4);
	Database::Record r2(4);
	Database::Record r3(4);
	Database::Record r4(4);
	
	Database::Record r5(3);
	Database::Record r6(3);
	Database::Record r7(3);
	Database::Record r8(3);
	Database::Record r9(3);
	
	
	TEST_CASE("TESTING DATABASE LIBRARY", "[Databaselib]") {

		//Test constructor
		SECTION("Record: constructor") {
			REQUIRE(r0.getSize() == 0);
			REQUIRE(r5.getSize() == 0);
			std::cout << "Passed - Record: getSize when no entries have been set" << std::endl;

		}

		//Test getSize
		SECTION("Record: getsize") {
			for (auto i = 0; i < 4; ++i) {
				r0[i] = std::to_string(i);
				r1[i] = std::to_string(i + 1);
				r2[i] = std::to_string(i + 2);
				r3[i] = std::to_string(i + 3);
				r4[i] = std::to_string(i + 4);
			}

			for (auto i = 0; i < 3; ++i) {
				r5[i] = std::to_string(i);
				r6[i] = std::to_string(i + 3);
				r7[i] = std::to_string(i + 6);
				r8[i] = std::to_string(i + 9);
				r9[i] = std::to_string(i + 12);
			}


			REQUIRE(r0.getSize() == 4);
			REQUIRE(r5.getSize() == 3);
			std::cout << "Passed - Record: getSize with every entry added" << std::endl;
		}

		//Test [] operator
		SECTION("Record: [] Operator") {
			std::string str = r4[3];
			REQUIRE("7" == str);

			REQUIRE(r0[0] == "0");
			REQUIRE(r0[3] == "3");
			REQUIRE(r4[0] == "4");
			REQUIRE(r4[3] == "7");

			REQUIRE(r5[0] == "0");
			REQUIRE(r5[2] == "2");
			REQUIRE(r9[0] == "12");
			REQUIRE(r9[2] == "14");
			std::cout << "Passed - Record: [] Operator" << std::endl;
		}




		//Declaration of Table Variables
		std::vector<std::string> attributes1 = { "BadAtt1", "A", "BadAtt2", "B", "C", "D", "BadAtt3" };
		std::vector<std::string> attributes2 = { "E", "F", "G" };
		std::vector<std::string> attributes3 = { "E", "F", "G", "E(1)" };
		Database::Table table1(attributes1);
		Database::Table table2;

		SECTION("Table: constructor & addNewAtt") {
			//Test addNewAtt
			table2.addNewAtt("E");
			table2.addNewAtt("F");
			table2.addNewAtt("G");
			REQUIRE(table1.getAtts() == attributes1); 
			REQUIRE(table2.getAtts() == attributes2);
			table2.addNewAtt("E");
			REQUIRE(table2.getAtts() == attributes3); 
			std::cout << "Passed - Table: Adding attributes and getAtts" << std::endl;
		}
		
		//Test deleteAtt
		SECTION("Table: delete attribute") {
			std::vector<std::string> testDelete1 = { "A", "BadAtt2", "B", "C", "D", "BadAtt3" };
			std::vector<std::string> testDelete2 = { "A", "B", "C", "D", "BadAtt3" };
			std::vector<std::string> testDelete3 = { "A", "B", "C", "D" };
			table1.deleteAtt("BadAtt1");
			REQUIRE(table1.getAtts() == testDelete1);
			table1.deleteAtt("BadAtt2");
			REQUIRE(table1.getAtts() == testDelete2);
			table1.deleteAtt("BadAtt3");
			REQUIRE(table1.getAtts() == testDelete3);
			table1.deleteAtt("NOT-IN-TABLE");
			REQUIRE(table1.getAtts() == testDelete3);
			std::cout << "Passed - Table: deleteAtt" << std::endl;
		}
		
		//Add Records to tables
		table1.insertRecord(r0);
		table1.insertRecord(r1);
		table1.insertRecord(r2);
		table1.insertRecord(r3);
		table1.insertRecord(r4);

		table2.insertRecord(r5);
		table2.insertRecord(r6);
		table2.insertRecord(r7);
		table2.insertRecord(r8);
		table2.insertRecord(r9);

		
		//Test getSize
		SECTION("Table: getsize") {
			REQUIRE(table1.getSize() == 5);
			REQUIRE(table2.getSize() == 5);
			std::cout << "Passed - Table: insertRecord" << std::endl;
			std::cout << "Passed - Table: getSize" << std::endl;
		}

		//Test Iterator
		SECTION("Table: Iterator") {
			std::vector<Database::Record>::iterator iter = table1.recordIterator(0);
			REQUIRE(iter->getSize() == r0.getSize());
			REQUIRE(iter->operator[](2) == r0[2]);
			iter++;
			REQUIRE(iter->getSize() == r1.getSize());
			REQUIRE(iter->operator[](2) == r1[2]);
			std::cout << "Passed - Table: iterator" << std::endl;
		}

		//Test setKey
		SECTION("Table: setKey") {
			table1.setKey("A");
			table1.setKey("E");
			std::cout << "Passed - Table: setKey" << std::endl;
		}


		//Test Count
		SECTION("Table: count") {
			table1.recordIterator(0)->operator[](0) = "";
			REQUIRE(table1.count("A") == 4);
			REQUIRE(table1.count("B") == 5);
			REQUIRE(table1.count("DNE") == 0);
			std::cout << "Passed - Table: count" << std::endl;
		}


		//Test Minimum
		SECTION("Table: Minimum") {
			REQUIRE(table1.minimum("A") == 1);
			table1.recordIterator(1)->operator[](1) = "";
			REQUIRE(table1.minimum("B") == 1);
			REQUIRE(table1.minimum("DNE") == 0);
			std::cout << "Passed - Table: minimum" << std::endl;
		}

		//Test Maximum
		SECTION("Table: Maximum") {
			REQUIRE(table1.maximum("A") == 4);
			REQUIRE(table1.maximum("B") == 5);
			REQUIRE(table1.maximum("DNE") == 0);
			std::cout << "Passed - Table: maximum" << std::endl;
		}



		//Edge Cases
		Database::Table table9;
		SECTION("Table: Edge Case- size of newly constructed table is 0") {
			REQUIRE(table9.getSize() == 0);
			std::cout << "Passed - Edge Cases: size of newly constructed table has 0 attributes" << std::endl;
		}

		SECTION("Table: Edge Case- Key cannot have duplicate entries & key cannot be set to attr that does not exist") {
			table9.addNewAtt("X");
			table9.addNewAtt("Y");
			table9.addNewAtt("Z");
			table9.addNewAtt("W");
			table9.setKey("W");
			table9.setKey("NOT ATTRIBUTE");
			table9.insertRecord(r0);
			table9.insertRecord(r0);
			REQUIRE(table9.getSize() == 1);
			std::cout << "Passed - Edge Cases: Keys cannot have duplicate entries" << std::endl;
			std::cout << "Passed - Edge Cases: Keys cannot be set to attributes that do not exist" << std::endl;
		}

		
		SECTION("Table: Edge Case- record without unknown entry value will store empty string") {
			table9.insertRecord(r5);
			REQUIRE(table9.recordIterator(1)->operator[](3) == "");
			std::cout << "Passed - Edge Cases: records added to table without all attributes filled in will return empty string for undeclared entries" << std::endl;
		}
		

		SECTION("Table: Edge Case- will not change key if newKey will have duplicate entries") {
			table9.setKey("X"); //cannot change to this key or else key will have duplicate entries so key stays same
			table9.insertRecord(r6);
			REQUIRE(table9.getSize() == 2);
			std::cout << "Passed - Edge Cases: key will not change if newKey will have duplicate entries" << std::endl;
		}

		
		SECTION("Table: Edge Case- Key can be changed") {
			table9.recordIterator(0)->operator[](0) = 20;
			table9.setKey("X");
			table9.insertRecord(r1);
			REQUIRE(table9.getSize() == 3);
			std::cout << "Passed - Edge Cases: Key can be changed" << std::endl;
		}

		SECTION("Table: Edge Case- Cannot change entry if it will create duplicate entry in key") {
			table9.recordIterator(0)->operator[](0) = "0";
			REQUIRE(table9.recordIterator(0)->operator[](0) == "0");
			std::cout << "Passed - Edge Cases: Cannot change entry if it will create a duplicate in key" << std::endl;
		}

		SECTION("Table: Edge Case- Key can be set to nothing") {
			table9.setKey("");
			table9.recordIterator(0)->operator[](0) = "0";
			REQUIRE(table9.recordIterator(0)->operator[](0) == "0");
			std::cout << "Passed - Edge Cases: Key can be set to nothing" << std::endl;
		}


		//Create Tables
		std::vector<std::string> attributes14 = { "C", "F", "G" };
		Database::Table table14(attributes14);
		table14.insertRecord(r5);
		table14.insertRecord(r6);
		table14.insertRecord(r7);
		table14.insertRecord(r8);
		table14.insertRecord(r9);


		//Test crossTables
		Database::Table table3 = table1.crossTables(table1, table14);
		
		//Make a "test" cross join table manually to compare with result of crossTables function
		std::vector<std::string> attributes4 = { "A", "B", "C", "D", "C(1)", "F", "G" };
		Database::Table table4(attributes4);

		SECTION("Table: crossTables") {
			Database::Record currRec(7);
			int entryNum = 0;
			for (int i = 0; i < table1.getSize(); ++i) {
				for (int j = 0; j < table14.getSize(); ++j) {
					for (auto k = 0; k < 4; ++k) {
						currRec[entryNum] = std::to_string(k + i);
						++entryNum;
					}
					for (auto k = 0; k < 3; ++k) {
						currRec[entryNum] = std::to_string(3 * j + k);
						++entryNum;
					}

					table4.insertRecord(currRec);
					entryNum = 0;
				}
			}

			REQUIRE(table3.getAtts() == attributes4);
			for (auto i = 0; i < table3.getSize(); ++i) {
				for (auto j = 0; j < table3.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table3.recordIterator(i)->operator[](j) == table4.recordIterator(i)->operator[](j));
				}
			}
			std::cout << "Passed - Table: crossTables" << std::endl;
		}


		//Test natJoinTables; new Table "table5" will replace "table14" to provide a more useful test case
		//----------------------------------------------
		//Create table5:

		std::vector<std::string> attributes5 = { "A", "D", "E" };
		Database::Table table5(attributes5);
		table5.setKey("A");

		Database::Record r10(3);
		Database::Record r11(3);
		Database::Record r12(3);
		Database::Record r13(3);
		Database::Record r14(3);

		r10[0] = "0"; r10[1] = "1"; r10[2] = "100";
		r11[0] = "2"; r11[1] = "2"; r11[2] = "200";
		r12[0] = "4"; r12[1] = "3"; r12[2] = "300";
		r13[0] = "6"; r13[1] = "4"; r13[2] = "400";
		r14[0] = "8"; r14[1] = "5"; r14[2] = "500";

		table5.insertRecord(r10);
		table5.insertRecord(r11);
		table5.insertRecord(r12);
		table5.insertRecord(r13);
		table5.insertRecord(r14);
		//----------------------------------------------
		

		Database::Table table6 = table1.natJoinTables(table1, table5);

		//Make a "test" natural join table manually to compare with result of natJoinTables function
		std::vector<std::string> attributes7 = { "A", "B", "C", "D", "D(1)", "E" };
		Database::Table table7(attributes7);

		Database::Record r15(6);
		Database::Record r16(6);
		Database::Record r17(6);
		Database::Record r18(6);
		Database::Record r19(6);
		Database::Record r20(6);
		Database::Record r21(6);

		r15[0] = "0"; r15[1] = "1"; r15[2] = "2"; r15[3] = "3"; r15[4] = "1"; r15[5] = "100";
		r16[0] = "1"; r16[1] = "2"; r16[2] = "3"; r16[3] = "4"; r16[4] = "*"; r16[5] = "*";
		r17[0] = "2"; r17[1] = "3"; r17[2] = "4"; r17[3] = "5"; r17[4] = "2"; r17[5] = "200";
		r18[0] = "3"; r18[1] = "4"; r18[2] = "5"; r18[3] = "6"; r18[4] = "*"; r18[5] = "*";
		r19[0] = "4"; r19[1] = "5"; r19[2] = "6"; r19[3] = "7"; r19[4] = "3"; r19[5] = "300";
		r20[0] = "6"; r20[1] = "*"; r20[2] = "*"; r20[3] = "*"; r20[4] = "4"; r20[5] = "400";
		r21[0] = "8"; r21[1] = "*"; r21[2] = "*"; r21[3] = "*"; r21[4] = "5"; r21[5] = "500";

		table7.insertRecord(r15);
		table7.insertRecord(r16);
		table7.insertRecord(r17);
		table7.insertRecord(r18);
		table7.insertRecord(r19);
		table7.insertRecord(r20);
		table7.insertRecord(r21);

		SECTION("Table: natJoinTables") {
			REQUIRE(table6.getAtts() == table7.getAtts());
			for (auto i = 0; i < table6.getSize(); ++i) {
				for (auto j = 0; j < table6.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table6.recordIterator(i)->operator[](j) == table7.recordIterator(i)->operator[](j));
				}
			}
			std::cout << "Passed - Table: natJoinTables" << std::endl;
		}


		//-----------------------------------------------
		//Database
		//-----------------------------------------------
		Database::Database database;

		//Test addTable
		SECTION("Database: addTable") {
			database.addTable(table1, "table1");
			database.addTable(table2, "table2");
			database.addTable(table3, "table3");
			database.addTable(table4, "table4");
			std::vector<std::string> listTables1 = { "table1", "table2", "table3", "table4" };

			REQUIRE(database.listTables() == listTables1);
			std::cout << "Passed - Database: addTable" << std::endl;
		}
		
		//Test dropTable
		SECTION("Database: dropTable") {
			database.dropTable("table3");
			database.dropTable("table4");
			std::vector<std::string> listTables2 = { "table1", "table2" };
			REQUIRE(database.listTables() == listTables2);
			std::cout << "Passed - Database: dropTable" << std::endl;
			std::cout << "Passed - Database: listTables" << std::endl; //listTables was tested in the process, so this passes too
		}

	    //Test getTables
		SECTION("Database: getTables") {
			std::vector<Database::Table> getTables = database.getTables();
			REQUIRE(getTables.size() == 2);
			Database::Table table15 = getTables[0];
			Database::Table table16 = getTables[1];
			REQUIRE(table1.getAtts() == table15.getAtts());
			for (auto i = 0; i < table1.getSize(); ++i) {
				for (auto j = 0; j < table1.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table1.recordIterator(i)->operator[](j) == table15.recordIterator(i)->operator[](j));
				}
			}
			REQUIRE(table2.getAtts() == table16.getAtts());
			for (auto i = 0; i < table2.getSize(); ++i) {
				for (auto j = 0; j < table2.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table2.recordIterator(i)->operator[](j) == table16.recordIterator(i)->operator[](j));
				}
			}
			std::cout << "Passed - Database: getTables" << std::endl;
		}
			

		//Test query
		SECTION("Query: test the * operator in select") {
			//----------------------------------------------
			//Query test 1:
			Database::Table table8 = database.query("*", "table1", "");
			REQUIRE(table8.getAtts() == table1.getAtts());
			for (auto i = 0; i < table8.getSize(); ++i) {
				for (auto j = 0; j < table8.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table8.recordIterator(i)->operator[](j) == table1.recordIterator(i)->operator[](j));
				}
			}

			std::cout << "Passed -Query: test the * operator in select" << std::endl;
		}
			//----------------------------------------------
			//Query test 2:
		SECTION("Query: test > operator") {
			Database::Table table10 = database.query("A", "table1", "A > 0");

			std::vector<std::string> attributes11 = { "A" };
			Database::Table table11(attributes11);

			Database::Record r22(1);
			Database::Record r23(1);
			Database::Record r24(1);
			Database::Record r25(1);

			r22[0] = "1";
			r23[0] = "2";
			r24[0] = "3";
			r25[0] = "4";

			table11.insertRecord(r22);
			table11.insertRecord(r23);
			table11.insertRecord(r24);
			table11.insertRecord(r25);

			REQUIRE(table10.getAtts() == table11.getAtts());
			for (auto i = 0; i < table10.getSize(); ++i) {
				for (auto j = 0; j < table10.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table10.recordIterator(i)->operator[](j) == table11.recordIterator(i)->operator[](j));
				}
			}
			std::cout << "Passed -Query: test > operator" << std::endl;
		}

			//----------------------------------------------
			//Query test 3:
		SECTION("Query: test comparison operator in where argument and OR operator") {
			Database::Table table12 = database.query("A", "table1", "A == 0 OR B == 2");
			std::vector<std::string> attributes13 = { "A" };
			Database::Table table13(attributes13);

			Database::Record r26(1);
			Database::Record r27(1);

			r26[0] = "0";
			r27[0] = "1";

			table13.insertRecord(r26);
			table13.insertRecord(r27);

			REQUIRE(table12.getAtts() == table13.getAtts());
			for (auto i = 0; i < table12.getSize(); ++i) {
				for (auto j = 0; j < table12.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table12.recordIterator(i)->operator[](j) == table13.recordIterator(i)->operator[](j));
				}
			}
			std::cout << "Passed -Query: test comparison operator in where argument and OR operator" << std::endl;
		}

			//----------------------------------------------
			//Query test 4:
		SECTION("Query: Out of order select, Not, <>, >, OR are tested") {
			Database::Table table15 = database.query("G, C, F", "table14", "NOT (C <> 0 OR (F > 4))");
			std::vector<std::string> attributes15 = { "G", "C", "F" };
			Database::Record r28(3);
			r28[0] = "2";
			r28[1] = "0";
			r28[2] = "1";

			Database::Table table16(attributes15);
			table16.insertRecord(r28);

			REQUIRE(table15.getAtts() == table16.getAtts());
			for (auto i = 0; i < table15.getSize(); ++i) {
				for (auto j = 0; j < table15.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table15.recordIterator(i)->operator[](j) == table16.recordIterator(i)->operator[](j));
				}
			}
			std::cout << "Passed -Query: Out of order select, Not, <>, >, OR are tested" << std::endl;
		}

			//----------------------------------------------
			//Query test 5:
		SECTION("Query: Fail query if select columns are not all attributes") {
			Database::Table table15 = database.query("G, A, C", "table14", "NOT (C <> 0 OR (F > 4))");

			REQUIRE(table15.getAtts().empty());
			std::cout << "Passed -Query: Fail query if select columns are not all attributes" << std::endl;
		}


			//----------------------------------------------
			//Query test 6:
		SECTION("Query: Fail query if from argument is not found in table") {
			Database::Table table15 = database.query("G, F, C", "table doesnt exist", "B = 0");

			REQUIRE(table15.getAtts().empty());
			std::cout << "Passed - Query: Fail query if from argument is not found in table" << std::endl;
		}

			//----------------------------------------------
			//Query test 7:
		SECTION("Query: Correctly use AND, <=, >=") {
			Database::Table table15 = database.query("G", "table14", "C <= 3 AND F >= 4");
			std::vector<std::string> attributes15 = { "G" };
			Database::Record r28(1);
			r28[0] = "5";

			Database::Table table16(attributes15);
			table16.insertRecord(r28);
			REQUIRE(table15.getAtts() == table16.getAtts());
			for (auto i = 0; i < table15.getSize(); ++i) {
				for (auto j = 0; j < table15.recordIterator(i)->getSize(); ++j) {
					REQUIRE(table15.recordIterator(i)->operator[](j) == table16.recordIterator(i)->operator[](j));
				}
			}
			std::cout << "Passed - Query: Correctly use AND, <=, >=" << std::endl;
		}


			//----------------------------------------------
			//Query test 8:
		SECTION("Query: Fail query if parenthesis are unbalanced") {
			Database::Table table15 = database.query("G", "table14", "((C <= 3) AND F >= 4)))");

			REQUIRE(table15.getAtts().empty());
			std::cout << "Passed - Query: Fail query if parenthesis are unbalanced" << std::endl;
		}	
	}
	