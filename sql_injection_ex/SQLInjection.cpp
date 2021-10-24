// File: SQLInjection.cpp
//

#include <algorithm>
#include <iostream>
#include <locale>
#include <string>
#include <tuple>
#include <vector>

#include "./include/sqlite3.h"

//DO NOT CHANGE
typedef std::tuple<std::string, std::string, std::string> user_record;
const std::string str_where = " where ";

/**
 * @brief
 *
 * @note DO NOT CHANGE
 *
 * @param possible_vector
 * @param argc
 * @param argv
 * @param azColName
 * @return 0 if successful
 */
static int callback(void *possible_vector, int argc, char **argv, char **azColName)
{
	if (possible_vector == NULL)
	{
		for (int i = 0; i < argc; i++)
		{
			std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
		}
		std::cout << std::endl;
	}
	else
	{
		auto *rows = static_cast<std::vector<user_record> *>(possible_vector);
		rows->push_back(std::make_tuple(argv[0], argv[1], argv[2]));
	}
	return 0;
}

/**
 * @brief Initialize a hard-coded portable SQLite3 database.
 *
 * @note DO NOT CHANGE
 *
 * @param db as the database
 * @return true if successful.
 */
bool initialize_database(sqlite3 *db)
{
	char *error_message = NULL;
	std::string sql = "CREATE TABLE USERS("
					  "ID INT PRIMARY KEY     NOT NULL,"
					  "NAME           TEXT    NOT NULL,"
					  "PASSWORD       TEXT    NOT NULL);";

	int result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
	if (result != SQLITE_OK)
	{
		std::cerr << "Failed to create USERS table. ERROR = " << error_message << std::endl;
		sqlite3_free(error_message);
		return false;
	}
	std::cout << "USERS table created." << std::endl;

	// insert some dummy data
	sql = "INSERT INTO USERS (ID, NAME, PASSWORD)"
		  "VALUES (1, 'Fred', 'Flinstone');"
		  "INSERT INTO USERS (ID, NAME, PASSWORD)"
		  "VALUES (2, 'Barney', 'Rubble');"
		  "INSERT INTO USERS (ID, NAME, PASSWORD)"
		  "VALUES (3, 'Wilma', 'Flinstone');"
		  "INSERT INTO USERS (ID, NAME, PASSWORD)"
		  "VALUES (4, 'Betty', 'Rubble');";

	result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
	if (result != SQLITE_OK)
	{
		std::cout << "Data failed to insert to USERS table. ERROR = " << error_message << std::endl;
		sqlite3_free(error_message);
		return (false);
	}

	return (true);
}

/**
 *
 * @brief Query records using sql string.
 *
 * @note You cannot just flag 1=1 as an error, since 2=2 will work just as well. You need something more generic
 * @param db  as the database
 * @param sql as formatted query string
 * @param records as the tuple {id, name, password}
 * @return true if successful
 *
 * TODO:  Fix run_query to fail and display an error if there is a suspected SQL Injection
 */
bool run_query(sqlite3 *db, const std::string &sql, std::vector<user_record> &records)
{
	records.clear(); // clear any prior results
	char *error_message = nullptr;

	// Select * from a database without doing an additional for-each loop.
	if (sql.length() > 25)
	{
		// Reference:
		//  "SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME='Fred' or 1=1;"
		//  if second equal operator '=' or 61 is found, there is an or statement.
		//  (for this example at least)

		// search between the first '='+1 to the last character.
		for (const auto c : (sql.substr(sql.find(61) + 1, sql.length())))
		{
			//  if a second '=' is found, don't provide records. "sql injection"
			if (c == 61)
			{
				// show message and return false.
				std::cout << "[SQL INJECTION] Suspected internal string manipulation No records will be returned" << std::endl;
				return (false);
			}
		}
	}

	if (sqlite3_exec(db, sql.c_str(), callback, &records, &error_message) != SQLITE_OK)
	{
		std::cout << "Data failed to be queried from USERS table. ERROR = " << error_message << std::endl;
		sqlite3_free(error_message);
		return (false);
	}
	return (true);
}

/**
 * @brief Run the SQL Injection. *(clearly not an issue)
 *
 * @details Take a normal query, duplicate the string, pop the last value of the
 *  string ';' and concatenate 'OR (true=true);' logic. Thus 'dumping' the hardcoded, unencrypted in-memory
 *  database.
 *
 * @note DO NOT CHANGE
 *
 * @param db as the database.
 * @param sql as the SQL query.
 * @param records as the record(s) to be printed.
 * @return true if successfully causes an "sql injection".
 */
bool run_query_injection(sqlite3 *db, const std::string &sql, std::vector<user_record> &records)
{
	std::string injectedSQL(sql);
	std::string localCopy(sql);

	// we work on the local copy because of the const
	std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);

	if (localCopy.find_last_of(str_where) >= 0)
	{
		// this sql has a where clause
		if (localCopy.back() == ';')
		{
			// smart SQL developer terminated with a semicolon - we can fix that!
			injectedSQL.pop_back();
		}
		switch ((rand() % 4))
		{
		case 1:
			injectedSQL.append(" or 2=2;");
			break;
		case 2:
			injectedSQL.append(" or 'hi'='hi';");
			break;
		case 3:
			injectedSQL.append(" or 'hack'='hack';");
			break;
		case 0:
		default:
			injectedSQL.append(" or 1=1;");
			break;
		}
	}
	return run_query(db, injectedSQL, records);
}

/**
 * @brief Prints every record.
 *  {for each record in records; print record}
 *
 * @note DO NOT CHANGE
 * @param sql as the query
 * @param records as every record in the entire database. Secure.
 */
void dump_results(const std::string &sql, const std::vector<user_record> &records)
{
	std::cout << std::endl
			  << "SQL: " << sql << " ==> " << records.size() << " records found." << std::endl;

	for (auto &record : records)
	{
		std::cout << "User: " << std::get<1>(record) << " [UID=" << std::get<0>(record) << " PWD=" << std::get<2>(record) << "]" << std::endl;
	}
}

/**
 * @brief Run a 'secure' query on an non-encrypted database containing passwords.
 *
 * @note DO NOT CHANGE
 * @param db
 */
void run_queries(sqlite3 *db)
{
	char *error_message;
	std::vector<user_record> records;

	std::cout << "\n* [Performing SQL QUERY ALL]" << std::endl; // THE ONLY THING ADDED FOR CLARITY

	// query all
	std::string sql = "SELECT * from USERS";
	if (!run_query(db, sql, records))
		return;
	dump_results(sql, records);

	std::cout << "\n* [Performing SQL QUERY]" << std::endl; // THE ONLY THING ADDED FOR CLARITY

	//  Query One record from database
	sql = "SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME='Fred'";
	if (!run_query(db, sql, records))
		return;
	dump_results(sql, records);

	std::cout << "\n** [Performing SQL INJECTION]" << std::endl; // THE ONLY THING ADDED FOR CLARITY

	//  run query 1 with injection 5 times
	for (auto i = 0; i < 5; ++i)
	{
		if (!run_query_injection(db, sql, records))
			continue;
		dump_results(sql, records);
	}
}

/**
 * @brief Entry Point
 *
 * @note You can change main by adding stuff to it, but all of the existing code must remain,
 *  and be in the in the order called, and with none of this existing code placed into
 *  conditional statements
 *
 * @return 0 if successful otherwise return 1 (failure)
 */
int main()
{
	int return_code = 0;
	// initialize random seed:
	srand(time(NULL));

	std::cout << "*** SQL Injection Example\n"
			  << std::endl;
	char *error_message = NULL;

	sqlite3 *db = NULL;
	// the database handle
	int result = sqlite3_open(":memory:", &db);

	if (result != SQLITE_OK)
	{
		std::cout << "Failed to connect to the database and terminating. ERROR=" << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	std::cout << "* Connected to the database.\n"
			  << std::endl;
	// initialize our database
	if (!initialize_database(db))
	{
		std::cout << "Database Initialization Failed. Terminating." << std::endl;
		return_code = -1;
	}
	else
	{
		run_queries(db);
	}

	// close the connection if opened
	if (db != NULL)
	{
		sqlite3_close(db);
	}

	return return_code;
}