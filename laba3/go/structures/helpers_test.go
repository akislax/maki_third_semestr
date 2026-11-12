package structures

import (
	"io"
	"os"
	"strings"
)

// captureOutput перехватывает все, что функция f печатает в консоль,
// и возвращает это строкой. Нужна, чтобы проверять печать
// и чтобы сообщения об ошибках не мусорили в выводе тестов.
func captureOutput(f func()) string {
	old := os.Stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	f()

	w.Close()
	os.Stdout = old
	out, _ := io.ReadAll(r)
	return string(out)
}

// args превращает аргументы команды в поток, так же как это делает main.
func args(s string) *strings.Reader {
	return strings.NewReader(s)
}
