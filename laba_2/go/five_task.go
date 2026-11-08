package main

import "fmt"

type HuffNode struct {
	symbol byte
	weight int
	left   *HuffNode
	right  *HuffNode
}

type HuffTree struct {
	codes [256]string // это хранение 0 и 1 для дерева
	root  *HuffNode
}

func takesMin(nodes []*HuffNode, count *int) *HuffNode {
	mini := 0

	for i := 1; i < *count; i++ {
		if nodes[i].weight < nodes[mini].weight {
			mini = i
		}
	}

	result := nodes[mini]
	// запомнили самый маленький

	// затерли
	nodes[mini] = nodes[*count-1]
	*count--

	return result
}

// count - сколько узлов!!!!!
func (h *HuffTree) buildTree(text string) {
	var nodes [256]*HuffNode // byte принимает от 0 до 255 значений поэтому 256

	count := 0

	for i := 0; i < len(text); i++ {
		c := text[i]
		found := false

		for j := 0; j < count; j++ {
			if nodes[j].symbol == c {
				nodes[j].weight++
				found = true
				break
			}
		}

		if found == false {
			nodes[count] = &HuffNode{c, 1, nil, nil}
			count++
		}
	}

	for count > 1 {
		first := takesMin(nodes[:], &count)
		second := takesMin(nodes[:], &count)

		papa := &HuffNode{0, first.weight + second.weight, first, second}

		nodes[count] = papa
		count++
	}

	h.root = nodes[0]
	h.buildCodes(h.root, "")
}

// подаем корень и пустое значение, потому что у корня путь ""
func (h *HuffTree) buildCodes(node *HuffNode, code string) {
	if node == nil {
		return
	}

	if node.left == nil && node.right == nil {
		h.codes[node.symbol] = code
		return
	}

	h.buildCodes(node.left, code+"0")
	h.buildCodes(node.right, code+"1")
}

// берет из таблицы codes значения и просто выводит результат в виде 0101010
func (h *HuffTree) encode(text string) string {
	result := ""
	for i := 0; i < len(text); i++ {
		result += h.codes[text[i]]
	}
	return result
}

// Стоишь в корне.
// Читаешь бит: 0 - шаг влево, 1 - шаг вправо.
// Попал в букву - записал её и вернулся в корень.
func (h *HuffTree) decode(bits string) string {
	result := []byte{}
	current := h.root // стоим в корне

	for i := 0; i < len(bits); i++ {
		// шаг по дереву
		if bits[i] == '0' {
			current = current.left // 0 влево
		} else {
			current = current.right
		}

		// дошли до буквы?
		if current.left == nil && current.right == nil {
			result = append(result, current.symbol) // записали букву
			current = h.root                        // вернулись в корень
		}
	}
	return string(result)
}

func (h *HuffTree) printTable() {
	for i := 0; i < 256; i++ {
		if h.codes[i] != "" {
			fmt.Println(string([]byte{byte(i)}), ":", h.codes[i])
		}
	}
}
