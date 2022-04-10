#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main (int argc, char **argv){
	if (argc == 3){
	char sym;
	int count = 0;
	int strflag = 0;
	int  N = strtol(argv[2], NULL, 10);
	for (int i = 0; argv[2][i] != '\0'; i++){
		if (!isdigit(argv[2][i]))
			strflag = 1;
	}
	if (strflag == 1){
		fprintf(stderr, "Incorrect string format\n");
		return 0;
	}
	FILE *f;
	f = fopen(argv[1], "r");
	if (f == NULL){
		fprintf(stderr, "Open file error");
		return 1;
	}
	while ((N == 0) || (count < N)){
		do {
			sym = fgetc(f);
			if (sym == EOF)
				break;
			printf("%c", sym);
		} while (sym != '\n');
		if (sym == EOF)
			break;
		count++;
		if (count == N){
			printf("\n");
			count = 0;
		}
	}
	fclose(f);
	}
	else{
		fprintf(stderr, "incorrect format: file-name, strings quantity\n");
		return 1;
	}
	
	return 0;
}
