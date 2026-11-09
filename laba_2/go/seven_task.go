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

	value := c.values.data[index]
	c.moveToEnd(index)
	return value
}

func (c *LRUCache) set(key int, value int) {
	index := c.find(key)

	if index != -1 {
		c.values.data[index] = value
		c.moveToEnd(index)
		return
	}

	if c.keys.size == c.mesto {
		arrayRemoveAt(&c.keys, 0)
		arrayRemoveAt(&c.values, 0)
	}

	arrayPushBack(&c.keys, key)
	arrayPushBack(&c.values, value)
}
