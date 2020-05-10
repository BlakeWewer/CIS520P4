build: sumdiff_openmp.c
	gcc -fopenmp sumdiff_openmp.c -o sumdiff

run: build
	./sumdiff

test: build
	./sumdiff > test_results.txt

clean:
	rm sumdiff