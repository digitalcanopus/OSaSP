#include <stdio.h>
#include <stdlib.h>
int main (int argc, char **argv){
	char sym;
	int count = 0;
	int  N = strtol(argv[2], NULL, 10);
	if (N < 0){
		perror("Invalid string");
		return 1;
	}
	FILE *f;
	f = fopen(argv[1], "r");
	if (f == NULL){
		perror("Open file error");
		return 1;
	}
	while ((N == 0) || (count < N)){
		do{
			sym = fgetc(f);
			if (sym == EOF)
				break;
			printf("%c", sym);
		}while (sym != '\n');
		if (sym == EOF)
			break;
		count++;
		if (count == N){
			getc(stdin);
			count = 0;
		}
	}
	fclose(f);
	return 0;
}
