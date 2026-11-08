package main

import "fmt"

func goodLine(arr *Array[int], userInt int) {
	m := arrayLength(arr)

	for i := 0; i < m; i++ {
		summa := 0
		for j := i; j < m; j++ {
			summa += arr.data[j]

			if summa == userInt {
				fmt.Print("[ ")
				for k := i; k <= j; k++ {
					fmt.Print(arr.data[k], " ")
				}
				fmt.Println("]")
			}
		}
	}
}
