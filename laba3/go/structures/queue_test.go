package structures

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
)

// конструктор
func TestQueueNewQueueIsEmpty(t *testing.T) {
	q := NewQueue()
	assert.True(t, q.IsEmptyQ())
}

// Push и Top
func TestQueueTopIsFirstPushed(t *testing.T) {
	q := NewQueue()
	q.Push("1")
	q.Push("2")

	assert.False(t, q.IsEmptyQ())
	assert.Equal(t, "1", q.Top()) // первым пришел - первым выйдет
}

// Pop
func TestQueuePopRemovesFirst(t *testing.T) {
	q := NewQueue()
	q.Push("1")
	q.Push("2")
	q.Pop()

	assert.Equal(t, "2", q.Top())
}

func TestQueuePopLastMakesEmpty(t *testing.T) {
	q := NewQueue()
	q.Push("1")
	q.Pop()
	assert.True(t, q.IsEmptyQ())

	q.Push("2") // после опустошения снова работает
	assert.Equal(t, "2", q.Top())
}

func TestQueuePopAndTopOnEmpty(t *testing.T) {
	q := NewQueue()
	q.Pop()
	assert.True(t, q.IsEmptyQ())
	assert.Equal(t, "", q.Top())
}

// PrintQ
func TestQueuePrintFromHead(t *testing.T) {
	q := NewQueue()
	q.Push("1")
	q.Push("2")

	out := captureOutput(func() { q.PrintQ() })
	assert.Equal(t, "1 -> 2 -> nil\n", out)
}

// HandleQueue
func TestQueueHandlerAllCommands(t *testing.T) {
	db := DB{}

	out := captureOutput(func() {
		HandleQueue(QPUSH, db, "q", args("1"))
		HandleQueue(QPUSH, db, "q", args("2"))
		HandleQueue(QPUSH, db, "q", args("3"))
		HandleQueue(QPOP, db, "q", args("")) // выходит 1
		HandleQueue(QPRINT, db, "q", args(""))
		HandleQueue(PRINT, db, "q", args("")) // чужая команда - ничего не делает
	})

	assert.Contains(t, out, "-> 1")
	assert.Equal(t, "queue", db["q"].Type)
	assert.Equal(t, []string{"2", "3"}, loadStrings(db, "q"))
}

func TestQueueHandlerPopEmpty(t *testing.T) {
	db := DB{}
	out := captureOutput(func() { HandleQueue(QPOP, db, "q", args("")) })
	assert.Contains(t, out, "очередь пуста")
}

func TestQueueHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"q": {Type: "stack", Data: json.RawMessage("[]")}}
	captureOutput(func() { HandleQueue(QPUSH, db, "q", args("1")) })
	assert.Equal(t, "stack", db["q"].Type)
}
