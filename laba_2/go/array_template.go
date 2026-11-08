package main

import "fmt"

type Array[T any] struct {
	data     []T
	size     int
	capacity int
}

func initArray[T any](arr *Array[T]) {
	arr.capacity = 4
	arr.data = make([]T, arr.capacity)
	arr.size = 0
}

func resizeArray[T any](arr *Array[T]) {
	newCapacity := arr.capacity * 2
	newData := make([]T, newCapacity)
	for i := 0; i < arr.size; i++ {
		newData[i] = arr.data[i]
	}
	arr.data = newData
	arr.capacity = newCapacity
}

func arrayPushBack[T any](arr *Array[T], value T) {
	if arr.size == arr.capacity {
		resizeArray(arr)
	}
	arr.data[arr.size] = value
	arr.size++
}

func arrayInsertAt[T any](arr *Array[T], index int, value T) {
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

func arrayGetAt[T any](arr *Array[T], index int) (T, bool) {
	var empty T
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return empty, false
	}
	return arr.data[index], true
}

func arraySetAt[T any](arr *Array[T], index int, value T) {
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return
	}
	arr.data[index] = value
}

func arrayRemoveAt[T any](arr *Array[T], index int) {
	if index < 0 || index >= arr.size {
		fmt.Println("такого индекса нету")
		return
	}
	for i := index; i < arr.size-1; i++ {
		arr.data[i] = arr.data[i+1]
	}
	arr.size--
}

func arrayLength[T any](arr *Array[T]) int {
	return arr.size
}

func arrayPrint[T any](arr *Array[T]) {
	for i := 0; i < arr.size; i++ {
		fmt.Print(arr.data[i], " ")
	}
	fmt.Printf("(size=%d, capacity=%d)\n", arr.size, arr.capacity)
}
