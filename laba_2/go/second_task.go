package main

const SIZE = 100
const a = 0.6180339887

type Node[T comparable] struct {
	value T
	next  *Node[T]
}

type hashTable[T comparable] struct {
	table [SIZE]*Node[T]
}

func hashkeyString(key string) int {
	var h uint64 = 0
	for i := 0; i < len(key); i++ {
		h = h*31 + uint64(key[i])
	}
	return int(h % 100)
}

func hashkeyInt(key int) int {
	x := float64(key) * a
	sda := x - float64(int(x))
	if sda < 0 {
		sda += 1
	}
	h := int(100 * sda)
	return h
}

func (t *hashTable[T]) hashFunc(key T) int {
	beta := 0
	switch k := any(key).(type) {
	case string:
		beta = hashkeyString(k) % SIZE
	case int:
		beta = hashkeyInt(k) % SIZE
	}
	return beta
}

func (t *hashTable[T]) search(key T) bool {
	index := t.hashFunc(key)
	current := t.table[index]
	for current != nil {
		if current.value == key {
			return true
		}
		current = current.next
	}
	return false
}

func (t *hashTable[T]) add(key T) {
	if t.search(key) {
		return
	}
	index := t.hashFunc(key)
	newNode := &Node[T]{key, t.table[index]}
	t.table[index] = newNode
}

func (t *hashTable[T]) remove(key T) {
	index := t.hashFunc(key)
	current := t.table[index]
	var prev *Node[T] = nil

	for current != nil {
		if current.value == key {
			if prev == nil {
				t.table[index] = current.next
			} else {
				prev.next = current.next
			}
			return
		}
		prev = current
		current = current.next
	}
}

func (t *hashTable[T]) loadFromJson(arr []T) {
	for _, el := range arr {
		t.add(el)
	}
}

func (t *hashTable[T]) saveToJson() []T {
	arr := []T{}
	for i := 0; i < SIZE; i++ {
		current := t.table[i]
		for current != nil {
			arr = append(arr, current.value)
			current = current.next
		}
	}
	return arr
}

func (t *hashTable[T]) toArray(arr *Array[T]) {
	for i := 0; i < SIZE; i++ {
		current := t.table[i]
		for current != nil {
			arrayPushBack(arr, current.value)
			current = current.next
		}
	}
}
