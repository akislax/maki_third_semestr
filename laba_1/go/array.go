package main

import "fmt"

type Array struct {
	data     []string
	size     int
	capacity int
}

func initArray(arr *Array) {
	arr.capacity = 4
	arr.data = make([]string, arr.capacity)
	arr.size = 0
}

func resizeArray(arr *Array) {
	newCapacity := arr.capacity * 2
	newData := make([]string, newCapacity)
	for i := 0; i < arr.size; i++ {
		newData[i] = arr.data[i]
	}
	arr.data = newData
	arr.capacity = newCapacity
}

func arrayPushBack(arr *Array, value string) {
	if arr.size == arr.capacity {
		resizeArray(arr)
	}
	arr.data[arr.size] = value
	arr.size++
}

func arrayInsertAt(arr *Array, index int, value string) {
	if index < 0 || index > arr.size {
		fmt.Println("такого индекса нету")
		return
	}
	if arr.size == arr.capacity {
		resizeArray(arr)
	}
	for i := arr.size - 1; i >= index; i-- {
		arr.data[i+1] = arr.data[i]
	}
	arr.data[index] = value
	arr.size++
}

func arrayGetAt(arr *Array, index int) (string, bool) {
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return "", false
	}
	return arr.data[index], true
}

func arraySetAt(arr *Array, index int, value string) {
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return
	}
	arr.data[index] = value
}

func arrayRemoveAt(arr *Array, index int) {
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return
	}
	for i := index; i < arr.size-1; i++ {
		arr.data[i] = arr.data[i+1]
	}
	arr.size--
}

func arrayLength(arr *Array) int {
	return arr.size
}

func arrayPrint(arr *Array) {
	for i := 0; i < arr.size; i++ {
		fmt.Print(arr.data[i], " ")
	}
	fmt.Printf("(size=%d, capacity=%d)\n", arr.size, arr.capacity)
}