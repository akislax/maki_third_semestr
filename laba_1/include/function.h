#pragma once
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include "commands.h"
using namespace std;
using json = nlohmann::json;

void handleArray(Command command, json& db, const string& name, stringstream& ss);
void handleFlist(Command command, json& db, const string& name, stringstream& ss);
void handleDlist(Command command, json& db, const string& name, stringstream& ss);
void handleStack(Command command, json& db, const string& name, stringstream& ss);
void handleQueue(Command command, json& db, const string& name, stringstream& ss);
void handleTree(Command command, json& db, const string& name, stringstream& ss);
void handlePrint(json& db, const string& name, stringstream& ss);