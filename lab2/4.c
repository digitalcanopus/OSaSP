#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
int main (int argc, char **argv){
	if (argc == 3){
	FILE *f1, *f2;
	char sym;
	struct stat buff;
	f1 = fopen(argv[1], "r+");
	if (f1 == NULL){
		fprintf(stderr, "Open copy file error");
		return 1;
	}
	f2 = fopen(argv[2], "w+");
	if (f2 == NULL){
		fprintf(stderr, "Open paste file error");
		fclose(f1);
		return 2;
	}
	stat(argv[1], &buff);
	chmod(argv[2], buff.st_mode);
	while ((sym = getc(f1)) != EOF){
		putc(sym, f2);
	}
	fclose(f1);
	fclose(f2);
	}
	else {
		fprintf(stderr, "incorrect format: copy file, paste file\n");
		return 1;
	}
	return 0;
}
