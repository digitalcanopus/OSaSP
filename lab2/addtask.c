#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

int dirnum = 0;
int filnum = 0;

void printinfo(char *path);

char* concat(char *s1, char *s2){
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	char *result = malloc(len1 + len2 + 1);
	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);
	return result;
}


void dirfind(char *directory, char *searchname){
	DIR *dir;
	struct dirent *ent;
	if (directory[strlen(directory) - 1] != '/')
		directory = concat(directory, "/");
	if ((dir = opendir(directory)) != NULL)
		while ((ent = readdir(dir)) != NULL){
			if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, ".."))){
				if (opendir(concat(directory, ent->d_name)) != NULL){
					dirnum++;
					dirfind(concat(directory, ent->d_name), searchname);
				}
				else
					filnum++;
			}
			if (!strcmp(ent->d_name, searchname)){
				printf("path: %s\n", directory);
				printinfo(concat(directory, ent->d_name));
			}
		}
	closedir(dir);
}

void printinfo(char *path){
	struct stat buf = { 0 };
	char access_rights[9];
	int i;
	stat(path, &buf);
	struct tm * timeinfo;
	printf("size: %lu bytes\n", buf.st_size);
	timeinfo = localtime(&buf.st_mtime);
	printf("time: %s", asctime(timeinfo));
	for (i = 0; i < 9; i += 3){
		access_rights[8 - i] = buf.st_mode & (0x1 << i) ? 'x' : '-';
		access_rights[8 - (i + 1)] = buf.st_mode & (0x1 << (i + 1)) ? 'w' : '-';
		access_rights[8 - (i + 2)] = buf.st_mode & (0x1 << (i + 2)) ? 'r' : '-';
	}
	printf("access rights: %s\n", access_rights);
	printf("inode: %lu\n\n", buf.st_ino);
}

int main(int argc, char *argv[]){
	if (argc == 3){
		dirfind(argv[1], argv[2]);
		printf("viewed directories quantity: %d\nviewed files quantity: %d\n", dirnum, filnum);
	}
	else{
		printf("incorrect format\n");
		return 1;
	}
	return 0;
}
