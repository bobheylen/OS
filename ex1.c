#include <stdio.h>
#include <stdlib.h>


typedef struct {
	short day, month;
	unsigned year;
} date_t;
void f( void ) {
	int x, y, z;
	printf("%d %d %d\n", x, y, z );
}
//Deze bovenstaande functie doet niets nuttig, maar doet je stack naar de vaantjes
// In deze oefening moest je dus malloc() gebruiken bij je data
date_t * date_struct( int day, int month, int year ) {
	date_t *dummy;
	dummy->day = (short)day;
	dummy->month = (short)month;
	dummy->year = (unsigned)year;
	return dummy;
}
int main( void ) {
	int day, month, year;
        date_t *d = (date_t*) malloc(sizeof(date_t));
        printf("\nGive day, month, year:");
        scanf("%d %d %d", &day, &month, &year);
        d = date_struct( day, month, year );
        f();
        printf("\ndate struct values: %d-%d-%d\n", d->day, d->month, d->year);
        //free(d);
        return 0;
}
