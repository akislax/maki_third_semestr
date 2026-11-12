package structures

import (
	"fmt"
	"io"
	"os"
)

// Array - динамический массив строк.
type Array struct {
	data     []string
	size     int
	capacity int
}

// NewArray создает пустой массив с начальной вместимостью 4.
func NewArray() *Array {
	capacity := 4
	return &Array{data: make([]string, capacity), size: 0, capacity: capacity}
}

// resizeArray увеличивает вместимость массива в два раза.
func (arr *Array) resizeArray() {
	newCapacity := arr.capacity * 2
	newData := make([]string, newCapacity)
	for i := 0; i < arr.size; i++ {
		newData[i] = arr.data[i]
	}
	arr.data = newData
	arr.capacity = newCapacity
}

// PushBack добавляет элемент в конец.
func (arr *Array) PushBack(text string) {
	if arr.size >= arr.capacity {
		arr.resizeArray()
	}
	arr.data[arr.size] = text
	arr.size++
}

// InsertAt вставляет элемент на место index.
func (arr *Array) InsertAt(text string, index int) {
	if index < 0 || index > arr.size {
		fmt.Println("такого индекса нету")
		return
	}

	if arr.size == arr.capacity {
		arr.resizeArray()
	}

	// сдвигаем элементы вправо, начиная с последнего,
	// чтобы не затереть соседа справа
	for i := arr.size - 1; i >= index; i-- {
		arr.data[i+1] = arr.data[i]
	}

	arr.data[index] = text
	arr.size++
}

// RemoveAt удаляет элемент с места index.
func (arr *Array) RemoveAt(index int) {
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return
	}

	// сдвигаем элементы влево, удаляемый затирается первым
	for i := index; i < arr.size-1; i++ {
		arr.data[i] = arr.data[i+1]
	}
	arr.size--
}

// SetAt заменяет элемент на месте index.
func (arr *Array) SetAt(text string, index int) {
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return
	}
	arr.data[index] = text
}

// GetAt возвращает элемент с места index.
// При неверном индексе возвращает пустую строку.
func (arr *Array) GetAt(index int) string {
	if index < 0 || index >= arr.size {
		fmt.Println("Нет такого индекса")
		return ""
	}
	return arr.data[index]
}

// PrintArray печатает все элементы, размер и вместимость.
func (arr *Array) PrintArray() {
	for i := 0; i < arr.size; i++ {
		fmt.Print(arr.data[i], " ")
	}
	fmt.Printf("(size=%d, capacity=%d)\n", arr.size, arr.capacity)
}

// Length возвращает количество элементов.
func (arr *Array) Length() int {
	return arr.size
}

// HandleArray выполняет команду над массивом с именем name.
func HandleArray(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "array") {
		return
	}

	arr := NewArray()
	for _, item := range loadStrings(db, name) {
		arr.PushBack(item)
	}

	var index int
	var value string
	switch command {
	case MPUSH:
		fmt.Fscan(ss, &value)
		arr.PushBack(value)
		fmt.Println("->", value)
	case MINSERT:
		fmt.Fscan(ss, &index, &value)
		arr.InsertAt(value, index)
	case MGET:
		fmt.Fscan(ss, &index)
		fmt.Println("->", arr.GetAt(index))
	case MSET:
		fmt.Fscan(ss, &index, &value)
		arr.SetAt(value, index)
	case MDEL:
		fmt.Fscan(ss, &index)
		arr.RemoveAt(index)
	case MLEN:
		fmt.Println("->", arr.Length())
	case MPRINT:
		arr.PrintArray()
	}

	items := make([]string, 0, arr.Length())
	for i := 0; i < arr.Length(); i++ {
		items = append(items, arr.GetAt(i))
	}
	saveStrings(db, name, "array", items)
}

// SaveBinary сохраняет массив в бинарный файл:
// количество элементов, затем строки.
func (arr *Array) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	writeInt(out, arr.size)
	for i := 0; i < arr.size; i++ {
		writeString(out, arr.data[i])
	}
}

// LoadBinary загружает массив из бинарного файла.
// Старые элементы перед загрузкой удаляются.
func (arr *Array) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	arr.size = 0
	count := readInt(in)
	for i := 0; i < count; i++ {
		arr.PushBack(readString(in))
	}
}
