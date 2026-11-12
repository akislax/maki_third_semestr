package structures

import (
	"fmt"
	"io"
	"os"
	"strconv"
)

// foldSum - хеш методом свертки: число режется на куски по 3 цифры,
// куски складываются.
func foldSum(key int) int {
	if key < 0 {
		key = -key
	}
	s := strconv.Itoa(key)
	sum := 0
	for i := 0; i < len(s); i += 3 {
		end := i + 3
		if end > len(s) {
			end = len(s)
		}
		part, _ := strconv.Atoi(s[i:end])
		sum += part
	}
	return sum
}

const hashSize = 100

// Node - узел цепочки в хеш-таблице.
type Node struct {
	Value int
	Next  *Node
}

// HashTable - хеш-таблица с цепочками.
type HashTable struct {
	table [hashSize]*Node
}

// NewHashTable создает пустую таблицу.
func NewHashTable() *HashTable {
	return &HashTable{}
}

func (ht *HashTable) hashKey(key int) int {
	return foldSum(key) % hashSize
}

// Search проверяет, есть ли ключ в таблице.
func (ht *HashTable) Search(key int) bool {
	for current := ht.table[ht.hashKey(key)]; current != nil; current = current.Next {
		if current.Value == key {
			return true
		}
	}
	return false
}

// Add добавляет ключ в начало цепочки своей ячейки. Повторы не добавляются.
func (ht *HashTable) Add(key int) {
	if ht.Search(key) {
		return
	}
	index := ht.hashKey(key)
	ht.table[index] = &Node{Value: key, Next: ht.table[index]}
}

// Remove удаляет ключ из таблицы.
func (ht *HashTable) Remove(key int) {
	index := ht.hashKey(key)

	var prev *Node
	for current := ht.table[index]; current != nil; current = current.Next {
		if current.Value == key {
			if prev == nil {
				ht.table[index] = current.Next
			} else {
				prev.Next = current.Next
			}
			return
		}
		prev = current
	}
}

// Print печатает непустые ячейки с их цепочками.
func (ht *HashTable) Print() {
	for i := 0; i < hashSize; i++ {
		if ht.table[i] == nil {
			continue
		}
		fmt.Print(i, ": ")
		for current := ht.table[i]; current != nil; current = current.Next {
			fmt.Print(current.Value, " -> ")
		}
		fmt.Println("nil")
	}
}

// GetAll возвращает все ключи таблицы.
func (ht *HashTable) GetAll() []int {
	result := []int{}
	for i := 0; i < hashSize; i++ {
		for current := ht.table[i]; current != nil; current = current.Next {
			result = append(result, current.Value)
		}
	}
	return result
}

const maxTempt = 50

// Kukushka - хеш-таблица кукушки: две таблицы и две хеш-функции.
type Kukushka struct {
	size  int
	t1    []int
	t2    []int
	used1 []bool
	used2 []bool
}

// NewKukushka создает пустую таблицу на 11 ячеек.
func NewKukushka() *Kukushka {
	size := 11
	return &Kukushka{
		size:  size,
		t1:    make([]int, size),
		t2:    make([]int, size),
		used1: make([]bool, size),
		used2: make([]bool, size),
	}
}

func (k *Kukushka) hash1(key int) int {
	return foldSum(key) % k.size
}

func (k *Kukushka) hash2(key int) int {
	h := key % k.size
	if h < 0 {
		h += k.size
	}
	return h
}

// moreplace увеличивает таблицы в два раза и заново вставляет все ключи.
func (k *Kukushka) moreplace() {
	oldSize := k.size
	old1, old2 := k.t1, k.t2
	oldUsed1, oldUsed2 := k.used1, k.used2

	k.size *= 2
	k.t1 = make([]int, k.size)
	k.t2 = make([]int, k.size)
	k.used1 = make([]bool, k.size)
	k.used2 = make([]bool, k.size)

	for i := 0; i < oldSize; i++ {
		if oldUsed1[i] {
			k.Insert(old1[i])
		}
		if oldUsed2[i] {
			k.Insert(old2[i])
		}
	}
}

// Search проверяет, есть ли ключ в одной из двух таблиц.
func (k *Kukushka) Search(key int) bool {
	i1 := k.hash1(key)
	if k.used1[i1] && k.t1[i1] == key {
		return true
	}
	i2 := k.hash2(key)
	return k.used2[i2] && k.t2[i2] == key
}

// Insert вставляет ключ. Если место занято, старый ключ выталкивается
// в другую таблицу. Если вытеснения идут по кругу, таблицы расширяются.
func (k *Kukushka) Insert(key int) {
	if k.Search(key) {
		return
	}

	cur := key
	for tempt := 0; tempt < maxTempt; tempt++ {
		i1 := k.hash1(cur)
		if !k.used1[i1] {
			k.t1[i1] = cur
			k.used1[i1] = true
			return
		}
		k.t1[i1], cur = cur, k.t1[i1]

		i2 := k.hash2(cur)
		if !k.used2[i2] {
			k.t2[i2] = cur
			k.used2[i2] = true
			return
		}
		k.t2[i2], cur = cur, k.t2[i2]
	}
	k.moreplace()
	k.Insert(cur)
}

// Remove удаляет ключ из таблицы.
func (k *Kukushka) Remove(key int) {
	i1 := k.hash1(key)
	if k.used1[i1] && k.t1[i1] == key {
		k.used1[i1] = false
		return
	}
	i2 := k.hash2(key)
	if k.used2[i2] && k.t2[i2] == key {
		k.used2[i2] = false
	}
}

// Print печатает занятые ячейки обеих таблиц.
func (k *Kukushka) Print() {
	fmt.Print("t1: ")
	for i := 0; i < k.size; i++ {
		if k.used1[i] {
			fmt.Printf("[%d]=%d ", i, k.t1[i])
		}
	}
	fmt.Print("\nt2: ")
	for i := 0; i < k.size; i++ {
		if k.used2[i] {
			fmt.Printf("[%d]=%d ", i, k.t2[i])
		}
	}
	fmt.Println()
}

// GetAll возвращает все ключи из обеих таблиц.
func (k *Kukushka) GetAll() []int {
	result := []int{}
	for i := 0; i < k.size; i++ {
		if k.used1[i] {
			result = append(result, k.t1[i])
		}
	}
	for i := 0; i < k.size; i++ {
		if k.used2[i] {
			result = append(result, k.t2[i])
		}
	}
	return result
}

// HandleHash выполняет команду над таблицей с цепочками с именем name.
func HandleHash(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "hash") {
		return
	}

	ht := NewHashTable()
	for _, key := range loadInts(db, name) {
		ht.Add(key)
	}

	var value int
	switch command {
	case HADD:
		fmt.Fscan(ss, &value)
		ht.Add(value)
		fmt.Println("->", value)
	case HDEL:
		fmt.Fscan(ss, &value)
		ht.Remove(value)
	case HFIND:
		fmt.Fscan(ss, &value)
		if ht.Search(value) {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case HPRINT:
		ht.Print()
	}

	saveInts(db, name, "hash", ht.GetAll())
}

// HandleKukushka выполняет команду над таблицей кукушки с именем name.
func HandleKukushka(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "kukushka") {
		return
	}

	kt := NewKukushka()
	for _, key := range loadInts(db, name) {
		kt.Insert(key)
	}

	var value int
	switch command {
	case KADD:
		fmt.Fscan(ss, &value)
		kt.Insert(value)
		fmt.Println("->", value)
	case KDEL:
		fmt.Fscan(ss, &value)
		kt.Remove(value)
	case KFIND:
		fmt.Fscan(ss, &value)
		if kt.Search(value) {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case KPRINT:
		kt.Print()
	}

	saveInts(db, name, "kukushka", kt.GetAll())
}

// SaveBinary сохраняет таблицу в бинарный файл: количество ключей, затем ключи.
// Номер ячейки хранить не нужно - при загрузке Add сам посчитает хеш.
func (ht *HashTable) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	keys := ht.GetAll()
	writeInt(out, len(keys))
	for _, key := range keys {
		writeInt(out, key)
	}
}

// LoadBinary загружает таблицу из бинарного файла.
// Старые ключи перед загрузкой удаляются.
func (ht *HashTable) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	ht.table = [hashSize]*Node{}
	count := readInt(in)
	for i := 0; i < count; i++ {
		ht.Add(readInt(in))
	}
}

// SaveBinary сохраняет таблицу кукушки так же: количество ключей, затем ключи.
func (k *Kukushka) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	keys := k.GetAll()
	writeInt(out, len(keys))
	for _, key := range keys {
		writeInt(out, key)
	}
}

// LoadBinary загружает таблицу кукушки из бинарного файла.
// Старые ключи перед загрузкой удаляются.
func (k *Kukushka) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	*k = *NewKukushka()
	count := readInt(in)
	for i := 0; i < count; i++ {
		k.Insert(readInt(in))
	}
}
