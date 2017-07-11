## Database Class

* Constructor:  
&nbsp;&nbsp;&nbsp;&nbsp;`Database()`

* Adding a Table:  
&nbsp;&nbsp;&nbsp;&nbsp;`void addTable(Table new_table, std::string table_name)`

* Deleting a Table:  
&nbsp;&nbsp;&nbsp;&nbsp;`void dropTable(std::string table_name)`

* Listing Tables:  
&nbsp;&nbsp;&nbsp;&nbsp;`std::vector<std::string> listTables()`

* Retrieving Tables:  
&nbsp;&nbsp;&nbsp;&nbsp;`std::vector<Table> getTables()`

* Query Function:  
&nbsp;&nbsp;&nbsp;&nbsp;`Table query(std::string select, std::string from, std::string where)`

## Table Class

* Constructors:  
&nbsp;&nbsp;&nbsp;&nbsp;`Table()`  
&nbsp;&nbsp;&nbsp;&nbsp;`Table(std::vector<std::string> attribute_names)`

* New Attribute:  
&nbsp;&nbsp;&nbsp;&nbsp;`void addNewAtt(std::string attribute_name)`

* Delete Attribute:  
&nbsp;&nbsp;&nbsp;&nbsp;`void deleteAtt(std::string attribute_name)`

* Insert Record:  
&nbsp;&nbsp;&nbsp;&nbsp;`void insertRecord(Record new_record)`

* Get Attributes:  
&nbsp;&nbsp;&nbsp;&nbsp;`std::vector<std::string> getAtts()`

* Get size:  
&nbsp;&nbsp;&nbsp;&nbsp;`int getSize()`

* Set Key:  
&nbsp;&nbsp;&nbsp;&nbsp;`void setKey(std::string attribute_name)`

* Cross Join:  
&nbsp;&nbsp;&nbsp;&nbsp;`Table crossTables(Table table1, Table table2)`

* Natural Join:  
&nbsp;&nbsp;&nbsp;&nbsp;`Table natJoinTables(Table table1, Table table2)`

* Count Attributes:  
&nbsp;&nbsp;&nbsp;&nbsp;`int count(std::string attribute_name)`

* Determine Minimum:  
&nbsp;&nbsp;&nbsp;&nbsp;`int minimum(std::string attribute_name)`

* Determine Maximum:  
&nbsp;&nbsp;&nbsp;&nbsp;`int maximum(std::string attribute_name)`

## Record Class

* Constructor:  
&nbsp;&nbsp;&nbsp;&nbsp;`Record(int size)`

* Size of Record:  
&nbsp;&nbsp;&nbsp;&nbsp;`int getSize()`

* Accessor operator:  
&nbsp;&nbsp;&nbsp;&nbsp;`std::string& operator [] (int position)`
