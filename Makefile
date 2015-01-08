.PHONY: all optimizing non-optimizing clean

all: optimizing non-optimizing

optimizing:
	cd optimizing; make;

non-optimizing:
	cd non-optimizing; make;

clean:
	cd optimizing; make clean;
	cd non-optimizing; make clean;
