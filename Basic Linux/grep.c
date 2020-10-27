#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//citire si alocare
void read(int n_linii, char **arrayOfStrings) {
	
	// buffer pentru citirea si alocarea cu dimensiunea aferenta a liniilor textului
    char buffer[201];
    fgets(buffer, 2, stdin);

    //Citire cele n linii si alocare spatiu necesar
	int i;
	for (i = 0; i < n_linii; i++) {
	fgets(buffer, 200, stdin);

	if(buffer[strlen(buffer) - 1] == '\n') {
		buffer[strlen(buffer) - 1] = '\0';
	}
	
	arrayOfStrings[i] = strdup(buffer);

	}

}

// Functie pentru afisare
void print(char* str) {
	puts(str);
}

// colorare sirul de cautat si pastrat in redColor
void coloring(char *stringToBeSearched, char *redColor) {
	strcpy(redColor, "\e[0;31m");
	strcat(redColor, stringToBeSearched);
	strcat(redColor, "\e[m");
}

// In functia grep am creat un nou sir "grepString" in care creez noul sir cu cele care trebuie colorate
void grep(char *arrayOfStrings, char *stringToBeSearched, char *redColor) {
	int nrOfApr = 0, poz;
	char *foundString; // pointer catre sirul care trebuie cautat
	char grepString[201];
	strcpy(grepString, arrayOfStrings);

// caut in sirul care trebuie cautat in linia trimisa
	if((foundString = strstr(arrayOfStrings, stringToBeSearched)) != NULL ) {
		poz = foundString - arrayOfStrings; // aflu pozitia de unde incepe sirul care trebuie cautat
		foundString = foundString + strlen(stringToBeSearched); 
		strcpy(grepString + poz, redColor); // copiez in grepString sirul colorat pe pozitia potrivita
		strcat(grepString, foundString); // concatenez la sir restul ce nu trebuie colorat
		++nrOfApr; // cresc numarul de aparitii
		poz += strlen(stringToBeSearched);

// in continuare pentru fiecare sir gasit face totul ca if
		while((foundString = strstr(arrayOfStrings + poz, stringToBeSearched)) != NULL) {
			poz = foundString - arrayOfStrings;
			foundString = foundString + strlen(stringToBeSearched);
			strcpy(grepString + (poz + nrOfApr*10), redColor);
			strcat(grepString, foundString);
			++nrOfApr;
			poz += strlen(stringToBeSearched);
		}
		// modific sirul initial cu cel colorat
		arrayOfStrings = grepString;

		print(arrayOfStrings);
	}

}

// functie eliberare memorie
void free_mem(char **arrayOfStrings, char *stringToBeSearched, int n_linii)
{
    int i;
    for (i = 0; i < n_linii; ++i) {
    	free(arrayOfStrings[i]);
    	arrayOfStrings[i] = NULL;
    }
    free(arrayOfStrings);
    arrayOfStrings = NULL;
    free(stringToBeSearched);
    stringToBeSearched = NULL;	
}


int main() {
		
    char *stringToBeSearched = malloc(30 * sizeof(char));	// alocare memorie pentru sirul care trebuie cautat
    int n_linii;
	scanf("%[^\n]", stringToBeSearched);
	scanf("%d", &n_linii);
    char **arrayOfStrings = malloc( n_linii * sizeof(char*) );	// alocare pentru vectorul de linii

    read(n_linii, arrayOfStrings);	// citire si alocare fiecarei linii de text, spatiul necesar
    char redColor[41];	// 41 = 30 de caractere max + 10 de la culoare + caracterul null 
    coloring(stringToBeSearched, redColor);	// stocare in redColor sirul care trebuie cautat, colorat in rosu.

    // pentru fiecare linie de text se aplica functia grep
	int i;
    for(i = 0 ; i < n_linii; ++i) {
    	grep(arrayOfStrings[i], stringToBeSearched, redColor);
    }
    
    free_mem(arrayOfStrings, stringToBeSearched, n_linii);
    return 0;
}
