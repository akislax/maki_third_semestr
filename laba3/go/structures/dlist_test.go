package structures

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
)

// forward и backward печатают список в строку, чтобы удобно сравнивать.
func forward(dl *Dlist) string {
	return captureOutput(func() { dl.PrintForward() })
}

func backward(dl *Dlist) string {
	return captureOutput(func() { dl.PrintBackward() })
}

// конструктор
func TestDlistNewListIsEmpty(t *testing.T) {
	dl := NewDlist()
	assert.True(t, dl.IsEmptyList())
	assert.Nil(t, dl.GetHead())
}

// PushBack и PopBack
func TestDlistPushBackAndPopBack(t *testing.T) {
	dl := NewDlist()
	dl.PushBack("a")
	dl.PushBack("b")
	assert.Equal(t, "a <-> b <-> nil\n", forward(dl))
	assert.Equal(t, "b <-> a <-> nil\n", backward(dl))

	dl.PopBack()
	assert.Equal(t, "a <-> nil\n", forward(dl))

	dl.PopBack()
	dl.PopBack() // на пустом ничего не ломается
	assert.True(t, dl.IsEmptyList())
}

// PushFront и PopFront
func TestDlistPushFrontAndPopFront(t *testing.T) {
	dl := NewDlist()
	dl.PushFront("b")
	dl.PushFront("a")
	assert.Equal(t, "a <-> b <-> nil\n", forward(dl))

	dl.PopFront()
	assert.Equal(t, "b <-> nil\n", forward(dl))

	dl.PopFront()
	dl.PopFront() // на пустом ничего не ломается
	assert.True(t, dl.IsEmptyList())
}

// PushBetween - вставка после узла
func TestDlistPushAfterNode(t *testing.T) {
	dl := NewDlist()
	dl.PushBack("a")
	dl.PushBack("c")
	dl.PushBetween(dl.FindName("a"), "b") // в середину
	dl.PushBetween(dl.FindName("c"), "d") // после хвоста - d станет хвостом
	dl.PushBetween(nil, "x")

	assert.Equal(t, "a <-> b <-> c <-> d <-> nil\n", forward(dl))
	assert.Equal(t, "d <-> c <-> b <-> a <-> nil\n", backward(dl))
}

// PushBefore
func TestDlistPushBeforeNode(t *testing.T) {
	dl := NewDlist()
	dl.PushBack("b")
	dl.PushBack("d")
	dl.PushBefore(dl.FindName("b"), "a") // перед головой
	dl.PushBefore(dl.FindName("d"), "c") // в середину
	dl.PushBefore(nil, "x")

	assert.Equal(t, "a <-> b <-> c <-> d <-> nil\n", forward(dl))
}

// PopAfter
func TestDlistPopAfterNode(t *testing.T) {
	dl := NewDlist()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.PushBack("c")

	dl.PopAfter(dl.FindName("a")) // из середины
	assert.Equal(t, "a <-> c <-> nil\n", forward(dl))

	dl.PopAfter(dl.FindName("a")) // хвост - a станет хвостом
	assert.Equal(t, "a <-> nil\n", backward(dl))

	dl.PopAfter(dl.FindName("a")) // после хвоста никого нет
	dl.PopAfter(nil)
	assert.Equal(t, "a <-> nil\n", forward(dl))
}

// PopBefore
func TestDlistPopBeforeNode(t *testing.T) {
	dl := NewDlist()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.PushBack("c")

	dl.PopBefore(dl.FindName("c")) // из середины
	assert.Equal(t, "a <-> c <-> nil\n", forward(dl))

	dl.PopBefore(dl.FindName("c")) // голову - c станет головой
	assert.Equal(t, "c <-> nil\n", forward(dl))

	dl.PopBefore(dl.FindName("c")) // перед головой никого нет
	dl.PopBefore(nil)
	assert.Equal(t, "c <-> nil\n", forward(dl))
}

// FindName
func TestDlistFindName(t *testing.T) {
	dl := NewDlist()
	dl.PushBack("a")
	assert.NotNil(t, dl.FindName("a"))
	assert.Nil(t, dl.FindName("z"))
}

// PopName
func TestDlistPopName(t *testing.T) {
	dl := NewDlist()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.PushBack("c")

	dl.PopName("a") // голова
	dl.PopName("c") // хвост
	out := captureOutput(func() { dl.PopName("z") }) // такого нет

	assert.Equal(t, "b <-> nil\n", forward(dl))
	assert.Contains(t, out, "нет такого элемента")
}

// HandleDlist
func TestDlistHandlerAllCommands(t *testing.T) {
	db := DB{}
	run := func(command Command, a string) { HandleDlist(command, db, "dl", args(a)) }

	out := captureOutput(func() {
		run(LPUSHT, "b")   // b
		run(LPUSHH, "a")   // a b
		run(LPUSHT, "e")   // a b e
		run(LPUSHA, "b c") // a b c e
		run(LPUSHB, "e d") // a b c d e
		run(LPUSHA, "z x") // нет z
		run(LPUSHB, "z x") // нет z
		run(LDELH, "")     // b c d e
		run(LDELT, "")     // b c d
		run(LDELA, "b")    // b d
		run(LDELA, "z")    // нет z
		run(LDELB, "d")    // d
		run(LDELB, "z")    // нет z
		run(LPUSHT, "k")   // d k
		run(LDEL, "d")     // k
		run(LFIND, "k")
		run(LFIND, "z")
		run(LPRINT, "")
		run(LPRINTR, "")
		run(PRINT, "") // чужая команда
	})

	assert.Contains(t, out, "-> TRUE")
	assert.Contains(t, out, "-> FALSE")
	assert.Equal(t, "dlist", db["dl"].Type)
	assert.Equal(t, []string{"k"}, loadStrings(db, "dl"))
}

func TestDlistHandlerWrongTypeIsRejected(t *testing.T) {
	db := DB{"dl": {Type: "flist", Data: json.RawMessage("[]")}}
	captureOutput(func() { HandleDlist(LPUSHT, db, "dl", args("a")) })
	assert.Equal(t, "flist", db["dl"].Type)
}
