package structures

import (
	"fmt"
	"io"
	"os"
)

// QNode - узел очереди.
type QNode struct {
	A    string
	Next *QNode
}

// Queue - очередь на односвязном списке: забираем из head, добавляем в tail.
type Queue struct {
	head *QNode
	tail *QNode
}

// NewQueue создает пустую очередь.
func NewQueue() *Queue {
	return &Queue{}
}

// IsEmptyQ проверяет, пуста ли очередь.
func (q *Queue) IsEmptyQ() bool {
	return q.head == nil
}

// Push добавляет элемент в конец очереди.
func (q *Queue) Push(a string) {
	newNode := &QNode{A: a}

	if q.IsEmptyQ() {
		q.head = newNode
		q.tail = newNode
	} else {
		q.tail.Next = newNode
		q.tail = newNode
	}
}

// Pop убирает первый элемент очереди.
func (q *Queue) Pop() {
	if q.IsEmptyQ() {
		return
	}

	q.head = q.head.Next
	if q.IsEmptyQ() {
		q.tail = nil
	}
}

// Top возвращает первый элемент, для пустой очереди - пустую строку.
func (q *Queue) Top() string {
	if q.IsEmptyQ() {
		return ""
	}
	return q.head.A
}

// PrintQ печатает очередь от начала к концу.
func (q *Queue) PrintQ() {
	for current := q.head; current != nil; current = current.Next {
		fmt.Print(current.A, " -> ")
	}
	fmt.Println("nil")
}

// GetHead возвращает первый узел очереди.
func (q *Queue) GetHead() *QNode {
	return q.head
}

// HandleQueue выполняет команду над очередью с именем name.
func HandleQueue(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "queue") {
		return
	}

	q := NewQueue()
	for _, item := range loadStrings(db, name) {
		q.Push(item)
	}

	var value string
	switch command {
	case QPUSH:
		fmt.Fscan(ss, &value)
		q.Push(value)
		fmt.Println("->", value)
	case QPOP:
		if q.IsEmptyQ() {
			fmt.Println("-> очередь пуста")
		} else {
			fmt.Println("->", q.Top())
			q.Pop()
		}
	case QPRINT:
		q.PrintQ()
	}

	items := []string{}
	for current := q.GetHead(); current != nil; current = current.Next {
		items = append(items, current.A)
	}
	saveStrings(db, name, "queue", items)
}

// SaveBinary сохраняет очередь в бинарный файл:
// количество элементов, затем строки от начала к концу.
func (q *Queue) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	count := 0
	for current := q.head; current != nil; current = current.Next {
		count++
	}

	writeInt(out, count)
	for current := q.head; current != nil; current = current.Next {
		writeString(out, current.A)
	}
}

// LoadBinary загружает очередь из бинарного файла.
// Старые элементы перед загрузкой удаляются.
func (q *Queue) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	q.head = nil
	q.tail = nil
	count := readInt(in)
	for i := 0; i < count; i++ {
		q.Push(readString(in))
	}
}
