#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(){
	int MAX=100;
	char first[MAX];
	char second[MAX];
	char name[MAX];
	char str[MAX];
	int birthyear;

	printf("Enter first name: ");
	scanf("%99s", first);
	printf("Enter last name: ");
        scanf("%99s", second);
	//Without concat
	printf("Entered name: %s %s\n",first,second);
	//With concat
	strcpy(name,first);
	strcat(name,second);
	printf("Entered name: %s\n",name);


	int j=0;
	char ch;
	while(second[j] > 1){
		ch = second[j];
		str[j] = toupper(ch);
		j++;
	}
	printf("Capital second is %s.\n", str);

	// comparing strings second and str
	int result;
	result = strcmp(second, str);
	printf("strcmp(second, str) = %d\n", result);
	// result = 0 if strings are equal
	// result > 0 if the first non-matching character in second is greater (in ASCII) than that of str.
	// result < 0 if the first non-matching character in str is lower (in ASCII) than that of second.

	printf("Enter your birthyear: ");
        scanf("%d", &birthyear);
        printf("You where born in %d\n", birthyear);

	snprintf(name,MAX,"%s %s %d\n",first,second,birthyear);
	printf("%s",name);

	sscanf(name,"%s %s %d",first,second,&birthyear);
	printf("%s",first);
	printf("%s",second);
	printf("%d",birthyear);
}
