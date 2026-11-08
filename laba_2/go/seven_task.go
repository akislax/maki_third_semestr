package main

type LRUCache struct {
	mesto  int
	keys   Array[int]
	values Array[int]
}

func newLRUCache(capacity int) *LRUCache {
	c := &LRUCache{}
	c.mesto = capacity
	initArray(&c.keys)
	initArray(&c.values)
	return c
}

func (c *LRUCache) find(key int) int {
	for i := 0; i < c.keys.size; i++ {
		if c.keys.data[i] == key {
			return i
		}
	}
	return -1
}

func (c *LRUCache) moveToEnd(index int) {
	k := c.keys.data[index]
	v := c.values.data[index]

	arrayRemoveAt(&c.keys, index)
	arrayRemoveAt(&c.values, index)

	arrayPushBack(&c.keys, k)
	arrayPushBack(&c.values, v)
}

func (c *LRUCache) get(key int) int {
	index := c.find(key)
	if index == -1 {
		return -1
	}

	value := c.values.data[index] // взяли значение, пока знаем где оно
	c.moveToEnd(index)            // переставили
	return value                  // вернули, что взяли
}

func (c *LRUCache) set(key int, value int) {
	index := c.find(key)

	// ключ уже есть
	if index != -1 {
		c.values.data[index] = value // новое значение
		c.moveToEnd(index)           // стал свежим
		return
	}

	// ключа нет, а места нет
	if c.keys.size == c.mesto {
		arrayRemoveAt(&c.keys, 0) // самый старый - на месте 0
		arrayRemoveAt(&c.values, 0)
	}

	// добавляем в конец
	arrayPushBack(&c.keys, key)
	arrayPushBack(&c.values, value)
}
