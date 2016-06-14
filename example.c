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

	return 0;
}
