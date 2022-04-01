#include <stdio.h>
#include <dirent.h>
int entdir(char const *name){
	int i = 1;
	DIR *dir = opendir(name);
	if (!name){
		fprintf(stderr, "Open directory trouble");
		return 1;
	}
	struct dirent *d;
	while ((d = readdir(dir))){
		printf("%d. %s\n", i++, d->d_name);
	}
	closedir(dir);
	return 0;
}
int main(int argc, char **argv){
	printf("Current directory: \n");
	entdir(".");
	printf("Root directory: \n");
	entdir("/");
	return 0;
}
