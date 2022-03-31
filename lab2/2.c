#include<stdio.h>
#include<string.h>
int main(int argc, char **argv)
{
	int sym;
	FILE *f;
	f = fopen(argv[1], "w");
	if (f == NULL){
		perror ("Open file error");
		return 1;
	}
	while ((sym = getchar()) != '\n'){
		fputc(sym, f);
	}
	fclose(f);
	return 0;

}
