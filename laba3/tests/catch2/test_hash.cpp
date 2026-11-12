#include <catch2/catch_test_macros.hpp>
#include "capture.h"
#include "hashTable.h"

// ===== таблица с цепочками =====
// 5, 105 и 205 попадают в одну ячейку 5 (хеш = сумма свертки % 100),
// поэтому на них удобно проверять цепочки

TEST_CASE("HashTableTest.AddAndSearch")
{
    hashTable ht;
    ht.add(5);
    ht.add(123456);

    CHECK(ht.search(5));
    CHECK(ht.search(123456));
    CHECK_FALSE(ht.search(7));
}

TEST_CASE("HashTableTest.DuplicateIsIgnored")
{
    hashTable ht;
    ht.add(5);
    ht.add(5);
    CHECK(ht.getAll().size() == 1u);
}

TEST_CASE("HashTableTest.CollisionsGoToOneChain")
{
    hashTable ht;
    ht.add(5);
    ht.add(105);
    ht.add(205);

    startCapture();
    ht.print();
    string out = stopCapture();

    CHECK(out == "5: 205 -> 105 -> 5 -> nullptr\n");   // новые встают в начало цепочки
}

TEST_CASE("HashTableTest.RemoveFromChain")
{
    hashTable ht;
    ht.add(5);
    ht.add(105);
    ht.add(205);              // цепочка: 205 -> 105 -> 5

    ht.remove(105);           // из середины
    ht.remove(205);           // из начала
    ht.remove(999);           // такого нет

    CHECK_FALSE(ht.search(105));
    CHECK_FALSE(ht.search(205));
    CHECK(ht.search(5));
}

TEST_CASE("HashTableTest.NegativeKey")
{
    hashTable ht;
    ht.add(-7);
    CHECK(ht.search(-7));
}

// ===== кукушка =====
// 1 и 12 дают одинаковый hash1 (12 % 11 = 1), поэтому второй вытесняет
// первого во вторую таблицу

TEST_CASE("KukushkaTest.InsertAndSearch")
{
    Kukushka kt;
    kt.insert(42);
    CHECK(kt.search(42));
    CHECK_FALSE(kt.search(7));
}

TEST_CASE("KukushkaTest.DuplicateIsIgnored")
{
    Kukushka kt;
    kt.insert(42);
    kt.insert(42);
    CHECK(kt.getAll().size() == 1u);
}

TEST_CASE("KukushkaTest.CollisionMovesToSecondTable")
{
    Kukushka kt;
    kt.insert(1);
    kt.insert(12);            // выталкивает 1 во вторую таблицу

    startCapture();
    kt.print();
    string out = stopCapture();

    CHECK(out == "t1: [1]=12 \nt2: [1]=1 \n");
    CHECK(kt.search(1));     // находится во второй таблице
    CHECK(kt.search(12));    // находится в первой
}

TEST_CASE("KukushkaTest.RemoveFromBothTables")
{
    Kukushka kt;
    kt.insert(1);
    kt.insert(12);

    kt.remove(12);            // из первой таблицы
    kt.remove(1);             // из второй
    kt.remove(999);           // такого нет

    CHECK_FALSE(kt.search(1));
    CHECK_FALSE(kt.search(12));
    CHECK(kt.getAll().size() == 0u);
}

TEST_CASE("KukushkaTest.GrowsWhenFull")
{
    Kukushka kt;
    for (int i = 0; i < 100; i++)  // в начале 2 таблицы по 11 ячеек - придется расширяться
    {
        kt.insert(i);
    }
    for (int i = 0; i < 100; i++)
    {
        CHECK(kt.search(i));
    }
    CHECK(kt.getAll().size() == 100u);
}

// ===== handleHash =====
static void runHash(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleHash(command, db, "h", ss);
}

TEST_CASE("HashHandlerTest.AllCommands")
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

    CHECK(out.find("-> TRUE") != string::npos);
    CHECK(out.find("-> FALSE") != string::npos);
    CHECK(db["h"]["type"].get<string>() == "hash");
    CHECK(db["h"]["data"].size() == 2u);
}

TEST_CASE("HashHandlerTest.WrongTypeIsRejected")
{
    json db = json::object();
    db["h"]["type"] = "kukushka";
    db["h"]["data"] = json::array();

    startCapture();
    runHash(db, Command::HADD, "5");
    stopCapture();

    CHECK(db["h"]["type"].get<string>() == "kukushka");
}

// ===== handleKukushka =====
static void runKuk(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleKukushka(command, db, "k", ss);
}

TEST_CASE("KukushkaHandlerTest.AllCommands")
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

    CHECK(out.find("-> TRUE") != string::npos);
    CHECK(out.find("-> FALSE") != string::npos);
    CHECK(db["k"]["type"].get<string>() == "kukushka");
    CHECK(db["k"]["data"].size() == 2u);
}

TEST_CASE("KukushkaHandlerTest.WrongTypeIsRejected")
{
    json db = json::object();
    db["k"]["type"] = "hash";
    db["k"]["data"] = json::array();

    startCapture();
    runKuk(db, Command::KADD, "5");
    stopCapture();

    CHECK(db["k"]["type"].get<string>() == "hash");
}
