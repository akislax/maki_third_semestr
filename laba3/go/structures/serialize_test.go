package structures

import (
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/assert"
)

// Каждый тест: заполнить структуру, сохранить в файл,
// загрузить в новую структуру и проверить, что все совпало.
// Файлы создаются во временной папке, Go сам удалит ее после теста.

func TestSerializationArray(t *testing.T) {
	file := filepath.Join(t.TempDir(), "array.bin")

	arr := NewArray()
	arr.PushBack("кот")
	arr.PushBack("пес")
	arr.SaveBinary(file)

	loaded := NewArray()
	loaded.PushBack("старое") // должно стереться при загрузке
	loaded.LoadBinary(file)

	assert.Equal(t, 2, loaded.Length())
	assert.Equal(t, "кот", loaded.GetAt(0))
	assert.Equal(t, "пес", loaded.GetAt(1))
}

func TestSerializationStack(t *testing.T) {
	file := filepath.Join(t.TempDir(), "stack.bin")

	st := NewStack()
	st.Push("1")
	st.Push("2")
	st.Push("3")
	st.SaveBinary(file)

	loaded := NewStack()
	loaded.Push("старое")
	loaded.LoadBinary(file)

	assert.Equal(t, "3", loaded.Top()) // вершина та же
	loaded.Pop()
	assert.Equal(t, "2", loaded.Top())
	loaded.Pop()
	assert.Equal(t, "1", loaded.Top())
	loaded.Pop()
	assert.True(t, loaded.IsEmptyStack())
}

func TestSerializationQueue(t *testing.T) {
	file := filepath.Join(t.TempDir(), "queue.bin")

	q := NewQueue()
	q.Push("1")
	q.Push("2")
	q.SaveBinary(file)

	loaded := NewQueue()
	loaded.Push("старое")
	loaded.LoadBinary(file)

	assert.Equal(t, "1", loaded.Top())
	loaded.Pop()
	assert.Equal(t, "2", loaded.Top())
	loaded.Pop()
	assert.True(t, loaded.IsEmptyQ())
}

func TestSerializationFlist(t *testing.T) {
	file := filepath.Join(t.TempDir(), "flist.bin")

	fl := NewFlist()
	fl.AddTail("a")
	fl.AddTail("b")
	fl.SaveBinary(file)

	loaded := NewFlist()
	loaded.AddTail("старое")
	loaded.LoadBinary(file)

	assert.Equal(t, "a -> b -> nil\n", showFlist(loaded))
}

func TestSerializationDlist(t *testing.T) {
	file := filepath.Join(t.TempDir(), "dlist.bin")

	dl := NewDlist()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.SaveBinary(file)

	loaded := NewDlist()
	loaded.PushBack("старое")
	loaded.LoadBinary(file)

	assert.Equal(t, "a <-> b <-> nil\n", forward(loaded))
}

func TestSerializationTree(t *testing.T) {
	file := filepath.Join(t.TempDir(), "tree.bin")

	tr := NewTree()
	tr.Insert(5)
	tr.Insert(3)
	tr.Insert(8)
	tr.SaveBinary(file)

	loaded := NewTree()
	loaded.Insert(100)
	loaded.LoadBinary(file)

	assert.Equal(t, 5, loaded.GetRoot().Key) // форма дерева та же
	assert.Equal(t, 3, loaded.GetRoot().Left.Key)
	assert.Equal(t, 8, loaded.GetRoot().Right.Key)
	assert.Nil(t, loaded.Search(100)) // старое стерлось
}

func TestSerializationHashTable(t *testing.T) {
	file := filepath.Join(t.TempDir(), "hash.bin")

	ht := NewHashTable()
	ht.Add(5)
	ht.Add(105)
	ht.Add(7)
	ht.SaveBinary(file)

	loaded := NewHashTable()
	loaded.Add(999)
	loaded.LoadBinary(file)

	assert.True(t, loaded.Search(5))
	assert.True(t, loaded.Search(105))
	assert.True(t, loaded.Search(7))
	assert.False(t, loaded.Search(999))
}

func TestSerializationKukushka(t *testing.T) {
	file := filepath.Join(t.TempDir(), "kukushka.bin")

	kt := NewKukushka()
	kt.Insert(1)
	kt.Insert(12)
	kt.Insert(42)
	kt.SaveBinary(file)

	loaded := NewKukushka()
	loaded.Insert(999)
	loaded.LoadBinary(file)

	assert.True(t, loaded.Search(1))
	assert.True(t, loaded.Search(12))
	assert.True(t, loaded.Search(42))
	assert.False(t, loaded.Search(999))
}

func TestSerializationMissingFile(t *testing.T) {
	missing := filepath.Join(t.TempDir(), "нет_такого_файла.bin")
	badPath := filepath.Join(t.TempDir(), "нет_такой_папки", "file.bin")

	// загрузка из несуществующего файла ничего не меняет
	arr := NewArray()
	arr.PushBack("a")
	arr.LoadBinary(missing)
	assert.Equal(t, 1, arr.Length())

	st := NewStack()
	st.LoadBinary(missing)
	q := NewQueue()
	q.LoadBinary(missing)
	fl := NewFlist()
	fl.LoadBinary(missing)
	dl := NewDlist()
	dl.LoadBinary(missing)
	tr := NewTree()
	tr.LoadBinary(missing)
	ht := NewHashTable()
	ht.LoadBinary(missing)
	kt := NewKukushka()
	kt.LoadBinary(missing)

	assert.True(t, st.IsEmptyStack())
	assert.True(t, q.IsEmptyQ())
	assert.True(t, fl.IsEmptyList())
	assert.True(t, dl.IsEmptyList())
	assert.Nil(t, tr.GetRoot())
	assert.Empty(t, ht.GetAll())
	assert.Empty(t, kt.GetAll())

	// сохранение в несуществующую папку не падает
	arr.SaveBinary(badPath)
	st.SaveBinary(badPath)
	q.SaveBinary(badPath)
	fl.SaveBinary(badPath)
	dl.SaveBinary(badPath)
	tr.SaveBinary(badPath)
	ht.SaveBinary(badPath)
	kt.SaveBinary(badPath)
	assert.NoFileExists(t, badPath)
}
