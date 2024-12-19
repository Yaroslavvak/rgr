#pragma once
#include <iostream>
#include <string>
#include <libpq-fe.h>
#include <iomanip>
using namespace std;

class Display {
public:
    void PrintMessage(const string& msg) {
        cout << msg << endl;
    }

    void PrintTable(PGresult* result) {
        cout << endl;
        int rowCount = PQntuples(result);
        int colCount = PQnfields(result);
        cout << left;
        for (int i = 0; i < colCount; i++)
            cout << setw(30) << PQfname(result, i);
        cout << endl;
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++)
                cout << setw(30) << PQgetvalue(result, i, j);
            cout << endl;
        }
        cout << endl;
    }

    int ShowMainMenu() {
        cout << "\nMenu Options:" << endl;
        cout << "1. View table\n2. Insert record\n3. Modify record\n4. Remove record\n5. Populate data\n6. Run queries\n7. Exit" << endl;
        int choice;
        cout << "Select an option: ";
        cin >> choice;
        return choice;
    }

    int ShowQueryMenu() {
        cout << "\nQuery Options:" << endl;
        cout << "1. Display the book title and username by listing status\n2. Display the book title and username whose listing is younger than the date\n3. Display listing data by user ID" << endl;
        int choice;
        cout << "Select a query: ";
        cin >> choice;
        return choice;
    }

    int PromptID() {
        int id;
        cout << "Enter the ID: ";
        cin >> id;
        return id;
    }

    int PromptCount() {
        int count;
        cout << "Enter the number of entries to generate: ";
        cin >> count;
        return count;
    }

    string PromptTableName() {
        string tableName;
        cout << "Provide the table name: ";
        cin >> tableName;
        return tableName;
    }

    string PromptValues() {
        string entries;
        cout << "Enter values (comma-separated): ";
        cin.ignore();
        getline(cin, entries);
        return entries;
    }

    string PromptCondition() {
        string filter;
        cout << "Specify the condition for the query: ";
        cin >> filter;
        return filter;
    }
};
