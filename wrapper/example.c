#include <catcount.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int count = cat_count();
	char cat1[] = "Alan";
	char cat2[] = "Adacat";
	printf("%s and %s are %d cats <3\n", cat1, cat2, count);
	return 0;
}
