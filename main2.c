#include <stdio.h>

int main()
{
	int a=120;
	float b=123.0f;
	double c=1222.90;
	void *ptr;
	int *p;

	printf("\nSize of a: %ld",sizeof(a)); // size of int
	printf("\nSize of b: %ld",sizeof(b)); // size of float
	printf("\nSize of c: %ld",sizeof(c)); // size of double
	printf("\nSize of *ptr: %ld",sizeof(*ptr)); // size of void
	printf("\nSize of *p: %ld",sizeof(*p)); // size of int
	printf("\nSize of ptr: %ld",sizeof(ptr)); // size of pointer
        printf("\nSize of p: %ld\n",sizeof(p)); // size of pointer

	return 0;
}
