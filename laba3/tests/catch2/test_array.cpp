#include <catch2/catch_test_macros.hpp>
#include "capture.h"
#include "arrayClass.h"

// конструктор
TEST_CASE("ArrayTest.NewArrayIsEmpty")
{
    Array arr;
    CHECK(arr.lenght() == 0);
}

// pushBack
TEST_CASE("ArrayTest.PushBackAddsToEnd")
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");

    CHECK(arr.lenght() == 2);
    CHECK(arr.getAt(0) == "a");
    CHECK(arr.getAt(1) == "b");
}

// resizeArray (через pushBack)
TEST_CASE("ArrayTest.GrowsBeyondStartCapacity")
{
    Array arr;
    for (int i = 0; i < 10; i++)     // вместимость 4, должно расшириться
    {
        arr.pushBack(to_string(i));
    }
    CHECK(arr.lenght() == 10);
    CHECK(arr.getAt(9) == "9");
}

// insertAt
TEST_CASE("ArrayTest.InsertInMiddle")
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("c");
    arr.insertAt("b", 1);

    CHECK(arr.getAt(0) == "a");
    CHECK(arr.getAt(1) == "b");
    CHECK(arr.getAt(2) == "c");
}

TEST_CASE("ArrayTest.InsertWhenFull")
{
    Array arr;
    for (int i = 0; i < 4; i++)      // заполнили всю вместимость
    {
        arr.pushBack("x");
    }
    arr.insertAt("y", 0);            // insertAt должен сам расширить массив
    CHECK(arr.lenght() == 5);
    CHECK(arr.getAt(0) == "y");
}

TEST_CASE("ArrayTest.InsertWrongIndex")
{
    Array arr;
    arr.pushBack("a");
    startCapture();
    arr.insertAt("x", 5);            // такого индекса нет
    stopCapture();
    CHECK(arr.lenght() == 1);      // ничего не добавилось
}

// removeAt
TEST_CASE("ArrayTest.RemoveShiftsElements")
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");
    arr.pushBack("c");
    arr.removeAt(0);

    CHECK(arr.lenght() == 2);
    CHECK(arr.getAt(0) == "b");
}

TEST_CASE("ArrayTest.RemoveWrongIndex")
{
    Array arr;
    arr.pushBack("a");
    startCapture();
    arr.removeAt(3);                 // такого индекса нет
    stopCapture();
    CHECK(arr.lenght() == 1);      // ничего не удалилось
}

// setAt
TEST_CASE("ArrayTest.SetAtChangesValue")
{
    Array arr;
    arr.pushBack("a");
    arr.setAt("b", 0);
    CHECK(arr.getAt(0) == "b");
}

TEST_CASE("ArrayTest.SetAtWrongIndex")
{
    Array arr;
    arr.pushBack("a");
    startCapture();
    arr.setAt("b", 3);               // такого индекса нет
    stopCapture();
    CHECK(arr.getAt(0) == "a");    // старое значение осталось
}

// getAt
TEST_CASE("ArrayTest.GetAtWrongIndex")
{
    Array arr;
    startCapture();
    string got = arr.getAt(0);
    stopCapture();
    CHECK(got == "");              // при ошибке возвращает пустую строку
}

// printArray
TEST_CASE("ArrayTest.PrintShowsElements")
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");

    startCapture();
    arr.printArray();
    string out = stopCapture();

    CHECK(out == "a b (size=2, capacity=4)\n");
}

// handleArray
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleArray(command, db, "arr", ss);
}

TEST_CASE("ArrayHandlerTest.AllCommands")
{
    json db = json::object();

    startCapture();
    run(db, Command::MPUSH, "a");          // [a]
    run(db, Command::MPUSH, "c");          // [a, c]
    run(db, Command::MINSERT, "1 b");      // [a, b, c]
    run(db, Command::MSET, "0 z");         // [z, b, c]
    run(db, Command::MDEL, "2");           // [z, b]
    run(db, Command::MGET, "1");
    run(db, Command::MLEN, "");
    run(db, Command::MPRINT, "");
    run(db, Command::PRINT, "");           // чужая команда - ничего не делает
    stopCapture();

    CHECK(db["arr"]["type"].get<string>() == "array");
    CHECK(db["arr"]["data"].size() == 2u);
    CHECK(db["arr"]["data"][0].get<string>() == "z");
    CHECK(db["arr"]["data"][1].get<string>() == "b");
}

TEST_CASE("ArrayHandlerTest.WrongTypeIsRejected")
{
    json db = json::object();
    db["arr"]["type"] = "stack";             // под этим именем уже стек
    db["arr"]["data"] = json::array();

    startCapture();
    run(db, Command::MPUSH, "a");
    string out = stopCapture();

    CHECK(db["arr"]["type"].get<string>() == "stack");   // тип не поменялся
    CHECK(out.find("это не array") != string::npos);     // и вывелось сообщение
}
