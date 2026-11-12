#include <boost/test/unit_test.hpp>
#include "capture.h"
#include "treeClass.h"

// дерево для тестов:
//       5
//      / \
//     3   8
//    /
//   1
static void fill(Tree& tr)
{
    tr.insert(5);
    tr.insert(3);
    tr.insert(8);
    tr.insert(1);
}

// конструктор
BOOST_AUTO_TEST_CASE(TreeTest_NewTreeIsEmpty)
{
    Tree tr;
    BOOST_CHECK(tr.getRoot() == nullptr);
}

// insert и search
BOOST_AUTO_TEST_CASE(TreeTest_InsertAndSearch)
{
    Tree tr;
    fill(tr);

    BOOST_CHECK_EQUAL(tr.getRoot()->key, 5);
    BOOST_CHECK(tr.search(1) != nullptr);
    BOOST_CHECK(tr.search(8) != nullptr);
    BOOST_CHECK(tr.search(7) == nullptr);    // такого нет
}

BOOST_AUTO_TEST_CASE(TreeTest_InsertGoesLeftAndRight)
{
    Tree tr;
    fill(tr);

    BOOST_CHECK_EQUAL(tr.getRoot()->left->key, 3);    // меньше - влево
    BOOST_CHECK_EQUAL(tr.getRoot()->right->key, 8);   // больше - вправо
}

// обходы
BOOST_AUTO_TEST_CASE(TreeTest_Traversals)
{
    Tree tr;
    fill(tr);

    startCapture();
    tr.printPreorder();
    BOOST_CHECK_EQUAL(stopCapture(), "5 3 1 8 ");

    startCapture();
    tr.printInorder();
    BOOST_CHECK_EQUAL(stopCapture(), "1 3 5 8 ");

    startCapture();
    tr.printPostorder();
    BOOST_CHECK_EQUAL(stopCapture(), "1 3 8 5 ");

    startCapture();
    tr.bfs();
    BOOST_CHECK_EQUAL(stopCapture(), "5 3 8 1 ");
}

BOOST_AUTO_TEST_CASE(TreeTest_BfsOnEmpty)
{
    Tree tr;
    startCapture();
    tr.bfs();
    BOOST_CHECK_EQUAL(stopCapture(), "");
}

// isComplete
BOOST_AUTO_TEST_CASE(TreeTest_CompleteTree)
{
    Tree tr;
    BOOST_CHECK(tr.isComplete());        // пустое дерево считается полным

    fill(tr);                            // уровни заполнены слева направо
    BOOST_CHECK(tr.isComplete());
}

BOOST_AUTO_TEST_CASE(TreeTest_NotCompleteTree)
{
    Tree tr;
    tr.insert(5);
    tr.insert(8);                        // только правый ребенок, слева дырка
    tr.insert(9);
    BOOST_CHECK(!tr.isComplete());
}

// handleTree
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleTree(command, db, "t", ss);
}

BOOST_AUTO_TEST_CASE(TreeHandlerTest_AllCommands)
{
    json db = json::object();

    startCapture();
    run(db, Command::TINSERT, "5");
    run(db, Command::TINSERT, "3");
    run(db, Command::TINSERT, "8");
    run(db, Command::TFIND, "3");
    run(db, Command::TFIND, "7");
    run(db, Command::TCOMPLETE, "");
    run(db, Command::TPRINT, "");
    run(db, Command::PRINT, "");             // чужая команда
    string out = stopCapture();

    BOOST_CHECK_NE(out.find("-> TRUE"), string::npos);
    BOOST_CHECK_NE(out.find("-> FALSE"), string::npos);
    BOOST_CHECK_EQUAL(db["t"]["type"].get<string>(), "tree");
    BOOST_CHECK_EQUAL(db["t"]["data"].size(), 3u);
    BOOST_CHECK_EQUAL(db["t"]["data"][0].get<int>(), 5);    // сохранено в ширину
    BOOST_CHECK_EQUAL(db["t"]["data"][1].get<int>(), 3);
    BOOST_CHECK_EQUAL(db["t"]["data"][2].get<int>(), 8);
}

BOOST_AUTO_TEST_CASE(TreeHandlerTest_NotComplete)
{
    json db = json::object();

    startCapture();
    run(db, Command::TINSERT, "5");
    run(db, Command::TINSERT, "8");
    run(db, Command::TINSERT, "9");
    run(db, Command::TCOMPLETE, "");
    string out = stopCapture();

    BOOST_CHECK_NE(out.find("-> FALSE"), string::npos);
}

BOOST_AUTO_TEST_CASE(TreeHandlerTest_WrongTypeIsRejected)
{
    json db = json::object();
    db["t"]["type"] = "array";
    db["t"]["data"] = json::array();

    startCapture();
    run(db, Command::TINSERT, "5");
    stopCapture();

    BOOST_CHECK_EQUAL(db["t"]["type"].get<string>(), "array");
}
