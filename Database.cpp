#include "DatabaseLibrary.h"


namespace DatabaseLibrary {
    //Constructor for an empty database
    Database::Database(){
        
    }
    
    // Create an table in Database with argument name
    void Database::addTable(Table table, std::string tableName){
        std::string originalName = tableName;
        int count = 1;
        while(std::find(tableNames.begin(), tableNames.end(), tableName) != tableNames.end()){
            tableName = originalName + "(" + std::to_string(count) + ")";
            ++count;
        }
        tables.push_back(table);
        tableNames.push_back(tableName);
    }
    
    // Delete a table in Database with argument name
    void Database::deleteTable(std::string tableName){
        std::vector<std::string>::iterator iter = std::find(tableNames.begin(), tableNames.end(), tableName);
        size_t distance = std::distance(tableNames.begin(), iter);
        
        if(distance != tableNames.size()){
            tables.erase(tables.begin() + distance);
            tableNames.erase(tableNames.begin() + distance);
        } else {
            std::cout << "Table was not found" << std::endl;
        }
    }
    
    //List all tables in Database
    std::vector<std::string> Database::getTableNames() const {
        return tableNames;
    }
    
    //Return all tables in Database
    std::vector<Table> Database::getTables() const {
        return tables;
    }

	//Return an all uppercase version of the input string
	std::string Database::stringToUpper(std::string inputString) {
		std::transform(inputString.begin(), inputString.end(), inputString.begin(), ::toupper);
		return inputString;

	}

    //Run query
    Table Database::query(std::string selectArg, std::string fromArg, std::string whereArg){
		//Declare all tables used in query function
		Table resultTable;
		Table errorTable;
		Table fromTable;

		//Find from argument in database
		size_t tableIndex;
		std::vector<std::string>::iterator iter = std::find(tableNames.begin(), tableNames.end(), fromArg);
		std::vector<int> indexAttributes;
		std::vector<std::string>::iterator indexIterator;

		//If fromTable is not in database, throw an error
		if (iter == tableNames.end()) {
			std::cout << "Table not found in query" << std::endl;
			return errorTable;
		}
		tableIndex = std::distance(tableNames.begin(), iter);
		fromTable = tables.at(tableIndex);


		//Find 'selectArg' arguments in from table
		if (selectArg == "*") {
			for (size_t i = 0; i < fromTable.getAttributes().size(); ++i) {
				resultTable.addAttribute(fromTable.getAttributes().at(i));
				indexAttributes.push_back(i);
			}
		}
		else {
			Table testAttributeTable(selectArg);
			std::vector<std::string> attributesCopy = fromTable.getAttributes();
			for (size_t i = 0; i < testAttributeTable.getAttributes().size(); i++) {
				std::string attribute = testAttributeTable.getAttributes().at(i);
				indexIterator = std::find(attributesCopy.begin(), attributesCopy.end(), attribute);
				if (indexIterator == attributesCopy.end()) {
					std::cout << "At least one of your select attributes is not present in the table you are selecting from" << std::endl;
					return errorTable;
				}
				else {
					resultTable.addAttribute(testAttributeTable.getAttributes().at(i));
					indexAttributes.push_back(std::distance(attributesCopy.begin(), indexIterator));
				}
			}
		}

		//Parse several layers of the 'whereArg' argument
		std::vector<std::string> parsedString = ParseForSyntax(whereArg);
		std::vector<int> expressionResult;
		std::vector<std::vector<int>> lookupExpression;

		//Evaluate where operators one parse at a time
		std::stack<std::string> resultStack = ParseForOperators(fromTable, parsedString, lookupExpression);
		std::stack<std::string> workableStack = ParseForLogic(fromTable, resultStack, lookupExpression);
		if (workableStack.size() == 0) {
			return errorTable;
		}

		//LookupExpression contains the indexes of all records that will be in our resultTable, so we must convert this vector of indexes into a Table
		expressionResult = lookupExpression.at(stoi(workableStack.top()));
		for (size_t i = 0; i < expressionResult.size(); ++i) {
			Record newRecord(indexAttributes.size());
			for (size_t j = 0; j < indexAttributes.size(); ++j) {
				newRecord.setEntry(j, fromTable.recordIterator(expressionResult.at(i))->getEntry(indexAttributes.at(j)));
			}
			resultTable.addRecord(newRecord);
		}
		
        return resultTable;
    }

	//Parse all logical operators so that only one argument is left in the workable stack
	std::stack<std::string> Database::ParseForLogic(Table fromTable, std::stack<std::string> &resultStack, std::vector<std::vector<int>> &lookupExpression) {
		int parenthesisCounter = 0;
		std::vector<int> expressionResult;
		std::stack<std::string> workableStack;
		std::stack<std::string> errorStack;

		//use stack isntead of result vector
		while (resultStack.size() != 0) {
			workableStack.push(resultStack.top());
			resultStack.pop();
			std::string poppedOperator = "";
			std::string popppedArgument = "";
			
			//If an AND or OR operator is found, then evaulate logical operator and put the result in the lookupExpression
			if (workableStack.top() == "AND" || workableStack.top() == "OR") {
				if (workableStack.size() > 1 && resultStack.size() >= 1) {
					poppedOperator = workableStack.top();
					workableStack.pop();
					popppedArgument = resultStack.top();
					resultStack.pop();

					//If a NOT operator is found, we must first evaluate the NOT
					if (resultStack.size() != 0 && resultStack.top() == "NOT") {
						expressionResult = evaluateNot(fromTable, lookupExpression.at(stoi(popppedArgument)));
						lookupExpression.push_back(expressionResult);
						resultStack.pop();
						resultStack.push(std::to_string(lookupExpression.size() - 1));
					}
					else {
						resultStack.push(popppedArgument);
					}

					//Evaluate the logical operator and save the result in the lookupexpession vector. A reference to the vector that was just pushed in is saved in the workable stack
					expressionResult = evaluateAndOr(fromTable, lookupExpression.at(stoi(workableStack.top())), lookupExpression.at(stoi(resultStack.top())), poppedOperator);
					workableStack.pop();
					resultStack.pop();
					lookupExpression.push_back(expressionResult);
					workableStack.push(std::to_string(lookupExpression.size() - 1));
				}
				else {
					std::cout << "AND and OR operators must have a left and right hand side argument" << std::endl;
					return errorStack;
				}
			}

			//If a NOT has been triggered, than evaluate. Save the result in the lookupExpression vector. A reference to this vector is pushed into the workable stack
			else if (workableStack.top() == "NOT") {
				if (workableStack.size() >= 2) {
					poppedOperator = workableStack.top();
					workableStack.pop();
					expressionResult = evaluateNot(fromTable, lookupExpression.at(stoi(workableStack.top())));
					workableStack.pop();
					lookupExpression.push_back(expressionResult);
					workableStack.push(std::to_string(lookupExpression.size() - 1));
				}
			}

			//Keep note of the open and close parenthesis to make sure the user input has balanced parenthesis
			else if (workableStack.top() == "(") {
				--parenthesisCounter;
				if (parenthesisCounter < 0) {
					std::cout << "Parenthesis must be balanced in your where argument" << std::endl;
					return errorStack;
				}
				workableStack.pop();
				popppedArgument = workableStack.top();
				workableStack.pop();
				if (workableStack.top() != ")") {
					std::cout << "You must have balanced parenthesis" << std::endl;
					return errorStack;
				}
				workableStack.pop();
				workableStack.push(popppedArgument);
			}
			else if (workableStack.top() == ")") {
				++parenthesisCounter;
			}
		}

		if (parenthesisCounter != 0) {
			std::cout << "You must have balanced parenthesis" << std::endl;
			return errorStack;
		}

		return workableStack;
	}


	//Parse all operators and evaluate
	std::stack<std::string> Database::ParseForOperators(Table fromTable, std::vector<std::string> parsedString, std::vector<std::vector<int>> &lookupExpression) {
		//Declare possible operators to look for
		std::vector<std::string> operators = { "<", ">", "<>", "<=", ">=", "="};
		std::vector<std::string> notOperators = {"AND", "OR", "NOT", "(" , ")"};
		std::vector<int> expressionResult;
		std::stack<std::string> resultStack;
		std::stack<std::string> errorStack;

		//Look for all operators and evaluate the left and right arguments
		for (size_t i = 0; i < parsedString.size(); ++i) {
			std::string upperString = stringToUpper(parsedString[i]);
			if (std::find(operators.begin(), operators.end(), parsedString[i]) != operators.end()) {
				if (i > 0 && i <= parsedString.size() - 2) {
					expressionResult = evaluateOperator(fromTable, parsedString[i - 1], parsedString[i + 1], parsedString[i]);
					lookupExpression.push_back(expressionResult);
					resultStack.push(std::to_string(lookupExpression.size() - 1));
				}
				else {
					std::cout << "Operator must have a left hand and right side arguement." << std::endl;
					return errorStack;
				}
			}
			else if (std::find(notOperators.begin(), notOperators.end(), upperString) != notOperators.end()) {
				resultStack.push(upperString);
			}
		}

		return resultStack;
	}


    //Print the names of all the tables in the database
    void Database::printDatabase() {
        for(size_t i = 0; i < tables.size(); ++i){
            std::cout << tableNames[i] << std::endl;
            tables[i].printTable();
            std::cout << std::endl;
        }
    }

	//return true if an entire string is made of only digits
	bool Database::isDigits(const std::string &str) {
		return std::all_of(str.begin(), str.end(), isdigit);
	}

	//return true if input string is floating number
	bool Database::isFloat(const std::string &str) {
		std::istringstream iss(str);
		float f;
		iss >> f;
		//Check entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}

	//Evaluate all operators and return a vector of indexes which show were the records needed are located in the from table
	std::vector<int> Database::evaluateOperator(Table fromTable, std::string attribute, std::string data, std::string operators) {
		std::vector<int> errorVector;
		std::vector<int> resultVector;

		//Look for attributes that are being referenced by the query
		size_t indexAttr = 0;
		std::vector<std::string>::iterator iter;
		std::vector<std::string> fromAttributes = fromTable.getAttributes();
		iter = std::find(fromAttributes.begin(), fromAttributes.end(), attribute);
		if (iter == fromAttributes.end()) {
			std::cout << "'where' columns in query cannot be found" << std::endl;
			return errorVector;
		}
		indexAttr = std::distance(fromAttributes.begin(), iter);

		//Execute operation for all records in table
		for (size_t i = 0; i < fromTable.getNumRecords(); ++i) {
			std::string value = fromTable.recordIterator(i)->getEntry(indexAttr);
			if (isDigits(value) && isDigits(data)) {
				int intValue = stoi(value);
				int intData = stoi(data);

				// If the items we are comparing are integers convert to int before doing the comparison
				if (operators == "=" && intValue == intData) {
					resultVector.push_back(i);
				}
				else if (operators == "<" && intValue < intData) {
					resultVector.push_back(i);
				}
				else if (operators == ">" && intValue > intData) {
					resultVector.push_back(i);
				}
				else if (operators == "<=" && intValue <= intData) {
					resultVector.push_back(i);
				}
				else if (operators == ">=" && intValue >= intData) {
					resultVector.push_back(i);
				}
				else if (operators == "<>" && intValue != intData) {
					resultVector.push_back(i);
				}
			}

			else if (isFloat(value) && isFloat(data)) {
				float floatValue = stof(value);
				float floatData = stof(data);

				// If the items we are comparing are float convert to float before doing the comparison
				if (operators == "=" && floatValue == floatData) {
					resultVector.push_back(i);
				}
				else if (operators == "<" && floatValue < floatData) {
					resultVector.push_back(i);
				}
				else if (operators == ">" && floatValue > floatData) {
					resultVector.push_back(i);
				}
				else if (operators == "<=" && floatValue <= floatData) {
					resultVector.push_back(i);
				}
				else if (operators == ">=" && floatValue >= floatData) {
					resultVector.push_back(i);
				}
				else if (operators == "<>" && floatValue != floatData) {
					resultVector.push_back(i);
				}
			}

			else {
				// If the items we are comparing are not numeric, then perform bit wise comparisons
				if (operators == "=" && value == data) {
					resultVector.push_back(i);
				}
				else if (operators == "<" && value < data) {
					resultVector.push_back(i);
				}
				else if (operators == ">" && value > data) {
					resultVector.push_back(i);
				}
				else if (operators == "<=" && value <= data) {
					resultVector.push_back(i);
				}
				else if (operators == ">=" && value >= data) {
					resultVector.push_back(i);
				}
				else if (operators == "<>" && value != data) {
					resultVector.push_back(i);
				}
			}
		}

		return resultVector;
	}


	//Evaluate AND and OR operators and return a vector of indexes which show were the records needed are located in the from table
	std::vector<int> Database::evaluateAndOr(Table fromTable, std::vector<int> table1, std::vector<int> table2, std::string operators) {
		std::vector<int> resultVector;
		int prevInt;

		//Evaluate if opertor is AND
		if (operators == "AND") {
			for (size_t i = 0; i < table1.size(); ++i) {
				if (std::find(table2.begin(), table2.end(), table1.at(i)) != table2.end()) {
					resultVector.push_back(table1.at(i));
				}
			}
		}
		//Evaluate if operator is OR
		else if (operators == "OR") {
			table1.insert(table1.end(), table2.begin(), table2.end());
			if (table1.size() == 0) {
				return resultVector;
			}
			std::sort(table1.begin(), table1.end());
			prevInt = table1.at(0);
			resultVector.push_back(prevInt);
			for (size_t i = 1; i < table1.size(); ++i) {
				if (prevInt != table1.at(i)) {
					resultVector.push_back(table1.at(i));
				}
				prevInt = table1.at(i);
			}
		}

		return resultVector;
	}


	//Return a vector of indexes that are in fromTable but not table1
	std::vector<int> Database::evaluateNot(Table fromTable, std::vector<int> table1) {
		std::vector<int> resultVector;
		for (size_t i = 0; i < fromTable.getNumRecords(); ++i) {
			if (std::find(table1.begin(), table1.end(), i) == table1.end()) {
				resultVector.push_back(i);
			}
		}
		return resultVector;
	}


	std::vector<std::string> Database::ParseForSyntax(std::string str) {
		std::vector<std::string> operators = { "<", ">", "<>", "<=", ">=", "=", ")", "(" };

		//Make sure query is balanced and all arguments have spaces between them
		std::string::iterator iter;
		std::string filteredStr = "";
		for (iter = str.begin(); iter != str.end(); ++iter) {
			if (std::find(operators.begin(), operators.end(), std::string(1, *iter)) != operators.end()) {
				if (*iter == '<' || *iter == '>') {
					if ((iter + 1) != (str.end()) && std::find(operators.begin(), operators.end(), std::string(1, *(iter + 1))) != operators.end()) {
						filteredStr.push_back(' ');
						filteredStr.push_back(*iter);
						filteredStr.push_back(*(iter + 1));
						filteredStr.push_back(' ');
						++iter;
					}
					else {
						filteredStr.push_back(' ');
						filteredStr.push_back(*iter);
						filteredStr.push_back(' ');
					}
				}
				else {
					filteredStr.push_back(' ');
					filteredStr.push_back(*iter);
					filteredStr.push_back(' ');
				}
			}
			else {
				filteredStr.push_back(*iter);
			}

		}

		//Parse over string once more and put each argument in its own element of a vector
		std::vector<std::string> parsedArguments;
		std::istringstream iss(filteredStr);
		std::string parsedString = "";
		while (iss >> parsedString) {
			if (parsedString[0] == '"') {
				if (parsedString[parsedString.size() - 1] != '"') {
					parsedArguments.push_back(parsedString.substr(1));
					while (iss >> parsedString) {
						parsedArguments[parsedArguments.size() - 1].append(" ");
						if (parsedString[parsedString.size() - 1] == '"') {
							parsedArguments[parsedArguments.size() - 1].append(parsedString.substr(0, parsedString.size() - 1));
							break;
						}
						else {
							parsedArguments[parsedArguments.size() - 1].append(parsedString);
						}
					}
				}
				else {
					parsedArguments.push_back(parsedString.substr(1, parsedString.size() - 2));
				}
			}
			else {
				parsedArguments.push_back(parsedString);
			}
		}

		return parsedArguments;
	}
}

