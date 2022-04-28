#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int dirWalk(char *path, int nProcesses, char *seq);
int createChildProcesses(int *processNumber, char *fileName, char *seq);

char *getAbsPath (char *relPath){
	if (relPath[0] == '/')
		return relPath;
	else{
		char curDir[PATH_MAX];
		if (!getcwd(curDir, PATH_MAX)){
			fprintf(stderr, "getcwd() fail");
			return NULL;	
		}
		return strcat(strcat(curDir, "/"), relPath);
	}	
}

int dirWalk(char *path, int maxnProcesses, char *seq){
	static int nProcesses = 0;
	int nFilesFound = 0;
	int add_nFilesFound = 0;
	DIR *curDir = opendir(path);
	if (!curDir){
		fprintf(stderr, "opendir() fail at  '%s'\n", path);
		return 4;
	}	
	char *newPath = (char*)malloc(PATH_MAX);
	if (!newPath){
		fprintf(stderr, "allocation error at '%s'", path);
		closedir(curDir);
		return 4;
	}
	struct dirent *dir;
	while (dir = readdir(curDir)){		
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 ) 
			continue;
		strcpy(newPath, path);
		strcat(strcat(newPath, "/"), dir->d_name);		
		if (dir->d_type == DT_DIR) {			
			add_nFilesFound = dirWalk(newPath, maxnProcesses, seq);
			if (add_nFilesFound < 0){
				fprintf(stderr, "dirWalk() fail at '%s'\n", newPath);
				continue;
			}
			nFilesFound += add_nFilesFound;
		}
		else if (dir->d_type == DT_REG){		
			//create process	
			if (nProcesses >= maxnProcesses){
				int wstatus;		
				if (waitpid(-1, &wstatus, 0) == -1){
					fprintf(stderr, "waitpid() fail at '%s'\n", newPath);				
					continue;	
				}				
				else if (WIFEXITED(wstatus))
					if (WEXITSTATUS(wstatus) == 1)
						++nFilesFound;
					else if (WEXITSTATUS(wstatus))
						fprintf(stderr, "child process terminated wrong");			
				--nProcesses;	
			}			
			if ((createChildProcesses(&nProcesses, newPath, seq)) < 0){
				fprintf(stderr, "createChildProcesses() fail at '%s'\n", newPath);
				continue;
			}
		}	
	}		
	free(newPath);
	if (closedir(curDir)){
		fprintf(stderr, "closedir() fail at '%s'\n", path);
		return 4;
	}	
	return nFilesFound;	
}

int findSeq(char *seq, char* fileName, int *byteLook){
	const int sizeSeq = strlen(seq);
	char *buffer = (char *)calloc(sizeSeq, 1);
	if (!buffer){
		fprintf(stderr, "calloc() fail");
		return 3;
	}
	FILE *file = fopen(fileName, "rb");
	if (!file){
		fprintf(stderr, "fopen() fail at %s\n", fileName);
		free(buffer);
		return 3;
	}	
	int nFound = 0;
	long int offset = 0;	
	char flagFirst = 1;
	*byteLook = -1;
	while (!feof(file)){
		if (fseek(file, offset, SEEK_SET))
			fprintf(stderr, "fseek() fail");
		if (flagFirst){		
			*byteLook = fread(buffer, 1, sizeSeq, file);
			flagFirst = 0;
		}
		else{
			fread(buffer, 1, sizeSeq, file);
			++(*byteLook);
		}
		if (strcmp(buffer, seq) == 0)
			++nFound;				
		++offset;				
	} 	
	free(buffer);		
	if (fclose(file)){
		fprintf(stderr, "fclose() fail\n");
		return 3;
	}	
	return nFound;
}

int createChildProcesses(int *processNumber, char *fileName, char *seq){
	pid_t cpid = fork();
	int nFound;
	int nBytesLook;
	switch(cpid){
	case -1: return -1;
	case 0:
		nFound = findSeq(seq, fileName, &nBytesLook);
		if (nFound >= 0)
			printf("pid: %d\tactive processes: %d\t %s \tfound: %d\t viewed bytes: %d\n\n", getpid(), 1 + *processNumber, fileName, nFound, nBytesLook);
		_exit(nFound != 0);			
	default: 
		++(*processNumber);
		return 0;		
	}	
}

int main(int argc, char *argv[]){
	if (argc != 4){
		fprintf(stderr, "incorrect format: bytes sequence, search dir, max quantity of processes");
		return -1;
	}
	char *absPath;
	absPath = getAbsPath(argv[2]);
	if (!absPath){
		fprintf(stderr, "getAbsPath() fail");
		return 1;
	}		
	char *endptr;
	int N;		
	N = strtol(argv[3], &endptr, 10);
	if (errno == ERANGE){
		fprintf(stderr, "incorrect number format");
		return 2;
	}	
	if (endptr[0] != '\0' || endptr == argv[3]){		
		fprintf(stderr, "couldn't convert N");
		return 2;	
	}		
	endptr = NULL;		
	if (N <= 0){
		fprintf(stderr, "max count of children processes must be a positive value");
		return 1;
	}
	int nFilesFound = dirWalk(absPath, N, argv[1]);
	if (nFilesFound < 0){
		fprintf(stderr, "dirWalk() fail");
		return 1;
	}	
	int wstatus;
	for (int i = 0; i < N; ++i)
		if (waitpid(-1, &wstatus, 0) == -1)
			fprintf(stderr, "waitpid() fail at the end");
		else if (WIFEXITED(wstatus))
			if (WEXITSTATUS(wstatus) == 1)
				++nFilesFound;
			else if (WEXITSTATUS(wstatus))			
				fprintf(stderr, "child process terminated wrong");			
	printf("\nfound files quantity: %d\n", nFilesFound);
	
	return 0;
}

