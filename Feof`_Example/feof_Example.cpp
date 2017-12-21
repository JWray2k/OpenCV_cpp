#define _CRT_SECURE_NO_DEPRECATE //https://stackoverflow.com/questions/14386/fopen-deprecated-warning - must come before #includes
/* feof example: byte counter */
#include <stdio.h>
#include <iostream>

void pause()   {
	system("pause"); // Beautiful implementation that doesn't need openCV's waitkey funcion - https://stackoverflow.com/questions/193469/how-to-make-visual-studio-pause-after-executing-a-console-app-in-debug-mode 
}

int main()
{
	FILE * pFile;
	int n = 0;
	pFile = fopen("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", "rb"); // 453058 bytes
	if (pFile == NULL) perror("Error opening file");
	else
	{
		while (fgetc(pFile) != EOF) {
			++n;
		}
		if (feof(pFile)) {
			puts("End-of-File reached.");
			printf("Total number of bytes read: %d\n", n);
		}
		else puts("End-of-File was not reached.");
		fclose(pFile);
	}
	atexit(pause); //added a pause because this file was read so blazingly fast
	return 0;
}

/*
feof (Function)
int feof ( FILE * stream );

Check end-of-file indicator
Checks whether the end-of-File indicator associated with stream is set, returning a value different from zero if it is.

This indicator is generally set by a previous operation on the stream that attempted to read at or past the end-of-file.

Notice that stream's internal position indicator may point to the end-of-file for the next operation, but still, the 
end-of-file indicator may not be set until an operation attempts to read at that point.

This indicator is cleared by a call to clearerr, rewind, fseek, fsetpos or freopen. Although if the position indicator 
is not repositioned by such a call, the next i/o operation is likely to set the indicator again.

Parameters
	- stream
		- Pointer to a FILE object that identifies the stream.

Return Value
	- A non-zero value is returned in the case that the end-of-file indicator associated with the stream is set.
	  Otherwise, zero is returned.



This code opens the file called myfile.txt, and counts the number of characters that it contains by reading all of them 
one by one.The program checks whether the end - of - file was reached, and if so, prints the total number of bytes read.
http://www.cplusplus.com/reference/cstdio/feof/
*/