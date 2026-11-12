package structures

import (
	"encoding/json"
	"strconv"
	"testing"

	"github.com/stretchr/testify/assert"
)

// конструктор
func TestArrayNewArrayIsEmpty(t *testing.T) {
	arr := NewArray()
	assert.Equal(t, 0, arr.Length())
}

// PushBack
func TestArrayPushBackAddsToEnd(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.PushBack("b")

	assert.Equal(t, 2, arr.Length())
	assert.Equal(t, "a", arr.GetAt(0))
	assert.Equal(t, "b", arr.GetAt(1))
}

// resizeArray (через PushBack)
func TestArrayGrowsBeyondStartCapacity(t *testing.T) {
	arr := NewArray()
	for i := 0; i < 10; i++ { // вместимость 4, должно расшириться
		arr.PushBack(strconv.Itoa(i))
	}
	assert.Equal(t, 10, arr.Length())
	assert.Equal(t, "9", arr.GetAt(9))
}

// InsertAt
func TestArrayInsertInMiddle(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.PushBack("c")
	arr.InsertAt("b", 1)

	assert.Equal(t, "a", arr.GetAt(0))
	assert.Equal(t, "b", arr.GetAt(1))
	assert.Equal(t, "c", arr.GetAt(2))
}

func TestArrayInsertWhenFull(t *testing.T) {
	arr := NewArray()
	for i := 0; i < 4; i++ { // заполнили всю вместимость
		arr.PushBack("x")
	}
	arr.InsertAt("y", 0) // InsertAt должен сам расширить массив
	assert.Equal(t, 5, arr.Length())
	assert.Equal(t, "y", arr.GetAt(0))
}

func TestArrayInsertWrongIndex(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	captureOutput(func() { arr.InsertAt("x", 5) }) // такого индекса нет
	assert.Equal(t, 1, arr.Length())
}

// RemoveAt
func TestArrayRemoveShiftsElements(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.PushBack("b")
	arr.PushBack("c")
	arr.RemoveAt(0)

	assert.Equal(t, 2, arr.Length())
	assert.Equal(t, "b", arr.GetAt(0))
}

func TestArrayRemoveWrongIndex(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	captureOutput(func() { arr.RemoveAt(3) })
	assert.Equal(t, 1, arr.Length())
}

// SetAt
func TestArraySetAtChangesValue(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.SetAt("b", 0)
	assert.Equal(t, "b", arr.GetAt(0))
}

func TestArraySetAtWrongIndex(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	captureOutput(func() { arr.SetAt("b", 3) })
	assert.Equal(t, "a", arr.GetAt(0)) // старое значение осталось
}

// GetAt
func TestArrayGetAtWrongIndex(t *testing.T) {
	arr := NewArray()
	var got string
	captureOutput(func() { got = arr.GetAt(0) })
	assert.Equal(t, "", got) // при ошибке возвращает пустую строку
}

// PrintArray
func TestArrayPrintShowsElements(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.PushBack("b")

	out := captureOutput(func() { arr.PrintArray() })
	assert.Equal(t, "a b (size=2, capacity=4)\n", out)
}

// HandleArray
func TestArrayHandlerAllCommands(t *testing.T) {
	db := DB{}

	captureOutput(func() {
		HandleArray(MPUSH, db, "arr", args("a"))       // [a]
		HandleArray(MPUSH, db, "arr", args("c"))       // [a, c]
		HandleArray(MINSERT, db, "arr", args("1 b"))   // [a, b, c]
		HandleArray(MSET, db, "arr", args("0 z"))      // [z, b, c]
		HandleArray(MDEL, db, "arr", args("2"))        // [z, b]
		HandleArray(MGET, db, "arr", args("1"))
		HandleArray(MLEN, db, "arr", args(""))
		HandleArray(MPRINT, db, "arr", args(""))
		HandleArray(PRINT, db, "arr", args("")) // чужая команда - ничего не делает
	})

	assert.Equal(t, "array", db["arr"].Type)
	assert.Equal(t, []string{"z", "b"}, loadStrings(db, "arr"))
}

func TestArrayHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"arr": {Type: "stack", Data: json.RawMessage("[]")}} // под этим именем уже стек

	out := captureOutput(func() { HandleArray(MPUSH, db, "arr", args("a")) })

	assert.Equal(t, "stack", db["arr"].Type) // тип не поменялся
	assert.Contains(t, out, "это не array")  // и вывелось сообщение
}
