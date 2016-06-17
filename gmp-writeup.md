# Adding pkg-config support to GMP (or: get GMP recognized by aqbanking)

TL;DR if you compile against libraries installed by brew, export the
```
CFLAGS="$CFLAGS -I/usr/local/include"
LDFLAGS="$LDFLAGS -L/usr/local/lib"
```
user environmental variables.

0. (pkg-config file for picking up gmp was installed manually as per https://gist.github.com/margold/73e90e1a564c55bbe750477bb9c78abd)

1. Starting with `auto.conf`, which includes gmp.h:
- build will fail with "gmp.h not found"
- when looking at what make runs, the includes for gmp are missing

So include and lib paths have to be specified in Makefile.

2. Commenting out `make distcheck`, running `auto.conf` again, and including the following into Makefile manually:
`INCLUDES = -I/usr/local/Cellar/gmp/6.1.0/include` (after DEFAULT_INCLUDES)
and
`LDFLAGS = -L/usr/local/Cellar/gmp/6.1.0/lib -lgmp` (LDFLAGS was already present but empty)
it works:
```
 margold@home-macbook ~/RC/c-stack/test/autodir $ make
/Applications/Xcode.app/Contents/Developer/usr/bin/make  all-am
gcc -DHAVE_CONFIG_H -I. -I/usr/local/Cellar/gmp/6.1.0/include    -g -O2 -MT test.o -MD -MP -MF .deps/test.Tpo -c -o test.o test.c
mv -f .deps/test.Tpo .deps/test.Po
gcc  -g -O2  -L/usr/local/Cellar/gmp/6.1.0/lib -lgmp -o test test.o

margold@home-macbook ~/RC/c-stack/test/autodir $ ./test
Adacat <3
```

So configure+Makefile.am (the files generating the Makefile) are not correctly generating variables having to do with headers (one of DEFAULT_INCLUDES/INCLUDES/CFLAGS/AM_CFLAGS) and libraries (one of LDFLAGS/AM_LDFLAGS/LDLIBS/LDADD).

3. What things can we set that have to do with libraries in configure.ac and Makefile.am?

In configure.ac,
we can check if the system already knows where the library is, so we can use #ifdefs in code, or notify the user that a required library is missing, or ask the user to tell the system where the library is by modifying CFLAGS and LIBFLAGS environmental variables:
`AC_CHECK_LIB([gmp],[mpz_clear])` sets a #define statement in config.h:
`#undef HAVE_LIBGMP` or `#define HAVE_LIBGMP 1`.
You can also stop the process if the library is not found and show an error message to the user:
`AC_CHECK_LIB([gmp], [mpz_clear], [], [AC_MSG_ERROR([unable to find the mpz_clear() function of gmp library (or the whole library). Please modify the CFLAGS and LDFLAGS environmental variables to specify the right -I and -L flags.)])])`
Or use AC_SEARCH_LIBS instead of AC_CHECK_LIB (see https://autotools.io/autoconf/finding.html).
But we can't find a library the system doesn't know about.

So that's not it.

In Makefile.am:
If we feel comfortable assuming that other users will have pkg-config installed, we can create the CFLAGS and LDFLAGS variables in Makefile.am:
```
AM_CFLAGS=`pkg-config --cflags gmp`
AM_LDFLAGS=`pkg-config --libs gmp`
```

then it works:
 ```
 margold@home-macbook ~/RC/c-stack/test/autodir $ make
/Applications/Xcode.app/Contents/Developer/usr/bin/make  all-am
gcc -DHAVE_CONFIG_H -I.    `pkg-config --cflags gmp` -g -O2 -MT test.o -MD -MP -MF .deps/test.Tpo -c -o test.o test.c
mv -f .deps/test.Tpo .deps/test.Po
gcc `pkg-config --cflags gmp` -g -O2 `pkg-config --libs gmp`  -o test test.o

margold@home-macbook ~/RC/c-stack/test/autodir $ ./test
Adacat <3
```

Which is one way to let Autotools know where to find this library. You can also specify these environmental variables in other ways, such as:
./configure LDFLAGS=-L... CFLAGS=-I...

4. Systematizing further:
Brew will actually link the libraries, headers, and many types of special files (such as pkg-config files) it installs in /usr/local/Cellar/ to the appropriate locations in /usr/local/ (/usr/local/lib, /usr/local/include, and so on). Run brew install in verbose mode to see the symbolic links it creates.
(Code making the symlinks: https://github.com/Homebrew/brew/blob/b2c9625d780277f021c63e21cac4a7c954170784/Library/Homebrew/keg.rb#L302).
This way programs working with the special types of files (such as pkg-config) can find them. It also allows us to set the user environmental variables somewhere persistent (like your .bash_profile, .bashrc, or .profile):
```
CFLAGS="$CFLAGS -I/usr/local/include"
LDFLAGS="$LDFLAGS -L/usr/local/lib"
```
Likewise for other non-standard locations you (or your package manager) install your libraries to.

You will still need to tell the compiler to link the gmp library eg. by adding `AM_LDFLAGS=-lgmp` to Makefile.am.

Then it works:
```
margold@home-macbook ~/RC/c-stack/test/autodir $ make
/Applications/Xcode.app/Contents/Developer/usr/bin/make  all-am
gcc -DHAVE_CONFIG_H -I.     -I/usr/local/include -MT test.o -MD -MP -MF .deps/test.Tpo -c -o test.o test.c
mv -f .deps/test.Tpo .deps/test.Po
gcc  -I/usr/local/include -lgmp -L/usr/local/lib -o test test.o  -lgmp
margold@home-macbook ~/RC/c-stack/test/autodir $ ./test
Adacat <3
```

5. Aftermath:
Why does the user need to set LDFLAGS/CFLAGS?
From the autoconf documentation: "...the package itself should never set a user variable..." "CFLAGS is one such variable." (http://stackoverflow.com/questions/517580/library-resolution-with-autoconf) ???
