#include "commands.h"

Command toCommand(const string& text)
{
    if (text == "MPUSH") return Command::MPUSH;
    if (text == "MINSERT") return Command::MINSERT;
    if (text == "MGET") return Command::MGET;
    if (text == "MSET") return Command::MSET;
    if (text == "MDEL") return Command::MDEL;
    if (text == "MLEN") return Command::MLEN;
    if (text == "MPRINT") return Command::MPRINT;

    if (text == "FPUSHH") return Command::FPUSHH;
    if (text == "FPUSHT") return Command::FPUSHT;
    if (text == "FPUSHA") return Command::FPUSHA;
    if (text == "FPUSHB") return Command::FPUSHB;
    if (text == "FDELH") return Command::FDELH;
    if (text == "FDELT") return Command::FDELT;
    if (text == "FDELA") return Command::FDELA;
    if (text == "FDELB") return Command::FDELB;
    if (text == "FDEL") return Command::FDEL;
    if (text == "FFIND") return Command::FFIND;
    if (text == "FPRINT") return Command::FPRINT;
    if (text == "FPRINTR") return Command::FPRINTR;

    if (text == "LPUSHH") return Command::LPUSHH;
    if (text == "LPUSHT") return Command::LPUSHT;
    if (text == "LPUSHA") return Command::LPUSHA;
    if (text == "LPUSHB") return Command::LPUSHB;
    if (text == "LDELH") return Command::LDELH;
    if (text == "LDELT") return Command::LDELT;
    if (text == "LDELA") return Command::LDELA;
    if (text == "LDELB") return Command::LDELB;
    if (text == "LDEL") return Command::LDEL;
    if (text == "LFIND") return Command::LFIND;
    if (text == "LPRINT") return Command::LPRINT;
    if (text == "LPRINTR") return Command::LPRINTR;

    if (text == "SPUSH") return Command::SPUSH;
    if (text == "SPOP") return Command::SPOP;
    if (text == "SPRINT") return Command::SPRINT;

    if (text == "QPUSH") return Command::QPUSH;
    if (text == "QPOP") return Command::QPOP;
    if (text == "QPRINT") return Command::QPRINT;

    if (text == "TINSERT") return Command::TINSERT;
    if (text == "TFIND") return Command::TFIND;
    if (text == "TCOMPLETE") return Command::TCOMPLETE;
    if (text == "TPRINT") return Command::TPRINT;

    if (text == "PRINT") return Command::PRINT;
    return Command::UNKNOWN;
}