package main

import "fmt"

func absInt(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func similarSubsets(set *hashTable[int]) {
	var arr Array[int]
	initArray(&arr)
	set.toArray(&arr)
	n := arr.size

	bestMask := 0
	bestDiff := -1

	total := 1
	for i := 0; i < n; i++ {
		total = total * 2
	}

	for mask := 0; mask < total; mask++ {
		left := 0
		right := 0
		m := mask

		for i := 0; i < n; i++ {
			if m%2 == 1 {
				left += arr.data[i]
			} else {
				right += arr.data[i]
			}
			m = m / 2
		}

		diff := absInt(left - right)

		if bestDiff == -1 || diff < bestDiff {
			bestDiff = diff
			bestMask = mask
		}
	}

	m := bestMask
	fmt.Print("Первое подмножество: { ")
	for i := 0; i < n; i++ {
		if m%2 == 1 {
			fmt.Print(arr.data[i], " ")
		}
		m = m / 2
	}
	fmt.Println("}")

	m = bestMask
	fmt.Print("Второе подмножество: { ")
	for i := 0; i < n; i++ {
		if m%2 == 0 {
			fmt.Print(arr.data[i], " ")
		}
		m = m / 2
	}
	fmt.Println("}")

	fmt.Println("Разница сумм:", bestDiff)

}
