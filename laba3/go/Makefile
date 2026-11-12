build:
	go build -o laba3go .

fmt:
	gofmt -w .

lint:
	go vet ./...
	gofmt -l .

test:
	go test ./structures -v

coverage:
	go test ./structures -coverprofile=coverage.out
	go tool cover -func=coverage.out | tail -1
	go tool cover -html=coverage.out -o coverage.html

bench:
	go test ./structures -bench=. -run=^$$

.PHONY: build fmt lint test coverage bench
