.PHONY: all start build done test clean

all: start build done

start:
	@echo "===== BUILDING FUSS-C ====="

build:
	mkdir -p build
	cd build; cmake ../src; make

done:
	@echo "===== DONE BUILDING FUSS-C ====="
	@echo

test: all 
	rm -rdf tmp
	perl test.pl

clean:
	rm -rdf build tmp *.plist

