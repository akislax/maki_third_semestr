#include <boost/test/unit_test.hpp>
#include "capture.h"
#include "hashTable.h"

// ===== таблица с цепочками =====
// 5, 105 и 205 попадают в одну ячейку 5 (хеш = сумма свертки % 100),
// поэтому на них удобно проверять цепочки

BOOST_AUTO_TEST_CASE(HashTableTest_AddAndSearch)
{
    hashTable ht;
    ht.add(5);
    ht.add(123456);

    BOOST_CHECK(ht.search(5));
    BOOST_CHECK(ht.search(123456));
    BOOST_CHECK(!ht.search(7));
}

BOOST_AUTO_TEST_CASE(HashTableTest_DuplicateIsIgnored)
{
    hashTable ht;
    ht.add(5);
    ht.add(5);
    BOOST_CHECK_EQUAL(ht.getAll().size(), 1u);
}

BOOST_AUTO_TEST_CASE(HashTableTest_CollisionsGoToOneChain)
{
    hashTable ht;
    ht.add(5);
    ht.add(105);
    ht.add(205);

    startCapture();
    ht.print();
    string out = stopCapture();

    BOOST_CHECK(out == "5: 205 -> 105 -> 5 -> nullptr\n");   // новые встают в начало цепочки
}

BOOST_AUTO_TEST_CASE(HashTableTest_RemoveFromChain)
{
    hashTable ht;
    ht.add(5);
    ht.add(105);
    ht.add(205);              // цепочка: 205 -> 105 -> 5

    ht.remove(105);           // из середины
    ht.remove(205);           // из начала
    ht.remove(999);           // такого нет

    BOOST_CHECK(!ht.search(105));
    BOOST_CHECK(!ht.search(205));
    BOOST_CHECK(ht.search(5));
}

BOOST_AUTO_TEST_CASE(HashTableTest_NegativeKey)
{
    hashTable ht;
    ht.add(-7);
    BOOST_CHECK(ht.search(-7));
}

// ===== кукушка =====
// 1 и 12 дают одинаковый hash1 (12 % 11 = 1), поэтому второй вытесняет
// первого во вторую таблицу

BOOST_AUTO_TEST_CASE(KukushkaTest_InsertAndSearch)
{
    Kukushka kt;
    kt.insert(42);
    BOOST_CHECK(kt.search(42));
    BOOST_CHECK(!kt.search(7));
}

BOOST_AUTO_TEST_CASE(KukushkaTest_DuplicateIsIgnored)
{
    Kukushka kt;
    kt.insert(42);
    kt.insert(42);
    BOOST_CHECK_EQUAL(kt.getAll().size(), 1u);
}

BOOST_AUTO_TEST_CASE(KukushkaTest_CollisionMovesToSecondTable)
{
    Kukushka kt;
    kt.insert(1);
    kt.insert(12);            // выталкивает 1 во вторую таблицу

    startCapture();
    kt.print();
    string out = stopCapture();

    BOOST_CHECK_EQUAL(out, "t1: [1]=12 \nt2: [1]=1 \n");
    BOOST_CHECK(kt.search(1));     // находится во второй таблице
    BOOST_CHECK(kt.search(12));    // находится в первой
}

BOOST_AUTO_TEST_CASE(KukushkaTest_RemoveFromBothTables)
{
    Kukushka kt;
    kt.insert(1);
    kt.insert(12);

    kt.remove(12);            // из первой таблицы
    kt.remove(1);             // из второй
    kt.remove(999);           // такого нет

    BOOST_CHECK(!kt.search(1));
    BOOST_CHECK(!kt.search(12));
    BOOST_CHECK_EQUAL(kt.getAll().size(), 0u);
}

BOOST_AUTO_TEST_CASE(KukushkaTest_GrowsWhenFull)
{
    Kukushka kt;
    for (int i = 0; i < 100; i++)  // в начале 2 таблицы по 11 ячеек - придется расширяться
    {
        kt.insert(i);
    }
    for (int i = 0; i < 100; i++)
    {
        BOOST_CHECK(kt.search(i));
    }
    BOOST_CHECK_EQUAL(kt.getAll().size(), 100u);
}

// ===== handleHash =====
static void runHash(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleHash(command, db, "h", ss);
}

BOOST_AUTO_TEST_CASE(HashHandlerTest_AllCommands)
{
    json db = json::object();

    startCapture();
    runHash(db, Command::HADD, "5");
    runHash(db, Command::HADD, "105");
    runHash(db, Command::HADD, "7");
    runHash(db, Command::HDEL, "7");
    runHash(db, Command::HFIND, "5");
    runHash(db, Command::HFIND, "7");
    runHash(db, Command::HPRINT, "");
    runHash(db, Command::PRINT, "");         // чужая команда
    string out = stopCapture();

    BOOST_CHECK_NE(out.find("-> TRUE"), string::npos);
    BOOST_CHECK_NE(out.find("-> FALSE"), string::npos);
    BOOST_CHECK_EQUAL(db["h"]["type"].get<string>(), "hash");
    BOOST_CHECK_EQUAL(db["h"]["data"].size(), 2u);
}

BOOST_AUTO_TEST_CASE(HashHandlerTest_WrongTypeIsRejected)
{
    json db = json::object();
    db["h"]["type"] = "kukushka";
    db["h"]["data"] = json::array();

    startCapture();
    runHash(db, Command::HADD, "5");
    stopCapture();

    BOOST_CHECK_EQUAL(db["h"]["type"].get<string>(), "kukushka");
}

// ===== handleKukushka =====
static void runKuk(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleKukushka(command, db, "k", ss);
}

BOOST_AUTO_TEST_CASE(KukushkaHandlerTest_AllCommands)
{
    json db = json::object();

    startCapture();
    runKuk(db, Command::KADD, "1");
    runKuk(db, Command::KADD, "12");
    runKuk(db, Command::KADD, "7");
    runKuk(db, Command::KDEL, "7");
    runKuk(db, Command::KFIND, "1");
    runKuk(db, Command::KFIND, "7");
    runKuk(db, Command::KPRINT, "");
    runKuk(db, Command::PRINT, "");          // чужая команда
    string out = stopCapture();

    BOOST_CHECK_NE(out.find("-> TRUE"), string::npos);
    BOOST_CHECK_NE(out.find("-> FALSE"), string::npos);
    BOOST_CHECK_EQUAL(db["k"]["type"].get<string>(), "kukushka");
    BOOST_CHECK_EQUAL(db["k"]["data"].size(), 2u);
}

BOOST_AUTO_TEST_CASE(KukushkaHandlerTest_WrongTypeIsRejected)
{
    json db = json::object();
    db["k"]["type"] = "hash";
    db["k"]["data"] = json::array();

    startCapture();
    runKuk(db, Command::KADD, "5");
    stopCapture();

    BOOST_CHECK_EQUAL(db["k"]["type"].get<string>(), "hash");
}
