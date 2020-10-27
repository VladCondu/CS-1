// Variabilele n si m sunt echivalente cu cele din enunt adica n = numarul de campuri si m = numarul de linii de text

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Functia de citire a delimit, out_del, n, m si fields
void read_1(char *del, char *fields, char *out_del, int *n, int *m) {
	fgets(del, 11, stdin);
	scanf("%d%s%s%d", n, fields, out_del, m);
}

// Functia de citire a liniilor de text
void read_2(char **string, int m) {
    //Citire cele m linii si alocare spatiu necesar
	int i;
	char buffer[201];
	getchar();  // eliberare buffer de '\n' de la ultima citire ca sa nu fie luat de fgets
	for (i = 0; i < m; i++) {
		fgets(buffer, 200, stdin);
		string[i] = strdup(buffer);		
	}
}
// Functie eliberare memorie 
void free_mem(char *fields, char *del, char *out_del, char **string, int *numeric_fields, int m) {
	int i;
	for (i = 0; i < m; ++i) {
		free(string[i]);
		string[i] = NULL;
	}

	free(string);
	string = NULL;
	free(out_del);
	out_del = NULL;
    free(fields);
    fields = NULL;
    free(del);
    del = NULL;	
    free(numeric_fields);
    numeric_fields = NULL;
}

//Functie pentru swap intre 2 variabile
void swap(int *px, int *py) {  
    int temp = *px;  
    *px = *py;  
    *py = temp;  
} 

//Functie pentru transformarea sirului fields din char in int si salvat in numeric_fields
void fields_convert(char *fields, int *numeric_fields, int n) {
	int count = 0;

	// Transforma fiecare field gasit in integer si-l pune intr-un vector de fields
	char *token = strtok(fields, ",");
	while (token != NULL) {
		numeric_fields[count++] = atoi(token);
		token = strtok(NULL, ",");
	}
	
	// Sortare vector de fields (bubble sort)
	int i, j;  
    for (i = 0; i < n-1; i++) {
    	for (j = 0; j < n-i-1; j++) {
        	if (numeric_fields[j] > numeric_fields[j+1]) {
            	swap(&numeric_fields[j], &numeric_fields[j+1]);   	
        	}
    	}
    }    
          
}

/* Functie pentru afisare cu argumentele campurile transformate in int, firstTime in caz ca e prima data cand afiseaza, pos in cazul in care a mai fost afisat
   sirul efectiv si cel de delimitatori de iesire. */
int print(int n, int *numeric_fields, int firstTime, int pos, char *pstr, char *out_del) {
	int i;
	for (i = 0; i < n; ++i) {
		if (pos == numeric_fields[i]) {
			if (firstTime) {
				printf("%s", pstr);
				return 0;
			} else {
				printf("%s%s", out_del, pstr);
				return firstTime;
			}
		}
	}

	return firstTime;
}

/* Functia pentru separarea liniei sirului in campuri atunci cand intalneste delimitatori. Campurile sunt salvate initial in token si apoi intr-un vector de campuri pentru a pastra ordinea */
void cut(char *del, int n, int *numeric_fields, char *out_del, char *string) {
	char *token = strtok(string, del);
	char token_string[200][200];
	int lenght = 0;

	while (token != NULL) {
		strcpy(token_string[lenght++], token); // copierea in vectorul de campuri
		token = strtok(NULL, del);
	}

	int i, firstTime = 1;
	for (i = 0; i < lenght ; i++) {
		firstTime = print(n, numeric_fields, firstTime, i+1, token_string[i], out_del); // pentru fiecare camp se aplica functia print pentru a-l afisa sau nu
	}
}

int main()
{
	int n,m;
	char *del;
	char *fields;
	char *out_del;
	del = (char * ) malloc ( 11 * sizeof(char) );
	fields = (char * ) malloc ( 201 * sizeof(char) );
	out_del = (char * ) malloc ( 11 * sizeof(char) );
	char **arrayOfStrings;
	
	read_1(del, fields, out_del, &n, &m); //citire sirul de delimitatori, out_del, n, m si sirul de fields.
	
	arrayOfStrings = (char **) malloc ( m * sizeof(char*) ); 
	
	read_2(arrayOfStrings, m); //citire numarul de linii de text si liniile de text
	int *numeric_fields = (int *) malloc(100 * sizeof(int) );
	fields_convert(fields, &(*numeric_fields), n); // conversie fields din char in int

	int i;
	for(i = 0; i < m; ++i) {
		cut(del, n, numeric_fields, out_del, arrayOfStrings[i]); // pentru fiecare linie de text se aplica cut
		printf("\n");
	}

	free_mem(fields, del, out_del, arrayOfStrings, numeric_fields, m);
	return 0;
}