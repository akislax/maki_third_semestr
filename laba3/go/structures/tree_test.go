package structures

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
)

// fillTree строит дерево для тестов:
//
//	      5
//	     / \
//	    3   8
//	   /
//	  1
func fillTree(tr *Tree) {
	tr.Insert(5)
	tr.Insert(3)
	tr.Insert(8)
	tr.Insert(1)
}

// конструктор
func TestTreeNewTreeIsEmpty(t *testing.T) {
	tr := NewTree()
	assert.Nil(t, tr.GetRoot())
}

// Insert и Search
func TestTreeInsertAndSearch(t *testing.T) {
	tr := NewTree()
	fillTree(tr)

	assert.Equal(t, 5, tr.GetRoot().Key)
	assert.NotNil(t, tr.Search(1))
	assert.NotNil(t, tr.Search(8))
	assert.Nil(t, tr.Search(7)) // такого нет
}

func TestTreeInsertGoesLeftAndRight(t *testing.T) {
	tr := NewTree()
	fillTree(tr)

	assert.Equal(t, 3, tr.GetRoot().Left.Key)  // меньше - влево
	assert.Equal(t, 8, tr.GetRoot().Right.Key) // больше - вправо
}

// обходы
func TestTreeTraversals(t *testing.T) {
	tr := NewTree()
	fillTree(tr)

	assert.Equal(t, "5 3 1 8 ", captureOutput(func() { tr.PrintPreorder() }))
	assert.Equal(t, "1 3 5 8 ", captureOutput(func() { tr.PrintInorder() }))
	assert.Equal(t, "1 3 8 5 ", captureOutput(func() { tr.PrintPostorder() }))
	assert.Equal(t, "5 3 8 1 ", captureOutput(func() { tr.Bfs() }))
}

func TestTreeBfsOnEmpty(t *testing.T) {
	tr := NewTree()
	assert.Equal(t, "", captureOutput(func() { tr.Bfs() }))
}

// IsComplete
func TestTreeCompleteTree(t *testing.T) {
	tr := NewTree()
	assert.True(t, tr.IsComplete()) // пустое дерево считается полным

	fillTree(tr) // уровни заполнены слева направо
	assert.True(t, tr.IsComplete())
}

func TestTreeNotCompleteTree(t *testing.T) {
	tr := NewTree()
	tr.Insert(5)
	tr.Insert(8) // только правый ребенок, слева дырка
	tr.Insert(9)
	assert.False(t, tr.IsComplete())
}

// HandleTree
func TestTreeHandlerAllCommands(t *testing.T) {
	db := DB{}
	run := func(command Command, a string) { HandleTree(command, db, "t", args(a)) }

	out := captureOutput(func() {
		run(TINSERT, "5")
		run(TINSERT, "3")
		run(TINSERT, "8")
		run(TFIND, "3")
		run(TFIND, "7")
		run(TCOMPLETE, "")
		run(TPRINT, "")
		run(PRINT, "") // чужая команда
	})

	assert.Contains(t, out, "-> TRUE")
	assert.Contains(t, out, "-> FALSE")
	assert.Equal(t, "tree", db["t"].Type)
	assert.Equal(t, []int{5, 3, 8}, loadInts(db, "t")) // сохранено в ширину
}

func TestTreeHandlerNotComplete(t *testing.T) {
	db := DB{}
	run := func(command Command, a string) { HandleTree(command, db, "t", args(a)) }

	out := captureOutput(func() {
		run(TINSERT, "5")
		run(TINSERT, "8")
		run(TINSERT, "9")
		run(TCOMPLETE, "")
	})

	assert.Contains(t, out, "-> FALSE")
}

func TestTreeHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"t": {Type: "array", Data: json.RawMessage("[]")}}
	captureOutput(func() { HandleTree(TINSERT, db, "t", args("5")) })
	assert.Equal(t, "array", db["t"].Type)
}
