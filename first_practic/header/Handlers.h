#pragma once
#include "Commands.h"
#include <sstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void handleSet(Command command, const std::string& name, std::stringstream& ss, json& db);
void handleStack(Command command, const std::string& name, std::stringstream& ss, json& db);
void handleQueue(Command command, const std::string& name, std::stringstream& ss, json& db);
void handleHT(Command command, const std::string& name, std::stringstream& ss, json& db);
