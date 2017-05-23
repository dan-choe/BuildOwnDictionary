#include "mispelling.h"

char** gentypos(int n, char* word) {
	srand(time(NULL));
	char** typos = (char**) malloc(sizeof(char*) * n);
	int len = strlen(word);
	while(n--) {
		int i = rand() % len;
		char nc = 97 + rand() % 26;
		typos[n] = strdup(word);
		typos[n][i] = nc;
		if(!strcmp(word, typos[n]))
			free(typos[n++]);//++
	}
	return typos;
}


// char* gentyposs(int n, char* word) {

// 	//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>%c\n", '*');

// 	srand(time(NULL));

// 	char* typos = (char*) malloc(sizeof(char*) * n);
// 	char* ptr =typos;

// 	while(n--){
// 		int len = strlen(word);
// 		int i = rand() % len;
// 		i++;
// 		char nc = 97 + rand() % 26;
// 		*(ptr++) = nc;
// 	}

// 		printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>	%s %s\n", typos, word);

// 		//typos[n] = strdup(word);
// 		//typos[n][i] = nc;
// 		// if(!strcmp(word, typos[n]))
// 		// 	free(typos[n++]);//++
// 	//}
// 	return typos;
// }


// char* gentyposs(int n, char* word) {

// 	srand(time(NULL));
// 	char* typos = (char*) malloc(sizeof(char*) * n);
// 	char* ptr =typos;

// 	int len = strlen(word);

// 	int m = strlen(word);
// 	while(m--) {
// 		int i = rand() % len;
// 		char nc = 97 + rand() % 26;
// 		i++;
// 		*(ptr++) = nc;
// 		//typos[n] = strdup(word);
// 		//typos[n][i] = nc;
// 		if(!strcmp(word, typos))
// 			free(typos);//++
// 	}


// 	return typos;
// }
