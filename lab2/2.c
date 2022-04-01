#include<stdio.h>
#include<string.h>
#define EXIT 27
int main(int argc, char **argv)
{	
	FILE *f;
	if (argc == 2){
		int sym;
		f = fopen(argv[1], "w");
		if (f == NULL){
			fprintf (stderr, "Open file error");
			return 1;
		}
		while ((sym = getchar()) != EXIT){
			fputc(sym, f);
		}
			
	}
	else {
		fprintf(stderr, "incorrect format: file-name\n");
		return 1;
	}
	fclose(f);	
	return 0;
}
