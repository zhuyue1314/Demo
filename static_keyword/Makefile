all: static_inherited_class static_fork

static_inherited_class: src/static_inherited_class.cc
	g++ src/static_inherited_class.cc -o static_inherited_class

static_fork: src/static_fork.cc
	g++ src/static_fork.cc -o static_fork

clean:
	rm -f static_inherited_class
	rm -f static_fork
