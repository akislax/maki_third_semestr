#pragma once
#include <string>
enum class Command
{
    SADD, SREM, SISMEMBER, SPUSH, SPOP, QPUSH, QPOP, HSET, HDEL, HGET, UNKNOWN

};
Command toCommand(const std::string& text);