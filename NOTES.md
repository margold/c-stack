# Questions
- what does it mean when brew installs a C library? ( "as C authors, the package manager is a key means by which we can get libraries for folding into our work") How does it play with apple-installed libraries of apps? (like gcc or c libraries) How do you even install C libraries on OSX? (`brew search libcurl` yields nothing)
- difference between terminal (eg. mintty on Windows) and shell (eg. bash)

# C "stack"
- compiler, POSIX tools, packet manager (for (ISO (?) standard or not) libraries and more POSIX tools)

## Compiling
1. set compiler flags
2. list libraries to link to (static libraries are linked by the compilerby copying their contents into the final executable, shared libraries are linked at runtime). (what is the linker exactly? is it a separate tool? if so, why do we say "the compiler is linking the (static) library")
3.

## Where are libraries installed
"the usual places"
- standard vendor directory
- standard directory for custom local packages (/usr/local); home directories
other places
-
Check for where libraries are installed with `find /usr -name 'lib*'`

### Env variables
CPATH, C_INCLUDE_PATH, LIBRARY_PATH, etc
gcc on Linux and gcc on OSX use different variables

### Passing library paths to compiler and linker (runtime)
-Lpath (compile time) and -Rpath (runtime)
LD_LIBRARY_PATH (Linux/Cygwin) or DYLD_LIBRARY_PATH(OSX): (additional) search path for shared libraries at runtime

# Tools
- See what libraries a program or dynamically linked library depends on:
• Cygwin: cygcheck libxx.dll
• Linux: ldd libxx.so
• Mac: otool -L libxx.dylib
- cscope (http://cscope.sourceforge.net/): according to Ouz, de-facto standard (b/c only?) C project browsing tool (i.e., "better grep")
- gprof (part of binutils; http://www.gnu.org/software/binutils/binutils.html) apparently not supported on OSX? use dtrace instead.

# Autotools
./configure && make && (sudo) make install
./configure takes certain configuration flags, such as `--prefix`

Autoscan => Autoconf => Automake (&Libtool in background)


# Misc Tools
- `env`: list all env variables known to the shell
- `ack`/`ag`: faster `grep` (installed with brew)

# Various info
- Cygwin: run by Red Hat (?). Uses a C library providing POSIX functions on Windows (cygwin1.dll apparently?). The library hides some differences between Windows and POSIX systems (eg. unified file system vs distinct drives). Tools that are used on POSIX systems (ls, bash, grep, make, etc) are compiled by linking to this library. A package manager is allowing the user to install all or a subset of these tools.
- POSIX: specifies how the shell, a set of commands (ls, grep, ...), and a set of C libraries should work; specifies a set of commands and libraries that needs to be available to the developer on the system.

# Finding libraries with pkg-config
- pkg-config is looking for `libname.pc` files
-

## Brew-installed libraries
apparently it matters if brew-installed pkg-config is used (??)
```
margold@home-macbook ~/RC/aqbanking-5.6.10 $ ls -lh `which pkg-config`
lrwxr-xr-x  1 margold  brew    42B Jun  6 16:10 /usr/local/bin/pkg-config -> ../Cellar/pkg-config/0.29.1/bin/pkg-config
```

1. when it works:
```
margold@home-macbook ~ $ pkg-config --libs libpng
-L/usr/local/Cellar/libpng/1.6.17/lib -lpng16

# .pc file was installed by brew
margold@home-macbook ~ $ brew ls --verbose libpng | grep pc
/usr/local/Cellar/libpng/1.6.17/lib/pkgconfig/libpng.pc
/usr/local/Cellar/libpng/1.6.17/lib/pkgconfig/libpng16.pc
```

2. when it doesn't
```
margold@home-macbook ~ $ pkg-config --libs gmp
Package gmp was not found in the pkg-config search path.
Perhaps you should add the directory containing `gmp.pc'
to the PKG_CONFIG_PATH environment variable
No package 'gmp' found

# no .pc file was installed by brew
brew ls --verbose gmp | grep pc
```
This is because gmp project doesn't include a .pc file (see repository: https://gmplib.org/repo/gmp-6.1/file/tip)
Solutions:
- register gmp manually with pkg-config (eg. with https://gist.github.com/douglas-vaz/7399155 and https://gist.github.com/douglas-vaz/7397347)
- add linker flags manually
- create gmp.pc file in brew formula .rb file? (https://github.com/Homebrew/homebrew-core/blob/master/Formula/gmp.rb)
