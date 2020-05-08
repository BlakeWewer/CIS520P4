build: sumdiff_pthread.c
	gcc -pthread sumdiff_pthread.c -o sumdiff

run: build
	./sumdiff

test: build
	./sumdiff > test_results.txt

clean:
	rm sumdiff