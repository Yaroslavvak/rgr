#pragma once
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <stdexcept>
using namespace std;

class Database {
public:
    Database() {
        time = static_cast<chrono::milliseconds>(0);
        const char* info = "dbname=vakulchuk user=postgres password=1234 host=localhost port=5432";
        connection = PQconnectdb(info);
        if (PQstatus(connection) != CONNECTION_OK) {
            throw runtime_error(PQerrorMessage(connection));
            PQfinish(connection);
            exit(1);
        }
    }

    ~Database() {
        PQfinish(connection);
    }

    PGresult* GetTable(string table_name) {
        string query = "SELECT * FROM public.\"" + table_name + "\"";
        PGresult* res = PQexec(connection, query.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
            PQclear(res);
            return nullptr;
        }
        return res;
    }
    
    void AddRecord_user(vector<string> values) {
        PGresult* result = GetTable("User");
        for (int i = 0; i < PQntuples(result); i++) {
            if (values[0] == PQgetvalue(result, i, 0)) {
                throw runtime_error("Index " + values[0] + " is already in use");
                PQclear(result);
            }
        }
        string vals = "'";
        for (auto& val : values)
            vals += val + "', '";
        vals.erase(vals.length() - 3);
        string query = "INSERT INTO \"User\" VALUES(" + vals + ")";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void AddRecord_book(vector<string> values) {
        PGresult* result = GetTable("Book");
        for (int i = 0; i < PQntuples(result); i++) {
            if (values[0] == PQgetvalue(result, i, 0)) {
                throw runtime_error("Index " + values[0] + " is already in use");
                PQclear(result);
            }
        }
        PGresult* result_user = PQexec(connection, "SELECT \"user_id\" FROM \"User\"");
        bool f = false;
        for (int i = 0; i < PQntuples(result_user); i++) {
            if (values[4] == PQgetvalue(result_user, i, 0)) {
                f = true;
                break;
            }
        }
        PQclear(result_user);
        if (!f) 
            throw runtime_error("The foreign key does not exist in the parent table \"User\"");
        string vals = "'";
        for (auto& val : values)
            vals += val + "', '";
        vals.erase(vals.length() - 3);
        string query = "INSERT INTO \"Book\" VALUES(" + vals + ")";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void AddRecord_listing(vector<string> values) {
        PGresult* result = GetTable("Listing");
        for (int i = 0; i < PQntuples(result); i++) {
            if (values[0] == PQgetvalue(result, i, 0)) {
                throw runtime_error("Index " + values[0] + " is already in use");
                PQclear(result);
            }
        }
        PGresult* result_book = PQexec(connection, "SELECT \"book_id\" FROM \"Book\"");
        bool f = false;
        for (int i = 0; i < PQntuples(result_book); i++) {
            if (values[2] == PQgetvalue(result_book, i, 0)) {
                f = true;
                break;
            }
        }
        PQclear(result_book);
        if (!f) 
            throw runtime_error("The foreign key does not exist in the parent table \"Book\"");
        string vals = "'";
        for (auto& val : values)
            vals += val + "', '";
        vals.erase(vals.length() - 3);
        string query = "INSERT INTO \"Listing\" VALUES(" + vals + ")";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void AddRecord_request(vector<string> values) {
        PGresult* result = GetTable("Request");
        for (int i = 0; i < PQntuples(result); i++) {
            if (values[0] == PQgetvalue(result, i, 0)) {
                throw runtime_error("Index " + values[0] + " is already in use");
                PQclear(result);
            }
        }
        PGresult* result_user = PQexec(connection, "SELECT \"user_id\" FROM \"User\"");
        bool f_user = false;
        for (int i = 0; i < PQntuples(result_user); i++) {
            if (values[1] == PQgetvalue(result_user, i, 0)) {
                f_user = true;
                break;
            }
        }
        PQclear(result_user);
        if (!f_user) 
            throw runtime_error("The foreign key does not exist in the parent table \"User\"");
        PGresult* result_listing = PQexec(connection, "SELECT \"listing_id\" FROM \"Listing\"");
        bool f_listing = false;
        for (int i = 0; i < PQntuples(result_listing); i++) {
            if (values[2] == PQgetvalue(result_listing, i, 0)) {
                f_listing = true;
                break;
            }
        }
        PQclear(result_listing);
        if (!f_listing)
            throw runtime_error("The foreign key does not exist in the parent table \"Listing\"");
        string vals = "'";
        for (auto& val : values)
            vals += val + "', '";
        vals.erase(vals.length() - 3);
        string query = "INSERT INTO \"Request\" VALUES(" + vals + ")";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }
    
    void UpdateRecord_user(vector<string> values, int idx) {
        PGresult* result = GetTable("User");
        ostringstream oss;
        for (int i = 1; i < PQnfields(result); i++) {
            oss << "\"" << PQfname(result, i) << "\"='" << values[i - 1] << "'";
            if (i < PQnfields(result) - 1)
                oss << ", ";
        }
        string query = "UPDATE \"User\" SET " + oss.str() + " WHERE " + PQfname(result, 0) + "=" + to_string(idx);
        PQclear(result);
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void UpdateRecord_book(vector<string> values, int idx) {
        PGresult* result_user = PQexec(connection, "SELECT \"user_id\" FROM \"User\"");
        bool f = false;
        for (int i = 0; i < PQntuples(result_user); i++) {
            if (values[4] == PQgetvalue(result_user, i, 0)) {
                f = true;
                break;
            }
        }
        PQclear(result_user);
        if (!f) 
            throw runtime_error("The foreign key does not exist in the parent table \"User\"");
        PGresult* result = GetTable("Book");
        ostringstream oss;
        for (int i = 1; i < PQnfields(result); i++) {
            oss << "\"" << PQfname(result, i) << "\"='" << values[i - 1] << "'";
            if (i < PQnfields(result) - 1)
                oss << ", ";
        }
        string query = "UPDATE \"Book\" SET " + oss.str() + " WHERE " + PQfname(result, 0) + "=" + to_string(idx);
        PQclear(result);
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }
  
    void UpdateRecord_listing(vector<string> values, int idx) {
        PGresult* result_book = PQexec(connection, "SELECT \"book_id\" FROM \"Book\"");
        bool f = false;
        for (int i = 0; i < PQntuples(result_book); i++) {
            if (values[2] == PQgetvalue(result_book, i, 0)) {
                f = true;
                break;
            }
        }
        PQclear(result_book);
        if (!f) 
            throw runtime_error("The foreign key does not exist in the parent table \"Book\"");
        PGresult* result = GetTable("Listing");
        ostringstream oss;
        for (int i = 1; i < PQnfields(result); i++) {
            oss << "\"" << PQfname(result, i) << "\"='" << values[i - 1] << "'";
            if (i < PQnfields(result) - 1)
                oss << ", ";
        }
        string query = "UPDATE \"Listing\" SET " + oss.str() + " WHERE " + PQfname(result, 0) + "=" + to_string(idx);
        PQclear(result);
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void UpdateRecord_request(vector<string> values, int idx) {
        PGresult* result_user = PQexec(connection, "SELECT \"user_id\" FROM \"User\"");
        bool f_user = false;
        for (int i = 0; i < PQntuples(result_user); i++) {
            if (values[1] == PQgetvalue(result_user, i, 0)) {
                f_user = true;
                break;
            }
        }
        PQclear(result_user);
        if (!f_user) 
            throw runtime_error("The foreign key does not exist in the parent table \"User\"");
        PGresult* result_listing = PQexec(connection, "SELECT \"listing_id\" FROM \"Listing\"");
        bool f_listing = false;
        for (int i = 0; i < PQntuples(result_listing); i++) {
            if (values[2] == PQgetvalue(result_listing, i, 0)) {
                f_listing = true;
                break;
            }
        }
        PQclear(result_listing);
        if (!f_listing)
            throw runtime_error("The foreign key does not exist in the parent table \"Listing\"");
        PGresult* result = GetTable("Request");
        ostringstream oss;
        for (int i = 1; i < PQnfields(result); i++) {
            oss << "\"" << PQfname(result, i) << "\"='" << values[i - 1] << "'";
            if (i < PQnfields(result) - 1)
                oss << ", ";
        }
        string query = "UPDATE \"Request\" SET " + oss.str() + " WHERE " + PQfname(result, 0) + "=" + to_string(idx);
        PQclear(result);
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void DeleteRecord_user(int idx) {
        PGresult* result_book = PQexec(connection, "SELECT \"user_id\" from \"Book\"");
        for (int i = 0; i < PQntuples(result_book); i++) {
            if (PQgetvalue(result_book, i, 0) == to_string(idx)) 
                throw runtime_error("The table is linked to a child table \"Book\"");
        }
        PGresult* result_request = PQexec(connection, "SELECT \"user_id\" from \"Request\"");
        for (int i = 0; i < PQntuples(result_request); i++) {
            if (PQgetvalue(result_request, i, 0) == to_string(idx)) 
                throw runtime_error("The table is linked to a child table \"Request\"");
        }
        PQclear(result_book);
        PQclear(result_request);
        string query = "DELETE FROM \"User\" WHERE \"user_id\"='" + to_string(idx) + "'";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void DeleteRecord_book(int idx) {
        PGresult* result_listing = PQexec(connection, "SELECT \"book_id\" from \"Listing\"");
        for (int i = 0; i < PQntuples(result_listing); i++) {
            if (PQgetvalue(result_listing, i, 0) == to_string(idx)) 
                throw runtime_error("The table is linked to a child table \"Listing\"");
        }
        PQclear(result_listing);
        string query = "DELETE FROM \"Book\" WHERE \"book_id\"='" + to_string(idx) + "'";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void DeleteRecord_listing(int idx) {
        PGresult* result_request = PQexec(connection, "SELECT \"listing_id\" from \"Request\"");
        for (int i = 0; i < PQntuples(result_request); i++) {
            if (PQgetvalue(result_request, i, 0) == to_string(idx))
                throw runtime_error("The table is linked to a child table \"Request\"");
        }
        PQclear(result_request);
        string query = "DELETE FROM \"Listing\" WHERE \"listing_id\"='" + to_string(idx) + "'";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void DeleteRecord_request(int idx) {
        string query = "DELETE FROM \"Request\" WHERE \"request_id\"='" + to_string(idx) + "'";
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void PopulateData_user(int limit) {
        string query = "INSERT INTO \"User\" (user_id, first_name, last_name, email)\n"
            "SELECT\n"
            "ROW_NUMBER() OVER () AS user_id,\n"
            "first_name,\n"
            "last_name,\n"
            "Lower(first_name || '.' || last_name) || '@example.com' as email\n"
            "FROM (SELECT unnest(array['Mykola', 'Andrii', 'Oleh', 'Oleksandr', 'Ihor', 'Vasyl', 'Yaroslav', 'Dmytro', 'Bohdan', 'Petro', 'Volodymyr', 'Serhii', 'Yurii', 'Roman', 'Artem', 'Pavlo', 'Taras', 'Stepan', 'Vitalii', 'Oleksii', 'Vadym', 'Hryhorii', 'Leonid', 'Anatolii', 'Kyrylo', 'Ivan', 'Oksana', 'Natalia', 'Liudmyla', 'Halyna']) AS first_name) AS first_name_table,\n"
            "(SELECT unnest (array['Shevchenko', 'Kovalchuk', 'Bondarenko', 'Tkachenko', 'Kravchenko', 'Melnychenko', 'Lysenko', 'Tymoshenko', 'Chernenko', 'Hrytsenko', 'Ponomarenko', 'Martynenko', 'Levchenko', 'Yaremchenko', 'Vasylenko', 'Kozachenko', 'Polishchuk', 'Kulychenko', 'Savchenko', 'Zavhorodko', 'Horodko', 'Ivanenko', 'Karpenko', 'Novychenko', 'Radchenko', 'Shapovalenko', 'Verbychenko', 'Dovzhenko', 'Kalchenko', 'Kostenko']) AS last_name) AS last_name_table\n"
            "ORDER BY RANDOM()\n"
            "LIMIT " + to_string(limit);
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) 
            throw runtime_error(PQerrorMessage(connection));
    }

    void PopulateData_book(int limit) {
        string query = "INSERT INTO \"Book\" (book_id, title, author, genre, condition, user_id)\n"
            "SELECT\n"
            "ROW_NUMBER() OVER () AS book_id,\n"
            "title,\n"
            "author,\n"
            "genre,\n"
            "condition,\n"
            "user_id\n"
            "FROM (SELECT unnest(array['Vita et Mysterium', 'Sparks of Eternity', 'Shadows in the Night', 'The Silent Path', 'Whispers of the Forest', 'Beneath the Moonlight', 'Echoes of the Past', 'Wings of Freedom', 'Through the Storm', 'The Lost Symphony', 'Rays of Hope', 'Tales of the Forgotten', 'Into the Horizon', 'The Last Journey', 'Beyond the Veil', 'A Dance with Time', 'Reflections of the Soul', 'The Secret Garden', 'Chronicles of a Dreamer', 'The Hearts Song']) AS title),\n"
            "(SELECT unnest(array['Shakespeare', 'Hemingway', 'Austen', 'Tolstoy', 'Dickens', 'Hemingway', 'Fitzgerald', 'Poe', 'Kafka', 'Orwell', 'Dostoevsky', 'Huxley', 'Hawthorne', 'Melville', 'Mann', 'Joyce', 'Tolkien', 'Woolf', 'Hemingway', 'Chekhov']) AS author),\n"
            "(SELECT unnest(array['Fiction', 'Mystery', 'Fantasy', 'Romance', 'Science Fiction', 'Thriller', 'Horror', 'Historical Fiction', 'Adventure', 'Drama', 'Poetry', 'Dystopian', 'Biography', 'Memoir', 'Non-fiction', 'Psychological Thriller', 'Young Adult', 'Crime', 'Humor', 'Magical Realism']) AS genre),\n"
            "(SELECT unnest(array['Brand New', 'Like New', 'Very Good', 'Good', 'Acceptable', 'Worn', 'Used', 'Damaged', 'Tattered', 'Well-Read']) AS condition),\n"
            "(SELECT user_id FROM \"User\" AS user_id)\n"
            "ORDER BY RANDOM()\n"
            "LIMIT " + to_string(limit);;
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK)
            throw runtime_error(PQerrorMessage(connection));
    }

    void PopulateData_listing(int limit) {
        string query = "INSERT INTO \"Listing\" (listing_id, created_date, book_id, status)\n"
            "SELECT\n"
            "ROW_NUMBER() OVER () AS listing_id,\n"
            "(date '2018-01-10' + (random() * (date '2023-05-20' - date '2018-01-10'))::int) AS created_date,\n"
            "book_id,\n"
            "status\n"
            "FROM "
            "(SELECT book_id FROM \"Book\" AS book_id)\n"
            "CROSS JOIN (SELECT unnest(array['Pending', 'Approved', 'Rejected', 'In Progress', 'Completed', 'Cancelled', 'Awaiting Confirmation', 'On Hold', 'Transferred', 'Returned']) AS status)\n"
            "ORDER BY RANDOM()\n"
            "LIMIT " + to_string(limit);;
        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) {
            throw runtime_error(PQerrorMessage(connection));
        }
    }

    void PopulateData_request(int limit) {
        string query = "INSERT INTO \"Request\" (request_id, user_id, listing_id)\n"
            "SELECT\n"
            "ROW_NUMBER() OVER () AS request_id,\n"
            "user_id,\n"
            "listing_id\n"
            "FROM (SELECT user_id FROM \"User\" AS user_id)\n"
            "CROSS JOIN (SELECT listing_id FROM \"Listing\" AS listing_id)\n"
            "ORDER BY RANDOM()\n"
            "LIMIT " + to_string(limit);

        if (PQresultStatus(PQexec(connection, query.c_str())) != PGRES_TUPLES_OK) {
            throw runtime_error(PQerrorMessage(connection));
        }
    }

    PGresult* QueryStatus(string status) {
        string query = "SELECT u.first_name AS first_name, u.last_name AS last_name, b.title AS title, l.status AS status\n"
            "FROM \"Request\" r\n"
            "JOIN \"User\" u ON r.user_id = u.user_id\n"
            "JOIN \"Listing\" l ON r.listing_id = l.listing_id\n"
            "JOIN \"Book\" b ON l.book_id = b.book_id\n"
            "WHERE l.status LIKE $1\n"
            "ORDER BY u.first_name, u.last_name, b.title, l.status;";
        status = "%" + status + "%";
        const char* paramValues[1] = { status.c_str() };
        auto start = chrono::high_resolution_clock::now();
        PGresult* result = PQexecParams(connection, query.c_str(), 1, nullptr, paramValues, nullptr, nullptr, 0);
        auto end = chrono::high_resolution_clock::now();
        time = chrono::duration_cast<chrono::milliseconds>(end - start);
        if (PQresultStatus(result) != PGRES_TUPLES_OK)
        {
            PQclear(result);
            throw runtime_error(PQerrorMessage(connection));
        }
        return result;
    }

    PGresult* QueryDate(string date) {
        string query = "SELECT u.first_name AS first_name, u.last_name AS last_name, b.title AS title, l.created_date AS created_date\n"
            "FROM \"Request\" r\n"
            "JOIN \"User\" u ON r.user_id = u.user_id\n"
            "JOIN \"Listing\" l ON r.listing_id = l.listing_id\n"
            "JOIN \"Book\" b ON l.book_id = b.book_id\n"
            "WHERE l.created_date > $1\n"
            "ORDER BY u.first_name, u.last_name, b.title, l.created_date;";
        const char* paramValues[1] = { date.c_str() };
        auto start = chrono::high_resolution_clock::now();
        PGresult* result = PQexecParams(connection, query.c_str(), 1, nullptr, paramValues, nullptr, nullptr, 0);
        auto end = chrono::high_resolution_clock::now();
        time = chrono::duration_cast<chrono::milliseconds>(end - start);
        if (PQresultStatus(result) != PGRES_TUPLES_OK)
        {
            PQclear(result);
            throw runtime_error(PQerrorMessage(connection));
        }
        return result;
    }

    PGresult* QueryID(string date) {
        string query = "SELECT u.user_id AS user_id, l.listing_id AS listing_id, l.created_date AS created_date, l.status AS status\n"
            "FROM \"Request\" r\n"
            "JOIN \"User\" u ON r.user_id = u.user_id\n"
            "JOIN \"Listing\" l ON r.listing_id = l.listing_id\n"
            "WHERE u.user_id = $1\n"
            "ORDER BY u.user_id, l.listing_id, l.created_date, l.status;";
        const char* paramValues[1] = { date.c_str() };
        auto start = chrono::high_resolution_clock::now();
        PGresult* result = PQexecParams(connection, query.c_str(), 1, nullptr, paramValues, nullptr, nullptr, 0);
        auto end = chrono::high_resolution_clock::now();
        time = chrono::duration_cast<chrono::milliseconds>(end - start);
        if (PQresultStatus(result) != PGRES_TUPLES_OK)
        {
            PQclear(result);
            throw runtime_error(PQerrorMessage(connection));
        }
        return result;
    }

    chrono::milliseconds GetTime() const {
        return time;
    }

private:
    PGconn* connection;
    chrono::milliseconds time;
};