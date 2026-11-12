package structures

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
)

// showFlist печатает список в строку, чтобы удобно сравнивать.
func showFlist(fl *Flist) string {
	return captureOutput(func() { fl.PrintList() })
}

// конструктор
func TestFlistNewListIsEmpty(t *testing.T) {
	fl := NewFlist()
	assert.True(t, fl.IsEmptyList())
	assert.Nil(t, fl.GetHead())
}

// PushFront и PopFront
func TestFlistPushFrontAddsToStart(t *testing.T) {
	fl := NewFlist()
	fl.PushFront("b")
	fl.PushFront("a")
	assert.Equal(t, "a -> b -> nil\n", showFlist(fl))
}

func TestFlistPopFront(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("b")
	fl.PopFront()
	assert.Equal(t, "b -> nil\n", showFlist(fl))

	fl.PopFront()
	fl.PopFront() // на пустом ничего не ломается
	assert.True(t, fl.IsEmptyList())
}

// AddTail и PopTail
func TestFlistAddTailAddsToEnd(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("b")
	assert.Equal(t, "a -> b -> nil\n", showFlist(fl))
}

func TestFlistPopTail(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("b")
	fl.AddTail("c")
	fl.PopTail()
	assert.Equal(t, "a -> b -> nil\n", showFlist(fl))

	fl.PopTail()
	fl.PopTail() // удалили последний элемент
	assert.True(t, fl.IsEmptyList())

	fl.PopTail() // на пустом ничего не ломается
	assert.True(t, fl.IsEmptyList())
}

// AddOne - вставка после узла
func TestFlistAddOneAfterNode(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("c")
	fl.AddOne(fl.FindValue("a"), "b")
	assert.Equal(t, "a -> b -> c -> nil\n", showFlist(fl))

	fl.AddOne(nil, "x") // нет узла - ничего не делаем
	assert.Equal(t, "a -> b -> c -> nil\n", showFlist(fl))
}

// DeleteNode - удаление после узла
func TestFlistDeleteNodeAfterNode(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("b")
	fl.AddTail("c")
	fl.DeleteNode(fl.FindValue("a"))
	assert.Equal(t, "a -> c -> nil\n", showFlist(fl))

	fl.DeleteNode(fl.FindValue("c")) // после последнего никого нет
	fl.DeleteNode(nil)
	assert.Equal(t, "a -> c -> nil\n", showFlist(fl))
}

// AddBefore
func TestFlistAddBefore(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("b")
	fl.AddTail("d")
	fl.AddBefore(fl.FindValue("b"), "a") // перед головой
	fl.AddBefore(fl.FindValue("d"), "c") // в середину
	assert.Equal(t, "a -> b -> c -> d -> nil\n", showFlist(fl))

	fl.AddBefore(nil, "x")
	assert.Equal(t, "a -> b -> c -> d -> nil\n", showFlist(fl))
}

// DeleteBefore
func TestFlistDeleteBefore(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("b")
	fl.AddTail("c")
	fl.AddTail("d")

	fl.DeleteBefore(fl.FindValue("d")) // удалит c
	assert.Equal(t, "a -> b -> d -> nil\n", showFlist(fl))

	fl.DeleteBefore(fl.FindValue("b")) // удалит голову a
	assert.Equal(t, "b -> d -> nil\n", showFlist(fl))

	fl.DeleteBefore(fl.FindValue("b")) // перед головой никого нет
	fl.DeleteBefore(nil)
	assert.Equal(t, "b -> d -> nil\n", showFlist(fl))
}

// FindValue
func TestFlistFindValue(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	assert.NotNil(t, fl.FindValue("a"))
	assert.Nil(t, fl.FindValue("z"))
}

// DeleteValue
func TestFlistDeleteValue(t *testing.T) {
	fl := NewFlist()
	fl.DeleteValue("a") // пустой список

	fl.AddTail("a")
	fl.AddTail("b")
	fl.AddTail("c")
	fl.DeleteValue("a") // голова
	fl.DeleteValue("c") // хвост
	fl.DeleteValue("z") // такого нет
	assert.Equal(t, "b -> nil\n", showFlist(fl))
}

// PrintReverse
func TestFlistPrintReverse(t *testing.T) {
	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("b")

	out := captureOutput(func() { fl.PrintReverse() })
	assert.Equal(t, "b a \n", out)
}

// HandleFlist
func TestFlistHandlerAllCommands(t *testing.T) {
	db := DB{}
	run := func(command Command, a string) { HandleFlist(command, db, "fl", args(a)) }

	out := captureOutput(func() {
		run(FPUSHT, "b")   // b
		run(FPUSHH, "a")   // a b
		run(FPUSHT, "e")   // a b e
		run(FPUSHA, "b c") // a b c e
		run(FPUSHB, "e d") // a b c d e
		run(FPUSHA, "z x") // нет z
		run(FPUSHB, "z x") // нет z
		run(FDELH, "")     // b c d e
		run(FDELT, "")     // b c d
		run(FDELA, "b")    // b d
		run(FDELA, "z")    // нет z
		run(FDELB, "d")    // d
		run(FDELB, "z")    // нет z
		run(FPUSHT, "k")   // d k
		run(FDEL, "d")     // k
		run(FFIND, "k")
		run(FFIND, "z")
		run(FPRINT, "")
		run(FPRINTR, "")
		run(PRINT, "") // чужая команда
	})

	assert.Contains(t, out, "-> TRUE")
	assert.Contains(t, out, "-> FALSE")
	assert.Equal(t, "flist", db["fl"].Type)
	assert.Equal(t, []string{"k"}, loadStrings(db, "fl"))
}

func TestFlistHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"fl": {Type: "dlist", Data: json.RawMessage("[]")}}
	captureOutput(func() { HandleFlist(FPUSHT, db, "fl", args("a")) })
	assert.Equal(t, "dlist", db["fl"].Type)
}
