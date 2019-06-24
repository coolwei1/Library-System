#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)

void CLEAR(), PAUSE(), STUDENT();
int MainMenu();

int RESPONE;

void main()
{
	do {
		RESPONE = MainMenu();
		switch (RESPONE)
		{
		case 1: {CLEAR(); LibrarianLOGIN(); break; }
		case 2: {CLEAR(); STUDENT(); break; }
		case 3: {exit(0); break; }
		default: CLEAR(); printf("\n\t> Please enter again.\n");
		}
	} while (RESPONE != 3);
	system("pause");
}
/* ******************************************************************************************************************* */
int MainMenu()
{
	char ch;

	printf("\n\t< LIBRARY SYSTEM >\n");
	printf("\n\t1. Librarian");
	printf("\n\t2. Student");
	printf("\n\t3. Exit");
	printf("\n\tYour choice > ");
	scanf(" %d", &RESPONE);

	while ((ch = getchar()) != '\n' && ch != EOF);

	return RESPONE;
}
/* ******************************************************************************************************************* */
void CLEAR() { system("cls"); }// CLEAR

void PAUSE() { system("pause"); }// PAUSE
