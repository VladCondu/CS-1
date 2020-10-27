#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// citire si alocare spatiu pentru liniile de text
void read(char **arrayOfStrings, int n) {
	int i;
	for (i = 0; i < n; ++i) {
		arrayOfStrings[i] = (char *) malloc (201);
		fgets(arrayOfStrings[i], 200, stdin);
		arrayOfStrings[i] = realloc (arrayOfStrings[i], strlen(arrayOfStrings[i]) + 1);
	}
}

// Functie eliberare memorie
void freeMem(char **arrayOfStrings, int n) {
	int i;
	for (i = 0; i < n; ++i) {
		free(arrayOfStrings[i]);
	}
	free(arrayOfStrings);
}

// Functia de verificare daca sirul este de fapt un numar
int IsValidNumber(char * string)
{
	int lenght;
	// cazul in care sirul este ultimul din linie si va contine '\n'
	if(string[strlen(string) - 1] == '\n') {
		lenght = strlen(string) - 1;
	} else {
		lenght = strlen(string);
	}
	// verifica codul ascii al caracterului, daca este in afara inseamna ca sirul trimis nu este un numar
	int i;
	for (i = 0; i < lenght; i ++)
	{
		if (string[i] < 48 || string[i] > 57) {
			return 0;
		}
	}

	return 1;
}

// Functia pentru criptare
void encrypt(char *token) {
	char key = token[0]; // key pastreaza primul caracter cu rolul de cheie
	int i;
	// pe fiecare caracter din token se aplica metoda de criptare
	for (i = 1; i < strlen(token); ++i) {
		if(token[i] != '\n') {
			token[i] += key;
			token[i] = token[i] % 256; 
		}
	}
}

// separa sirul trimis in campuri si retinut de token
void stringSeparator(char *pString) {
	char *token = strtok(pString, " ");
	int firstTime = 1; 
	while (token != NULL) {

		//verifica daca campul este numar si daca e afisat pentru prima data
		if (IsValidNumber(token)) {
			if (firstTime) {
				printf("%s", token);
				firstTime = 0;
			} else {
				printf(" %s", token);
			}
		} else {
			encrypt(token); // daca nu este numar il cripteaza
			if (firstTime) {
				printf("%s", token);
				firstTime = 0;
			} else {
				printf(" %s", token);
			}
		}
		token = strtok(NULL, " ");
	}
}

int main(void)
{
	int n;
	scanf("%d", &n);
	getchar(); // elimnare '/n' din buffer de la citirea anterioara
	char **arrayOfStrings = (char **) malloc ( n * sizeof(char *));
	read(arrayOfStrings, n);
	int i;
	for (i = 0; i < n; ++i) {
		stringSeparator(arrayOfStrings[i]);
	}
	freeMem(arrayOfStrings, n);
	return 0;
}