package main

import (
	"fmt"
	"math/rand"
	"strconv"
	"time"
)

func foldSum(key string) int {
	sum := 0
	for i := 0; i < len(key); i += 3 {
		end := i + 3
		if end > len(key) {
			end = len(key)
		}
		part := key[i:end]

		num := 0
		for j := 0; j < len(part); j++ {
			num = num*10 + int(part[j]) - '0'
		}
		sum += num
	}
	return absInt(sum)
}

type NodeSix struct {
	key   string
	value string
	next  *NodeSix
}

type hashTableSix struct {
	table [SIZE]*NodeSix
}

func (t *hashTableSix) hashKeySix(key string) int {
	return foldSum(key) % SIZE
}

func (t *hashTableSix) search(key string) string {
	index := t.hashKeySix(key)
	current := t.table[index]
	for current != nil {
		if current.key == key {
			return current.value
		}
		current = current.next
	}
	return ""
}

func (t *hashTableSix) add(key string, value string) {
	index := t.hashKeySix(key)

	current := t.table[index]
	for current != nil {
		if current.key == key {
			current.value = value
			return
		}
		current = current.next
	}

	newNode := &NodeSix{key, value, t.table[index]}
	t.table[index] = newNode
}

func (t *hashTableSix) remove(key string) {
	index := t.hashKeySix(key)

	current := t.table[index]
	var prev *NodeSix = nil

	for current != nil {
		if current.key == key {
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
	keys1 []string
	vals1 []string
	keys2 []string
	vals2 []string
	used1 []bool
	used2 []bool
}

func newKukushka() *Kukushka {
	k := &Kukushka{}
	k.size = 11
	k.keys1 = make([]string, k.size)
	k.vals1 = make([]string, k.size)
	k.keys2 = make([]string, k.size)
	k.vals2 = make([]string, k.size)
	k.used1 = make([]bool, k.size)
	k.used2 = make([]bool, k.size)
	return k
}

func (k *Kukushka) hash1(key string) int {
	return foldSum(key) % k.size
}

func (k *Kukushka) hash2(key string) int {
	var h uint64 = 0
	for i := 0; i < len(key); i++ {
		h = h*31 + uint64(key[i])
	}
	return int(h % uint64(k.size))
}

func (k *Kukushka) moreplace() {
	oldSize := k.size
	oldKeys1 := k.keys1
	oldVals1 := k.vals1
	oldKeys2 := k.keys2
	oldVals2 := k.vals2
	oldUsed1 := k.used1
	oldUsed2 := k.used2

	k.size = k.size * 2
	k.keys1 = make([]string, k.size)
	k.vals1 = make([]string, k.size)
	k.keys2 = make([]string, k.size)
	k.vals2 = make([]string, k.size)
	k.used1 = make([]bool, k.size)
	k.used2 = make([]bool, k.size)

	for i := 0; i < oldSize; i++ {
		if oldUsed1[i] {
			k.insert(oldKeys1[i], oldVals1[i])
		}
		if oldUsed2[i] {
			k.insert(oldKeys2[i], oldVals2[i])
		}
	}
}

func (k *Kukushka) search(key string) string {
	i1 := k.hash1(key)
	if k.used1[i1] && k.keys1[i1] == key {
		return k.vals1[i1]
	}

	i2 := k.hash2(key)
	if k.used2[i2] && k.keys2[i2] == key {
		return k.vals2[i2]
	}
	return ""
}

func (k *Kukushka) insert(key string, value string) {
	i1 := k.hash1(key)
	if k.used1[i1] && k.keys1[i1] == key {
		k.vals1[i1] = value
		return
	}
	i2 := k.hash2(key)
	if k.used2[i2] && k.keys2[i2] == key {
		k.vals2[i2] = value
		return
	}

	curKey := key
	curVal := value

	for tempt := 0; tempt < MAXTEMPT; tempt++ {
		i1 = k.hash1(curKey)
		if k.used1[i1] == false {
			k.keys1[i1] = curKey
			k.vals1[i1] = curVal
			k.used1[i1] = true
			return
		}
		k.keys1[i1], curKey = curKey, k.keys1[i1]
		k.vals1[i1], curVal = curVal, k.vals1[i1]

		i2 = k.hash2(curKey)
		if k.used2[i2] == false {
			k.keys2[i2] = curKey
			k.vals2[i2] = curVal
			k.used2[i2] = true
			return
		}
		k.keys2[i2], curKey = curKey, k.keys2[i2]
		k.vals2[i2], curVal = curVal, k.vals2[i2]
	}

	k.moreplace()
	k.insert(curKey, curVal)
}

func (k *Kukushka) remove(key string) {
	i1 := k.hash1(key)
	if k.used1[i1] && k.keys1[i1] == key {
		k.used1[i1] = false
		return
	}
	i2 := k.hash2(key)
	if k.used2[i2] && k.keys2[i2] == key {
		k.used2[i2] = false
	}
}

func printFold(key string) {
	sum := 0
	for i := 0; i < len(key); i += 3 {
		end := i + 3
		if end > len(key) {
			end = len(key)
		}
		part := key[i:end]
		if i > 0 {
			fmt.Print("+")
		}
		fmt.Print(part)

		num := 0
		for j := 0; j < len(part); j++ {
			num = num*10 + int(part[j]) - '0'
		}
		sum += num
	}
	fmt.Println(" =", absInt(sum))
}

func compareHash() {
	sizes := []int{1000, 10000, 50000}
	rand.Seed(time.Now().UnixNano())

	fmt.Println("Элементов | Цепочки: вставка, поиск (мс) | Кукушка: вставка, поиск (мс)")

	for _, n := range sizes {
		keys := make([]string, n)
		values := make([]string, n)
		for i := 0; i < n; i++ {
			keys[i] = strconv.Itoa(rand.Intn(1000000))
			values[i] = "value" + strconv.Itoa(i)
		}

		var chain hashTableSix
		cuckoo := newKukushka()

		start := time.Now()
		for i := 0; i < n; i++ {
			chain.add(keys[i], values[i])
		}
		chainInsert := float64(time.Since(start).Microseconds()) / 1000

		start = time.Now()
		for i := 0; i < n; i++ {
			chain.search(keys[i])
		}
		chainSearch := float64(time.Since(start).Microseconds()) / 1000

		start = time.Now()
		for i := 0; i < n; i++ {
			cuckoo.insert(keys[i], values[i])
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
