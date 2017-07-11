-------------------------------------------------------------------------
APPLICATION README
-------------------------------------------------------------------------

-------------------------------------------
RUNNING THE APPLICATION
-------------------------------------------

The Yelp data accessing application using the database provided by our
partner team needs to be run by doing the following in Putty (the server
is build.tamu.edu):

----- To compile: -----
make

----- To run after compiling: -----

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. && ./main.out

-------------------------------------------
USING THE APPLICATION
-------------------------------------------

Once the application is running, the screen will display:

Welcome to Yelp Analytics!
What can I help you find today?
Enter '1': Search for User
Enter '2': Search for Business
Enter '3': Get Reviews for Business
Enter '4': Get Reviews By User
Enter '5': Get Tips for a Business
Enter '6': Accuracy of Users Reviews

You should then enter the corresponding number for what type of
information you're looking for.

If the number 1 or 2 was selected, then the screen will then ask
you to enter the filters you want to apply to your search. These filters 
can be names, stars, or even categories. Once you have selected the 
filters you would like to apply, you will be prompted to fill in what
each filter should contain. Some filters will use a string. Some
filters will ask for an operator and number (EX: > 3), but it is important 
that these numbers be integers (the database provided does not support floating
point numbers). And the categories and attributes filters will as you
to type a number corresponding to the category or attribute you would
like to apply. After hitting enter, you will be able to review you filters.
Enter 'Y' if the input looks correct. You are then asked if you would like
to add statistics to the filter. Enter 'Y' to perform statistics. The output
should be printed to the console.

If you enter 3, 4, 5, or 6 at the main screen, you will be asked if you have
an ID value ready. If you have a business or user ID ready to enter, select
'1' and then enter your ID to perform the application. If you need to 
look up a user or business ID, select '2' and you will be redirected to the
main menu. You can find business and user IDs be using the 'Search for User'
and 'Search for Business' subapplications.

-------------------------------------------
MINIMUM REQUIREMENT FEATURES
-------------------------------------------

Given the inital navigation menu explained previously, as it can be seen,
the inputs 1, 2, 3, and 4 (Search for User, Search for Business, Get
Reviews for Business, Get Reviews By User) fulfills everything which
satisfys the minimum requirements of the application. These features
implement displaying information about a user(s) (1), displaying
information about a business(es) (2), and combining information from more
than one table (3, 4).

-------------------------------------------
EXTRA FEATURES
-------------------------------------------

There are three extra features which were added to the application
expanding from the minimum requirements:

1) Getting the tips for a business:
   If you enter 5 at the navigation menu of the application, the screen
   will have you put in a business ID, and then if the ID exists within
   the "tip" table, it will display to the screen the information of all
   tips given to that business.
2) Getting the accuracy of a user's reviews:
   If you enter 6 at the navigation menu of the application, the screen
   will have you put in a user ID, and if that user exists, the
   application will calculate how much the user's average review deviates
   from the average users' average reviews. This can be used to determine
   whether a given user is honest or an optimest/pessimest.
3) Printing statistics of a filtered search:
   When using options 1 or 2 on the navigation menu and use filters to
   print certain infromation about particular users/businesses, the
   application will give you the option to also display the statistics
   of the printed results. This includes the mean, maximum, and minimum
   values of the numeric attributes being displayed to the screen (as
   well as a couple other statistics).

-------------------------------------------
HOW WE MADE USE OF DATABASE FUNCTIONALITY
-------------------------------------------

Given our partner team's database, the primary functionality of the
database was used for the following purposes (this exludes obvious
database functionality that was used, including constructors, "get"
functions, etc.):

Database::query - This function was used for all 6 primary functions
of our database. Being the main engine behind how our application was
implemented, it was used for searching through the significant amounts
of data in the database to provide the user using the application with
information that corresponds to specifically what they are looking for.

Table::count, max, min - These three functions were used specifically
for printing the statistics of a set of tables printed to the user which
satisfy specified filters. For example, the "count" function was used to
determine the number of records in a table which have a value specified
for a certain attribute; the max and min functions were used for
printing the resulting statistics to the user.

-------------------------------------------
INSOLUBLE LIMITATIONS TO THEIR DATABASE
-------------------------------------------

The natural join provided was not fully functional for the needs of
our application. Since this method did not support various combinations
of tables, we had to create our own naturalTableJoin function.

Most of the limitations of the partner team's database (adding space
between comparison operators in the "where" argument of the query,
issues with the natural join) were able to be solved while implementing
the application. However, the one limitation which couldn't be resolved
involves using floats/doubles in their query function. Their database
would throw an exception in this case.

-------------------------------------------
EXTERNAL LIBRARIES USED
-------------------------------------------

The only external library used in this application was called "json.hpp"
which helped with parsing the .json files.

-------------------------------------------------------------------------
Hope you enjoy the application!
-------------------------------------------------------------------------
