#include <stdlib.h>
#include <gsl/gsl_cdf.h>
#include <stdio.h>  // paste stdio.h (declaration for printf) here
					// the linker will need to find stdio library (at which step?)
					// it will know to include the library b/c of -lc flag (for including libc, implicit)

int main(int argc, char *argv[])
{
	int count = 2;
	char cat1[] = "Alan";
	char cat2[] = "Adacat";
	printf("There are %d cats: ", count);
	printf("%s and %s <3\n", cat1, cat2);
	printf("Also, a gsl test: %g\n", gsl_cdf_gaussian_P(-1.96, 1));

	char *home = getenv("HOME");
	printf("And a getenv test: home is %s\n", home);

	return 0;
}
