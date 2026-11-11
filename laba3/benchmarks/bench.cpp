#include <benchmark/benchmark.h>
#include <random>
#include "arrayClass.h"
#include "stackClass.h"
#include "queueClass.h"
#include "flistClass.h"
#include "dlistClass.h"
#include "treeClass.h"
#include "hashTable.h"

// В каждом бенчмарке меряем, сколько времени занимает добавить N элементов.
// N задается в строке BENCHMARK(...)->Arg(...) и читается как state.range(0).

// массив
static void BM_ArrayPushBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        Array arr;
        for (int i = 0; i < state.range(0); i++)
        {
            arr.pushBack("x");
        }
    }
}
BENCHMARK(BM_ArrayPushBack)->Arg(1000)->Arg(10000);

// стек
static void BM_StackPush(benchmark::State& state)
{
    for (auto _ : state)
    {
        Stack st;
        for (int i = 0; i < state.range(0); i++)
        {
            st.push("x");
        }
    }
}
BENCHMARK(BM_StackPush)->Arg(1000)->Arg(10000);

// очередь
static void BM_QueuePush(benchmark::State& state)
{
    for (auto _ : state)
    {
        Queue q;
        for (int i = 0; i < state.range(0); i++)
        {
            q.push("x");
        }
    }
}
BENCHMARK(BM_QueuePush)->Arg(1000)->Arg(10000);

// односвязный список (addTail каждый раз идет до конца списка)
static void BM_FlistAddTail(benchmark::State& state)
{
    for (auto _ : state)
    {
        Flist fl;
        for (int i = 0; i < state.range(0); i++)
        {
            fl.addTail("x");
        }
    }
}
BENCHMARK(BM_FlistAddTail)->Arg(1000)->Arg(10000);

// двусвязный список (pushBack сразу идет в хвост)
static void BM_DlistPushBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        Dlist dl;
        for (int i = 0; i < state.range(0); i++)
        {
            dl.pushBack("x");
        }
    }
}
BENCHMARK(BM_DlistPushBack)->Arg(1000)->Arg(10000);

// дерево: ключи в случайном порядке, чтобы дерево не вытянулось в линию
static void BM_TreeInsert(benchmark::State& state)
{
    mt19937 gen(42);                          // генератор случайных чисел, 42 - чтобы каждый раз одинаково
    vector<int> keys;
    for (int i = 0; i < state.range(0); i++)
    {
        keys.push_back(int(gen() % 1000000));
    }

    for (auto _ : state)
    {
        Tree tr;
        for (int key : keys)
        {
            tr.insert(key);
        }
    }
}
BENCHMARK(BM_TreeInsert)->Arg(1000)->Arg(10000);

// хеш-таблица с цепочками
static void BM_HashAdd(benchmark::State& state)
{
    for (auto _ : state)
    {
        hashTable ht;
        for (int i = 0; i < state.range(0); i++)
        {
            ht.add(i);
        }
    }
}
BENCHMARK(BM_HashAdd)->Arg(1000)->Arg(10000);

// хеш-таблица кукушки
static void BM_KukushkaInsert(benchmark::State& state)
{
    for (auto _ : state)
    {
        Kukushka kt;
        for (int i = 0; i < state.range(0); i++)
        {
            kt.insert(i);
        }
    }
}
BENCHMARK(BM_KukushkaInsert)->Arg(1000)->Arg(10000);

BENCHMARK_MAIN();