build: sumdiff_base.c
	gcc sumdiff_base.c -o sumdiff

run: build
	./sumdiff

test: build
	./sumdiff > test_results.txt

clean:
	rm sumdiff