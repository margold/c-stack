Call a C library function from Python

### Build a library
Alternatively: use one of the standard C library functions that Python already wraps, that has a simple signature, such as `system()`.

- Write Hello World-type code (see cat_count.c)
- Create a header (see catcount.h)
- Create a shared library
    ```
    gcc -o catcount.o -c cat_count.c
    gcc -shared -o libcatcount.so catcount.o
    ```
- Move the created files to lib/ and include/ directories in your project directory:
    ```
    mv libcatcount.so ~/RC/lib/
    cp catcount.h ~/RC/include/
    ```

### Test your library from C
- Write more Hello World-type code using the library (see `example.c`)
- Compile your code (`make`)
- Run your code. Set the path to your library if it's in a non-standard location:
    ```
    export DYLD_LIBRARY_PATH=/Users/margold/RC/lib:$DYLD_LIBRARY_PATH
    ./example
    ```
    (use `LD_LIBRARY_PATH` on Linux)

### Call the library function from Python
- Write a module wrapping your library in Python (see `wrapper.py`)
- Call the module from a python script (see `example.py`) or the Python interpreter
