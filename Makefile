# -g will generate .dSym debug symbols

# -03: highest (?) optimization level. if result is hard to follow, use -00 for no optimization. Doesn't work with cc?
# -Werror: treat warnings as errors
P=example
OBJECTS=
CFLAGS = -g -Wall -Werror
LDLIBS=
CC=cc

$(P): $(OBJECTS)

clean:
	rm -f example

something:
	# -I adds the given path to the include search path (header files)
	# -L adds the given path to the library search path
	# order of linked libraries matter - dependencies should go first
	gcc -I/usr/local/include example.c -o example -L/usr/local/lib -lc  $CFLAGS  # -lc is actually implicit and doesnt't need to be specified
