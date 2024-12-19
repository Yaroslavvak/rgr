#pragma once
#include "model.h"
#include "view.h"
#include <chrono>
using namespace std;

class Manager {
public:
    Manager() {
        Database dbModel;
        Display dbView;
        dbView.PrintMessage("Successfully connected to the database.");
    }

    void Run() {
        while (true) {
            int choice = dbView.ShowMainMenu();
            if (choice == 1)
                ShowTableData();
            else if (choice == 2)
                InsertNewRecord();
            else if (choice == 3)
                ModifyRecord();
            else if (choice == 4)
                RemoveRecord();
            else if (choice == 5)
                PopulateDataRandomly();
            else if (choice == 6)
                ExecuteQuery();
            else if (choice == 7)
                break;
            else
                dbView.PrintMessage("Invalid option. Please try again.\n");
        }
    }

private:
    Database dbModel;
    Display dbView;

    vector<string> ParseInputString(const string& input) {
        vector<string> parsedValues;
        string value;
        stringstream ss(input);
        while (getline(ss, value, ',')) {
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            parsedValues.push_back(value);
        }
        return parsedValues;
    }

    void ShowTableData() {
        string tableName = dbView.PromptTableName();
        auto result = dbModel.GetTable(tableName);
        if (result == nullptr)
            dbView.PrintMessage("Table does not exist.");
        else
            dbView.PrintTable(result);
    }

    void InsertNewRecord() {
        string tableName = dbView.PromptTableName();
        string inputValues = dbView.PromptValues();
        vector<string> values = ParseInputString(inputValues);
        try {
            if (tableName == "User")
                dbModel.AddRecord_user(values);
            else if (tableName == "Book")
                dbModel.AddRecord_book(values);
            else if (tableName == "Listing")
                dbModel.AddRecord_listing(values);
            else if (tableName == "Request")
                dbModel.AddRecord_request(values);
            else
                dbView.PrintMessage("Table does not exist.");
            dbView.PrintMessage("Record was inserted successfully");
        }
        catch (const exception& e) {
            dbView.PrintMessage(e.what());
        }
    }

    void ModifyRecord() {
        string tableName = dbView.PromptTableName();
        int recordID = dbView.PromptID();
        string inputValues = dbView.PromptValues();
        vector<string> values = ParseInputString(inputValues);
        try {
            if (tableName == "User")
                dbModel.UpdateRecord_user(values, recordID);
            else if (tableName == "Book")
                dbModel.UpdateRecord_book(values, recordID);
            else if (tableName == "Listing")
                dbModel.UpdateRecord_listing(values, recordID);
            else if (tableName == "Request")
                dbModel.UpdateRecord_request(values, recordID);
            else
                dbView.PrintMessage("Table does not exist.");
            dbView.PrintMessage("Record was modified successfully");
        }
        catch (const exception& e) {
            dbView.PrintMessage(e.what());
        }
    }

    void RemoveRecord() {
        string tableName = dbView.PromptTableName();
        int recordID = dbView.PromptID();
        try {
            if (tableName == "User")
                dbModel.DeleteRecord_user(recordID);
            else if (tableName == "Book")
                dbModel.DeleteRecord_book(recordID);
            else if (tableName == "Listing")
                dbModel.DeleteRecord_listing(recordID);
            else if (tableName == "Request")
                dbModel.DeleteRecord_request(recordID);
            else
                dbView.PrintMessage("Table does not exist.");
            dbView.PrintMessage("Record was removed successfully");
        }
        catch (const exception& e) {
            dbView.PrintMessage(e.what());
        }
    }
    
    void PopulateDataRandomly() {
        string tableName = dbView.PromptTableName();
        int count = dbView.PromptCount();
        try {
            if (tableName == "User")
                dbModel.PopulateData_user(count);
            else if (tableName == "Book")
                dbModel.PopulateData_book(count);
            else if (tableName == "Listing")
                dbModel.PopulateData_listing(count);
            else if (tableName == "Request")
                dbModel.PopulateData_request(count);
            else
                dbView.PrintMessage("Table does not exist.");
            dbView.PrintMessage("Data was populated successfully");
        }
        catch (const exception& e) {
            dbView.PrintMessage(e.what());
        }
    }

    void ExecuteQuery() {
        int queryChoice = dbView.ShowQueryMenu();
        string parameter = dbView.PromptCondition();
        try {
            if (queryChoice == 1) {
                PGresult* result = dbModel.QueryStatus(parameter);
                dbView.PrintTable(result);
            }
            else if (queryChoice == 2) {
                PGresult* result = dbModel.QueryDate(parameter);
                dbView.PrintTable(result);
            }
            else if (queryChoice == 3) {
                PGresult* result = dbModel.QueryID(parameter);
                dbView.PrintTable(result);
            }
            else {
                dbView.PrintMessage("Invalid query choice.\n");
            }
            long long duration = dbModel.GetTime().count();
            dbView.PrintMessage("Query duration: " + to_string(duration) + " ms");
        }
        catch (const exception& e) {
            dbView.PrintMessage(e.what());
        }
    }
};
