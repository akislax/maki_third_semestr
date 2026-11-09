package main

import "fmt"

type HuffNode struct {
	symbol byte
	weight int
	left   *HuffNode
	right  *HuffNode
}

type HuffTree struct {
	codes [256]string
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

	nodes[mini] = nodes[*count-1]
	*count--

	return result
}

func (h *HuffTree) buildTree(text string) {
	var nodes [256]*HuffNode

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

func (h *HuffTree) encode(text string) string {
	result := ""
	for i := 0; i < len(text); i++ {
		result += h.codes[text[i]]
	}
	return result
}

func (h *HuffTree) decode(bits string) string {
	result := []byte{}
	current := h.root

	for i := 0; i < len(bits); i++ {
		if bits[i] == '0' {
			current = current.left
		} else {
			current = current.right
		}

		if current.left == nil && current.right == nil {
			result = append(result, current.symbol)
			current = h.root
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
