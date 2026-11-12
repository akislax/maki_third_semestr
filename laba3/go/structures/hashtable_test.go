package structures

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
)

// Таблица с цепочками.
// 5, 105 и 205 попадают в одну ячейку 5 (хеш = сумма свертки % 100),
// поэтому на них удобно проверять цепочки.

func TestHashTableAddAndSearch(t *testing.T) {
	ht := NewHashTable()
	ht.Add(5)
	ht.Add(123456)

	assert.True(t, ht.Search(5))
	assert.True(t, ht.Search(123456))
	assert.False(t, ht.Search(7))
}

func TestHashTableDuplicateIsIgnored(t *testing.T) {
	ht := NewHashTable()
	ht.Add(5)
	ht.Add(5)
	assert.Len(t, ht.GetAll(), 1)
}

func TestHashTableCollisionsGoToOneChain(t *testing.T) {
	ht := NewHashTable()
	ht.Add(5)
	ht.Add(105)
	ht.Add(205)

	out := captureOutput(func() { ht.Print() })
	assert.Equal(t, "5: 205 -> 105 -> 5 -> nil\n", out) // новые встают в начало цепочки
}

func TestHashTableRemoveFromChain(t *testing.T) {
	ht := NewHashTable()
	ht.Add(5)
	ht.Add(105)
	ht.Add(205) // цепочка: 205 -> 105 -> 5

	ht.Remove(105) // из середины
	ht.Remove(205) // из начала
	ht.Remove(999) // такого нет

	assert.False(t, ht.Search(105))
	assert.False(t, ht.Search(205))
	assert.True(t, ht.Search(5))
}

func TestHashTableNegativeKey(t *testing.T) {
	ht := NewHashTable()
	ht.Add(-7)
	assert.True(t, ht.Search(-7))
}

// Таблица кукушки.
// 1 и 12 дают одинаковый hash1 (12 % 11 = 1), поэтому второй
// выталкивает первого во вторую таблицу.

func TestKukushkaInsertAndSearch(t *testing.T) {
	kt := NewKukushka()
	kt.Insert(42)
	assert.True(t, kt.Search(42))
	assert.False(t, kt.Search(7))
}

func TestKukushkaDuplicateIsIgnored(t *testing.T) {
	kt := NewKukushka()
	kt.Insert(42)
	kt.Insert(42)
	assert.Len(t, kt.GetAll(), 1)
}

func TestKukushkaCollisionMovesToSecondTable(t *testing.T) {
	kt := NewKukushka()
	kt.Insert(1)
	kt.Insert(12) // выталкивает 1 во вторую таблицу

	out := captureOutput(func() { kt.Print() })
	assert.Equal(t, "t1: [1]=12 \nt2: [1]=1 \n", out)
	assert.True(t, kt.Search(1))  // находится во второй таблице
	assert.True(t, kt.Search(12)) // находится в первой
}

func TestKukushkaRemoveFromBothTables(t *testing.T) {
	kt := NewKukushka()
	kt.Insert(1)
	kt.Insert(12)

	kt.Remove(12)  // из первой таблицы
	kt.Remove(1)   // из второй
	kt.Remove(999) // такого нет

	assert.False(t, kt.Search(1))
	assert.False(t, kt.Search(12))
	assert.Empty(t, kt.GetAll())
}

func TestKukushkaGrowsWhenFull(t *testing.T) {
	kt := NewKukushka()
	for i := 0; i < 100; i++ { // в начале 2 таблицы по 11 ячеек - придется расширяться
		kt.Insert(i)
	}
	for i := 0; i < 100; i++ {
		assert.True(t, kt.Search(i))
	}
	assert.Len(t, kt.GetAll(), 100)
}

// HandleHash
func TestHashHandlerAllCommands(t *testing.T) {
	db := DB{}
	run := func(command Command, a string) { HandleHash(command, db, "h", args(a)) }

	out := captureOutput(func() {
		run(HADD, "5")
		run(HADD, "105")
		run(HADD, "7")
		run(HDEL, "7")
		run(HFIND, "5")
		run(HFIND, "7")
		run(HPRINT, "")
		run(PRINT, "") // чужая команда
	})

	assert.Contains(t, out, "-> TRUE")
	assert.Contains(t, out, "-> FALSE")
	assert.Equal(t, "hash", db["h"].Type)
	assert.Len(t, loadInts(db, "h"), 2)
}

func TestHashHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"h": {Type: "kukushka", Data: json.RawMessage("[]")}}
	captureOutput(func() { HandleHash(HADD, db, "h", args("5")) })
	assert.Equal(t, "kukushka", db["h"].Type)
}

// HandleKukushka
func TestKukushkaHandlerAllCommands(t *testing.T) {
	db := DB{}
	run := func(command Command, a string) { HandleKukushka(command, db, "k", args(a)) }

	out := captureOutput(func() {
		run(KADD, "1")
		run(KADD, "12")
		run(KADD, "7")
		run(KDEL, "7")
		run(KFIND, "1")
		run(KFIND, "7")
		run(KPRINT, "")
		run(PRINT, "") // чужая команда
	})

	assert.Contains(t, out, "-> TRUE")
	assert.Contains(t, out, "-> FALSE")
	assert.Equal(t, "kukushka", db["k"].Type)
	assert.Len(t, loadInts(db, "k"), 2)
}

func TestKukushkaHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"k": {Type: "hash", Data: json.RawMessage("[]")}}
	captureOutput(func() { HandleKukushka(KADD, db, "k", args("5")) })
	assert.Equal(t, "hash", db["k"].Type)
}
