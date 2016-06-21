from ctypes import cdll

# QUESTION how are errors handled?

# NOTE if your library is in a non-standard location, use the full path
# FIXME there should also be a way to set DYLD_LIBRARY_PATH (on OSX) or LD_LIBRARY_PATH (on Linux),
# but `export DYLD_LIBRARY_PATH=/Users/margold/RC/lib:$DYLD_LIBRARY_PATH` doesn't
# currently work for me on OSX
path_to_lib = '/Users/margold/RC/lib/libcatcount.so'
lib = cdll.LoadLibrary(path_to_lib)

def cat_count():
    return lib.cat_count()
