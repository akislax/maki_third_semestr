package structures

import (
	"math/rand"
	"strconv"
	"testing"
)

// В каждом бенчмарке меряем, сколько времени занимает добавить N элементов,
// для N = 1000 и N = 10000.
var benchSizes = []int{1000, 10000}

func BenchmarkArrayPushBack(b *testing.B) {
	for _, n := range benchSizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				arr := NewArray()
				for j := 0; j < n; j++ {
					arr.PushBack("x")
				}
			}
		})
	}
}

func BenchmarkStackPush(b *testing.B) {
	for _, n := range benchSizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				st := NewStack()
				for j := 0; j < n; j++ {
					st.Push("x")
				}
			}
		})
	}
}

func BenchmarkQueuePush(b *testing.B) {
	for _, n := range benchSizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				q := NewQueue()
				for j := 0; j < n; j++ {
					q.Push("x")
				}
			}
		})
	}
}

// AddTail каждый раз идет до конца списка
func BenchmarkFlistAddTail(b *testing.B) {
	for _, n := range benchSizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				fl := NewFlist()
				for j := 0; j < n; j++ {
					fl.AddTail("x")
				}
			}
		})
	}
}

// PushBack сразу идет в хвост
func BenchmarkDlistPushBack(b *testing.B) {
	for _, n := range benchSizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				dl := NewDlist()
				for j := 0; j < n; j++ {
					dl.PushBack("x")
				}
			}
		})
	}
}

// ключи в случайном порядке, чтобы дерево не вытянулось в линию
func BenchmarkTreeInsert(b *testing.B) {
	for _, n := range benchSizes {
		gen := rand.New(rand.NewSource(42))
		keys := make([]int, n)
		for j := range keys {
			keys[j] = gen.Intn(1000000)
		}

		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				tr := NewTree()
				for _, key := range keys {
					tr.Insert(key)
				}
			}
		})
	}
}

func BenchmarkHashAdd(b *testing.B) {
	for _, n := range benchSizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				ht := NewHashTable()
				for j := 0; j < n; j++ {
					ht.Add(j)
				}
			}
		})
	}
}

func BenchmarkKukushkaInsert(b *testing.B) {
	for _, n := range benchSizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				kt := NewKukushka()
				for j := 0; j < n; j++ {
					kt.Insert(j)
				}
			}
		})
	}
}
