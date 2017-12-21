#define _CRT_SECURE_NO_DEPRECATE //https://stackoverflow.com/questions/14386/fopen-deprecated-warning - must come before #includes
#include <stdio.h>
int main()
{
	char arr[50];
	int q = 4;
	sprintf(arr, "decimal %d in the array", q);
	printf("[%s] is print from the array\n", arr);
	return 0;
}

/*

sprintf() - I/O Function
	How is "sprintf()" used in C++?

Explanation
	sprintf() is an I/O function that writes the formatted data to an array. This function returns the total number of characters, 
	but on failure a negative value is returned.

Syntax:
	int sprintf ( char * str, const char * format, ... );

The following table lists the type specifier used with sprintf()

	Type				Description
c				Character
d or i			Signed decimal integer
e				Scientific notation using e character
E				Scientific notation using E character
f				Decimal floating point
g				Use the shorter of %e or %f
G				Use the shorter of %E or %f
o				Signed octal
s				String of characters
u				Unsigned decimal integer
x				Unsigned hexadecimal integer
X				Unsigned hexadecimal integer(capital)
p				Pointer address
%				A % followed by another % character will write % to the stream.

*/