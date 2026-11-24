#include "Commands.h"
#include <string>

Command toCommand(const std::string& text)
{
    if (text == "SADD") return Command::SADD;
    if (text == "SREM") return Command::SREM;
    if (text == "SISMEMBER") return Command::SISMEMBER;
    if (text == "SPUSH") return Command::SPUSH;
    if (text == "SPOP") return Command::SPOP;
    if (text == "QPUSH") return Command::QPUSH;
    if (text == "QPOP") return Command::QPOP;
    if (text == "HSET") return Command::HSET;
    if (text == "HDEL") return Command::HDEL;
    if (text == "HGET") return Command::HGET;
    return Command::UNKNOWN;
}