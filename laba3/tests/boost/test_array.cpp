#include <boost/test/unit_test.hpp>
#include "capture.h"
#include "arrayClass.h"

// конструктор
BOOST_AUTO_TEST_CASE(ArrayTest_NewArrayIsEmpty)
{
    Array arr;
    BOOST_CHECK_EQUAL(arr.lenght(), 0);
}

// pushBack
BOOST_AUTO_TEST_CASE(ArrayTest_PushBackAddsToEnd)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");

    BOOST_CHECK_EQUAL(arr.lenght(), 2);
    BOOST_CHECK_EQUAL(arr.getAt(0), "a");
    BOOST_CHECK_EQUAL(arr.getAt(1), "b");
}

// resizeArray (через pushBack)
BOOST_AUTO_TEST_CASE(ArrayTest_GrowsBeyondStartCapacity)
{
    Array arr;
    for (int i = 0; i < 10; i++)     // вместимость 4, должно расшириться
    {
        arr.pushBack(to_string(i));
    }
    BOOST_CHECK_EQUAL(arr.lenght(), 10);
    BOOST_CHECK_EQUAL(arr.getAt(9), "9");
}

// insertAt
BOOST_AUTO_TEST_CASE(ArrayTest_InsertInMiddle)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("c");
    arr.insertAt("b", 1);

    BOOST_CHECK_EQUAL(arr.getAt(0), "a");
    BOOST_CHECK_EQUAL(arr.getAt(1), "b");
    BOOST_CHECK_EQUAL(arr.getAt(2), "c");
}

BOOST_AUTO_TEST_CASE(ArrayTest_InsertWhenFull)
{
    Array arr;
    for (int i = 0; i < 4; i++)      // заполнили всю вместимость
    {
        arr.pushBack("x");
    }
    arr.insertAt("y", 0);            // insertAt должен сам расширить массив
    BOOST_CHECK_EQUAL(arr.lenght(), 5);
    BOOST_CHECK_EQUAL(arr.getAt(0), "y");
}

BOOST_AUTO_TEST_CASE(ArrayTest_InsertWrongIndex)
{
    Array arr;
    arr.pushBack("a");
    startCapture();
    arr.insertAt("x", 5);            // такого индекса нет
    stopCapture();
    BOOST_CHECK_EQUAL(arr.lenght(), 1);      // ничего не добавилось
}

// removeAt
BOOST_AUTO_TEST_CASE(ArrayTest_RemoveShiftsElements)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");
    arr.pushBack("c");
    arr.removeAt(0);

    BOOST_CHECK_EQUAL(arr.lenght(), 2);
    BOOST_CHECK_EQUAL(arr.getAt(0), "b");
}

BOOST_AUTO_TEST_CASE(ArrayTest_RemoveWrongIndex)
{
    Array arr;
    arr.pushBack("a");
    startCapture();
    arr.removeAt(3);                 // такого индекса нет
    stopCapture();
    BOOST_CHECK_EQUAL(arr.lenght(), 1);      // ничего не удалилось
}

// setAt
BOOST_AUTO_TEST_CASE(ArrayTest_SetAtChangesValue)
{
    Array arr;
    arr.pushBack("a");
    arr.setAt("b", 0);
    BOOST_CHECK_EQUAL(arr.getAt(0), "b");
}

BOOST_AUTO_TEST_CASE(ArrayTest_SetAtWrongIndex)
{
    Array arr;
    arr.pushBack("a");
    startCapture();
    arr.setAt("b", 3);               // такого индекса нет
    stopCapture();
    BOOST_CHECK_EQUAL(arr.getAt(0), "a");    // старое значение осталось
}

// getAt
BOOST_AUTO_TEST_CASE(ArrayTest_GetAtWrongIndex)
{
    Array arr;
    startCapture();
    string got = arr.getAt(0);
    stopCapture();
    BOOST_CHECK_EQUAL(got, "");              // при ошибке возвращает пустую строку
}

// printArray
BOOST_AUTO_TEST_CASE(ArrayTest_PrintShowsElements)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");

    startCapture();
    arr.printArray();
    string out = stopCapture();

    BOOST_CHECK_EQUAL(out, "a b (size=2, capacity=4)\n");
}

// handleArray
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleArray(command, db, "arr", ss);
}

BOOST_AUTO_TEST_CASE(ArrayHandlerTest_AllCommands)
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

    BOOST_CHECK_EQUAL(db["arr"]["type"].get<string>(), "array");
    BOOST_CHECK_EQUAL(db["arr"]["data"].size(), 2u);
    BOOST_CHECK_EQUAL(db["arr"]["data"][0].get<string>(), "z");
    BOOST_CHECK_EQUAL(db["arr"]["data"][1].get<string>(), "b");
}

BOOST_AUTO_TEST_CASE(ArrayHandlerTest_WrongTypeIsRejected)
{
    json db = json::object();
    db["arr"]["type"] = "stack";             // под этим именем уже стек
    db["arr"]["data"] = json::array();

    startCapture();
    run(db, Command::MPUSH, "a");
    string out = stopCapture();

    BOOST_CHECK_EQUAL(db["arr"]["type"].get<string>(), "stack");   // тип не поменялся
    BOOST_CHECK_NE(out.find("это не array"), string::npos);     // и вывелось сообщение
}
