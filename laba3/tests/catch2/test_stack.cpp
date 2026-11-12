#include <catch2/catch_test_macros.hpp>
#include "capture.h"
#include "stackClass.h"

// конструктор
TEST_CASE("StackTest.NewStackIsEmpty")
{
    Stack st;
    CHECK(st.isEmptyStack());
}

// push и top
TEST_CASE("StackTest.TopIsLastPushed")
{
    Stack st;
    st.push("1");
    st.push("2");

    CHECK_FALSE(st.isEmptyStack());
    CHECK(st.top() == "2");
}

// pop
TEST_CASE("StackTest.PopRemovesTop")
{
    Stack st;
    st.push("1");
    st.push("2");
    st.pop();

    CHECK(st.top() == "1");
}

TEST_CASE("StackTest.PopAndTopOnEmpty")
{
    Stack st;
    st.pop();                        // на пустом стеке ничего не ломается
    CHECK(st.isEmptyStack());
    CHECK(st.top() == "");
}

// print
TEST_CASE("StackTest.PrintFromTop")
{
    Stack st;
    st.push("1");
    st.push("2");

    startCapture();
    st.print();
    string out = stopCapture();

    CHECK(out == "2 -> 1 -> nullptr\n");
}

// handleStack
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleStack(command, db, "st", ss);
}

TEST_CASE("StackHandlerTest.AllCommands")
{
    json db = json::object();

    startCapture();
    run(db, Command::SPUSH, "1");
    run(db, Command::SPUSH, "2");
    run(db, Command::SPUSH, "3");
    run(db, Command::SPOP, "");            // снимает 3
    run(db, Command::SPRINT, "");
    run(db, Command::PRINT, "");           // чужая команда - ничего не делает
    string out = stopCapture();

    CHECK(out.find("-> 3") != string::npos);
    CHECK(db["st"]["type"].get<string>() == "stack");
    CHECK(db["st"]["data"].size() == 2u);
    CHECK(db["st"]["data"][0].get<string>() == "1");   // в json снизу вверх
    CHECK(db["st"]["data"][1].get<string>() == "2");
}

TEST_CASE("StackHandlerTest.PopEmpty")
{
    json db = json::object();

    startCapture();
    run(db, Command::SPOP, "");
    string out = stopCapture();

    CHECK(out.find("стек пуст") != string::npos);
}

TEST_CASE("StackHandlerTest.WrongTypeIsRejected")
{
    json db = json::object();
    db["st"]["type"] = "queue";
    db["st"]["data"] = json::array();

    startCapture();
    run(db, Command::SPUSH, "1");
    stopCapture();

    CHECK(db["st"]["type"].get<string>() == "queue");
}
