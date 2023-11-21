#include <stdio.h>

int a = 1;
int b = 2;
// for testing we use pointers to integers
int *p = &a;
int *q = &b;

void swap_values(int *p, int *q){
	int t;
	t = *p;
	*p = *q;
	*q = t;
}

void swap_pointers(int **ptr1 , int **ptr2){
	int *temp = *ptr1;
	* ptr1 = * ptr2;
	* ptr2 = temp;
}

int main()
{
	printf("Before:\n");
	printf("\taddress of p = %p and q = %p\n", p, q); // prints p = &a and q = &b
	printf("\tvalue of a = %d and b = %d\n", a, b);

	//swap_values( p , q );
	swap_pointers((int**) &p , (int**) &q);

	printf("After:\n");
	printf("\taddress of p = %p and q = %p\n", p, q); // prints p = &b and q = &a
	printf("\tvalue of a = %d and b = %d\n", a, b);

	return 0;
}
