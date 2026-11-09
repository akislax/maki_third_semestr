package main

import (
	"fmt"
	"math/rand"
	"strconv"
	"time"
)

type NodeSix struct {
	value int
	next  *NodeSix
}

type hashTableSix struct {
	table [SIZE]*NodeSix
}

func foldSum(key int) int {
	s := strconv.Itoa(absInt(key))
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

func (t *hashTableSix) hashKeySix(key int) int {
	return foldSum(key) % SIZE
}

func (t *hashTableSix) search(key int) bool {
	index := t.hashKeySix(key)
	current := t.table[index]
	for current != nil {
		if current.value == key {
			return true
		}
		current = current.next
	}
	return false
}

func (t *hashTableSix) add(key int) {
	if t.search(key) {
		return
	}
	index := t.hashKeySix(key)
	newNode := &NodeSix{key, t.table[index]}
	t.table[index] = newNode
}

func (t *hashTableSix) remove(key int) {
	index := t.hashKeySix(key)

	current := t.table[index]
	var prev *NodeSix = nil

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

const MAXTEMPT = 50

type Kukushka struct {
	size  int
	t1    []int
	t2    []int
	used1 []bool
	used2 []bool
}

func newKukushka() *Kukushka {
	k := &Kukushka{}
	k.size = 11
	k.t1 = make([]int, k.size)
	k.t2 = make([]int, k.size)
	k.used1 = make([]bool, k.size)
	k.used2 = make([]bool, k.size)
	return k
}

func (k *Kukushka) moreplace() {
	oldSize := k.size
	old1 := k.t1
	old2 := k.t2
	oldUsed1 := k.used1
	oldUsed2 := k.used2

	k.size = k.size * 2
	k.t1 = make([]int, k.size)
	k.t2 = make([]int, k.size)
	k.used1 = make([]bool, k.size)
	k.used2 = make([]bool, k.size)

	for i := 0; i < oldSize; i++ {
		if oldUsed1[i] {
			k.insert(old1[i])
		}
		if oldUsed2[i] {
			k.insert(old2[i])
		}
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

func (k *Kukushka) search(key int) bool {
	i1 := k.hash1(key)
	if k.used1[i1] && k.t1[i1] == key {
		return true
	}

	i2 := k.hash2(key)
	if k.used2[i2] && k.t2[i2] == key {
		return true
	}
	return false
}

func (k *Kukushka) insert(key int) {
	if k.search(key) {
		return
	}

	cur := key

	for tempt := 0; tempt < MAXTEMPT; tempt++ {
		i1 := k.hash1(cur)

		if k.used1[i1] == false {
			k.t1[i1] = cur
			k.used1[i1] = true
			return
		}

		tmp := k.t1[i1]
		k.t1[i1] = cur
		cur = tmp

		i2 := k.hash2(cur)
		if k.used2[i2] == false {
			k.t2[i2] = cur
			k.used2[i2] = true
			return
		}
		tmp = k.t2[i2]
		k.t2[i2] = cur
		cur = tmp
	}
	k.moreplace()
	k.insert(cur)
}

func (k *Kukushka) remove(key int) {
	i1 := k.hash1(key)
	if k.used1[i1] && k.t1[i1] == key {
		k.used1[i1] = false
		return
	}
	i2 := k.hash2(key)
	if k.used2[i2] && k.t2[i2] == key {
		k.used2[i2] = false
		return
	}
}

func printFold(key int) {
	s := strconv.Itoa(absInt(key))
	sum := 0
	for i := 0; i < len(s); i += 3 {
		end := i + 3
		if end > len(s) {
			end = len(s)
		}
		part := s[i:end]
		if i > 0 {
			fmt.Print("+")
		}
		fmt.Print(part)
		n, _ := strconv.Atoi(part)
		sum += n
	}
	fmt.Println(" =", sum)
}

func compareHash() {
	sizes := []int{1000, 10000, 50000}
	rand.Seed(time.Now().UnixNano())

	fmt.Println("Элементов | Цепочки: вставка, поиск (мс) | Кукушка: вставка, поиск (мс)")

	for _, n := range sizes {
		keys := make([]int, n)
		for i := 0; i < n; i++ {
			keys[i] = rand.Intn(1000000)
		}

		var chain hashTableSix
		cuckoo := newKukushka()

		start := time.Now()
		for i := 0; i < n; i++ {
			chain.add(keys[i])
		}
		chainInsert := float64(time.Since(start).Microseconds()) / 1000

		start = time.Now()
		for i := 0; i < n; i++ {
			chain.search(keys[i])
		}
		chainSearch := float64(time.Since(start).Microseconds()) / 1000

		start = time.Now()
		for i := 0; i < n; i++ {
			cuckoo.insert(keys[i])
		}
		cuckooInsert := float64(time.Since(start).Microseconds()) / 1000

		start = time.Now()
		for i := 0; i < n; i++ {
			cuckoo.search(keys[i])
		}
		cuckooSearch := float64(time.Since(start).Microseconds()) / 1000

		fmt.Printf("%d | %.3f, %.3f | %.3f, %.3f\n", n, chainInsert, chainSearch, cuckooInsert, cuckooSearch)
	}
}
