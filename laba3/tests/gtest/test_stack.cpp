#include <gtest/gtest.h>
#include "stackClass.h"

// конструктор
TEST(StackTest, NewStackIsEmpty)
{
    Stack st;
    EXPECT_TRUE(st.isEmptyStack());
}

// push и top
TEST(StackTest, TopIsLastPushed)
{
    Stack st;
    st.push("1");
    st.push("2");

    EXPECT_FALSE(st.isEmptyStack());
    EXPECT_EQ(st.top(), "2");
}

// pop
TEST(StackTest, PopRemovesTop)
{
    Stack st;
    st.push("1");
    st.push("2");
    st.pop();

    EXPECT_EQ(st.top(), "1");
}

TEST(StackTest, PopAndTopOnEmpty)
{
    Stack st;
    st.pop();                        // на пустом стеке ничего не ломается
    EXPECT_TRUE(st.isEmptyStack());
    EXPECT_EQ(st.top(), "");
}

// print
TEST(StackTest, PrintFromTop)
{
    Stack st;
    st.push("1");
    st.push("2");

    testing::internal::CaptureStdout();
    st.print();
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(out, "2 -> 1 -> nullptr\n");
}

// handleStack
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleStack(command, db, "st", ss);
}

TEST(StackHandlerTest, AllCommands)
{
    json db = json::object();

    testing::internal::CaptureStdout();
    run(db, Command::SPUSH, "1");
    run(db, Command::SPUSH, "2");
    run(db, Command::SPUSH, "3");
    run(db, Command::SPOP, "");            // снимает 3
    run(db, Command::SPRINT, "");
    run(db, Command::PRINT, "");           // чужая команда - ничего не делает
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("-> 3"), string::npos);
    EXPECT_EQ(db["st"]["type"].get<string>(), "stack");
    EXPECT_EQ(db["st"]["data"].size(), 2u);
    EXPECT_EQ(db["st"]["data"][0].get<string>(), "1");   // в json снизу вверх
    EXPECT_EQ(db["st"]["data"][1].get<string>(), "2");
}

TEST(StackHandlerTest, PopEmpty)
{
    json db = json::object();

    testing::internal::CaptureStdout();
    run(db, Command::SPOP, "");
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("стек пуст"), string::npos);
}

TEST(StackHandlerTest, WrongTypeIsRejected)
{
    json db = json::object();
    db["st"]["type"] = "queue";
    db["st"]["data"] = json::array();

    testing::internal::CaptureStdout();
    run(db, Command::SPUSH, "1");
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(db["st"]["type"].get<string>(), "queue");
}
