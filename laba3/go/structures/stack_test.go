package structures

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
)

// конструктор
func TestStackNewStackIsEmpty(t *testing.T) {
	st := NewStack()
	assert.True(t, st.IsEmptyStack())
}

// Push и Top
func TestStackTopIsLastPushed(t *testing.T) {
	st := NewStack()
	st.Push("1")
	st.Push("2")

	assert.False(t, st.IsEmptyStack())
	assert.Equal(t, "2", st.Top())
}

// Pop
func TestStackPopRemovesTop(t *testing.T) {
	st := NewStack()
	st.Push("1")
	st.Push("2")
	st.Pop()

	assert.Equal(t, "1", st.Top())
}

func TestStackPopAndTopOnEmpty(t *testing.T) {
	st := NewStack()
	st.Pop() // на пустом стеке ничего не ломается
	assert.True(t, st.IsEmptyStack())
	assert.Equal(t, "", st.Top())
}

// Print
func TestStackPrintFromTop(t *testing.T) {
	st := NewStack()
	st.Push("1")
	st.Push("2")

	out := captureOutput(func() { st.Print() })
	assert.Equal(t, "2 -> 1 -> nil\n", out)
}

// HandleStack
func TestStackHandlerAllCommands(t *testing.T) {
	db := DB{}

	out := captureOutput(func() {
		HandleStack(SPUSH, db, "st", args("1"))
		HandleStack(SPUSH, db, "st", args("2"))
		HandleStack(SPUSH, db, "st", args("3"))
		HandleStack(SPOP, db, "st", args("")) // снимает 3
		HandleStack(SPRINT, db, "st", args(""))
		HandleStack(PRINT, db, "st", args("")) // чужая команда - ничего не делает
	})

	assert.Contains(t, out, "-> 3")
	assert.Equal(t, "stack", db["st"].Type)
	assert.Equal(t, []string{"1", "2"}, loadStrings(db, "st")) // в json снизу вверх
}

func TestStackHandlerPopEmpty(t *testing.T) {
	db := DB{}
	out := captureOutput(func() { HandleStack(SPOP, db, "st", args("")) })
	assert.Contains(t, out, "стек пуст")
}

func TestStackHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"st": {Type: "queue", Data: json.RawMessage("[]")}}
	captureOutput(func() { HandleStack(SPUSH, db, "st", args("1")) })
	assert.Equal(t, "queue", db["st"].Type)
}
