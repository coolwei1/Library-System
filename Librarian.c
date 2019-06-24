#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#pragma warning (disable:4996)

void LibrarianADD(), LibrarianDELETE(int), LibrarianACC(int), LibrarianMENU(int), LibrarianLOGIN(), LibrarianSEARCH(), LibrarianDISPLAY(), SearchStudentRecord();

char ANSWER;
int RESPONSE;

FILE*LibrarianBIN;

typedef struct
{
	char ID[21], PASSWORD[21], NAME[101], POSITION[31];
}Librarian;

Librarian LB[100];

void LibrarianLOGIN()
{
	int i = 0, j, CMP, C1, C2, ch;
	char ID[21], PASSWORD[21], x;
	LibrarianBIN = fopen("LibrarianData.bin", "rb");
	/*Read all records into array*/
	do
	{
		fread(&LB[i], sizeof(Librarian), 1, LibrarianBIN);
		i++;
	} while (!feof(LibrarianBIN));
	fclose(LibrarianBIN);
	/*Login interface*/
	do
	{
		printf("\n\t< Librarian Login >");
		printf("\n\t* Enter \"x\" in ID to quit\n");
		printf("\n\tID: ");
		scanf(" %s", &ID);
		if (strcmp(ID, "X") == 0 || strcmp(ID, "x") == 0)
		{
			CLEAR();
			return;
		}

		printf("\n\tPassword: ");
		/*Masking*/
		for (i = 0; (x = getch()) != '\r';)
		{
			if (x == 8) //'8' is backspace in ASCII
			{
				if (i <= 0); //for user to erase password input
				else
				{
					i--;
					printf("\b \b");
					PASSWORD[i] = '\0';
				}
			}
			else if (i == 20)
				break;
			else
			{
				putch('*');
				PASSWORD[i] = x;
				i++;
			}
		}
		PASSWORD[i] = '\0';

		for (j = 0; j <= i; j++)
		{
			C1 = strcmp(LB[j].ID, ID);
			C2 = strcmp(LB[j].PASSWORD, PASSWORD);
			CMP = C1 || C2;
			if (CMP == 0)break;
		}
		if (CMP == 0) { printf("\n\n\tVerified !"); Sleep(700); CLEAR(); LibrarianMENU(j); }
		else
		{
			/*Confirmation*/
			printf("\n\n\t> Invalid ID and PASSWORD.");
			do
			{
				printf("\n\n\tContinue(Y/N)? ");
				scanf(" %c", &ANSWER);
				while (getchar() != '\n');
				if (toupper(ANSWER) == 'Y') { CLEAR(); continue; }
				else if (toupper(ANSWER) == 'N') { CLEAR(); return; }
				else printf("\n\t> Please enter again.");
			} while (toupper(ANSWER) != 'Y' && toupper(ANSWER) != 'N');
		}
	} while (toupper(ANSWER) == 'Y');
}

void LibrarianMENU(int i)
{
	int error;
	do
	{
		CLEAR();
		printf("\n\t< LIBRARIAN MENU >");
		printf("\n\tWelcome, %s", LB[i].NAME);
		printf("\n\n\t%-35s", "1 .  Logout");
		printf("\n\n\t%-35s  %-35s", "Modify Function", "Display function");
		printf("\n\t%-35s  %-35s", "===============", "================");
		printf("\n\t%-35s  %-35s", "2 .  Modify Account Information", "5 .  Display Librarian Record");
		printf("\n\t%-35s  %-35s", "3 .  Modify Student Information", "6 .  Display Student Record");
		printf("\n\t%-35s  %-35s", "4 .  Modify Book Information", "7 .  Display Book Record");
		printf("\n\n\t%-35s  %-35s", "Search function", "Checkout & Return function");
		printf("\n\t%-35s  %-35s", "===============", "==========================");
		printf("\n\t%-35s  %-35s", "8 .  Search Librarian Information", "11.  Display Checkout Records");
		printf("\n\t%-35s  %-35s", "9 .  Search Student Information", "12.  Display Returned Book History");
		printf("\n\t%-35s", "10.  Search Book Information");
		printf("\n\n\t%-35s  %-35s", "Delete function", "Add function");
		printf("\n\t%-35s  %-35s", "===============", "===============");
		printf("\n\t%-35s  %-35s", "13.  Delete Book", "16.  Add Book");
		printf("\n\t%-35s  %-35s", "14.  Delete Librarian", "17.  Add Librarian");
		printf("\n\t%-35s  %-35s", "15.  Delete Student", "18.  Add Student");
		printf("\n\n\tYour choice > ");
		error = scanf(" %d", &RESPONSE);
		while (getchar() != '\n');
		if (error != 1) { printf("\n\t> Invalid input. "); PAUSE(); CLEAR(); }
		else
		{
			switch (RESPONSE)
			{// 4, 7, 10, 11, 12, 13, 14
			case 1: {CLEAR(); return; break; }
			case 2: {CLEAR(); LibrarianACC(i); break; }
			case 3: {CLEAR(); ModifyStudentRecordbyLibrarian(i); break; }
			case 4: {CLEAR(); Modify_Book(); break; }
			case 5: {CLEAR(); LibrarianDISPLAY(); break; }
			case 6: {CLEAR(); DisplayStudentRecord(); PAUSE(); break; }
			case 7: {CLEAR(); Display_Book(); PAUSE(); break; }
			case 8: {CLEAR(); LibrarianSEARCH(); break; }
			case 9: {CLEAR(); SearchStudentRecord(); break; }
			case 10: {CLEAR(); Search_Book(); break; }
			case 11: {CLEAR(); Display_CheckOut_Record_Librarian(); break; }
			case 12: {CLEAR(); Display_CheckIn_Record_Librarian(); break; }
			case 13: {CLEAR(); Delete_Book(); break; }
			case 14: {CLEAR(); LibrarianDELETE(i); break; }
			case 15: {CLEAR(); DeleteStudentRecord(); break; }
			case 16: {CLEAR(); Add_Book(); break; }
			case 17: {CLEAR(); LibrarianADD(); break; }
			case 18: {CLEAR(); AddStudentRecord(); break; }
			default: {printf("\n\t> Invalid input. "); PAUSE(); CLEAR(); }
			}
		}
	} while (RESPONSE != 1);
	return;
}

void LibrarianACC(int i)
{
	int error, invalid, count = 0, c;
	char newPASSWORD[21], newID[21];
	/*Read records into arrays*/
	LibrarianBIN = fopen("LibrarianData.bin", "rb");
	do
	{
		fread(&LB[count], sizeof(Librarian), 1, LibrarianBIN);
		count++;
	} while (!feof(LibrarianBIN));
	fclose(LibrarianBIN);
	/*Get new information*/
	do
	{
		CLEAR();
		printf("\n\t< Modify Account Information >\n");
		printf("\n\t1. ID");
		printf("\n\t2. PASSWORD");
		printf("\n\t3. Back");
		printf("\n\tYour choice> ");
		error = scanf(" %d", &RESPONSE);
		while (getchar() != '\n');
		if (error != 1)
		{
			printf("\n\t> This is not a numeric input."); PAUSE();
		}
		else
		{
			switch (RESPONSE)
			{
			case 1:
			{
				// ID
				do
				{
					c = 0;
					printf("\n\tOld ID: %s", LB[i].ID);
					printf("\n\tNew ID: ");
					scanf("%[^\n]", newID);
					/*No same ID exist in the data*/
					do
					{
						invalid = strcmp(newID, LB[c].ID);
						if (invalid == 0)
						{
							invalid = 1;
							printf("\n\t> This ID already existed.\n");
							break;
						}
						c++;
					} while (c != count);
					invalid = stringlen(newID, 20) || xBlank(newID);
					if (invalid == 1)
						printf("\n\t> Cannot more than 20 characters and space(s).\n");
				} while (invalid == 1);
				CLEAR();
				strcpy(LB[i].ID, newID);
				break;
			}
			case 2:
			{
				do
				{
					printf("\n\tOld PASSWORD: %s", LB[i].PASSWORD);
					printf("\n\tNew PASSWORD: ");
					scanf("%[^\n]", newPASSWORD);
					invalid = stringlen(newPASSWORD, 20) || xBlank(newPASSWORD);
					if (invalid == 1)
						printf("\n\t> Cannot more than 20 characters and spaces.\n");
				} while (invalid == 1);
				CLEAR();
				strcpy(LB[i].PASSWORD, newPASSWORD);
				break;
			}
			case 3:
			{
				/*Confirmation*/
				printf("\n\tNew ID      : %s", LB[i].ID);
				printf("\n\tNew Password: %s", LB[i].PASSWORD);
				printf("\n\t> Confirm(Y=YES)? ");
				scanf(" %c", &ANSWER);
				if (toupper(ANSWER) == 'Y')
				{
					/*Append the new information*/
					LibrarianBIN = fopen("LibrarianData.bin", "wb");
					for (c = 0; c < count; c++)
						fwrite(&LB[c], sizeof(Librarian), 1, LibrarianBIN);
					fclose(LibrarianBIN);
					printf("\n\t> Modified successfully. ");
					PAUSE(); CLEAR();
					return;
				}
				else { PAUSE(); CLEAR(); return; }
				break;
			}
			}
		}
	} while (RESPONSE != 3 || RESPONSE < 0 || RESPONSE > 3);
}

void LibrarianADD()
{
	int invalid, a, j = 0, count = 0;
	char x, KEY[21];
	Librarian LB[100], newLB;
	/*Read all data into array*/
	LibrarianBIN = fopen("LibrarianData.bin", "rb");
	do
	{
		fread(&LB[count], sizeof(Librarian), 1, LibrarianBIN);
		count++;
	} while (!feof(LibrarianBIN));
	fclose(LibrarianBIN);

	printf("\n\t< Librarian ADD MODULE - Librarian >");
	printf("\n\t* Account will activate after restart the program\n");
	/*Confirm the user have the right to add data*/
	printf("\n\tEnter the key to add new data: ");
	/*Masking*/
	for (j = 0; (x = getch()) != '\r';)
	{
		if (x == 8) //'8' is backspace in ASCII
		{
			if (j <= 0); //for user to erase password input
			else
			{
				j--;
				printf("\b \b");
				KEY[j] = '\0';
			}
		}
		else if (j == 20)
			break;
		else
		{
			putch('*');
			KEY[j] = x;
			j++;
		}
	}
	KEY[j] = '\0';

	if (strcmp(LB[0].PASSWORD, KEY) != 0)
	{
		printf("\n\t> Wrong ! ");
		PAUSE(); CLEAR();
		return;
	}
	else printf("\n\t> Verified !\n");

	int Invalid01, Invalid02;// Special case: Use in entering ID only as cannot use the same variable "invalid" to validate data because it will cause unable to loop.
	do
	{
		a = 0;
		printf("\n\tEnter ID: ");
		scanf(" %[^\n]", &newLB.ID);
		Invalid01 = stringlen(newLB.ID, 20) || xBlank(newLB.ID);
		if (Invalid01 == 1)
		{
			printf("\n\t> Cannot more than 20 characters and space(s).\n");
		}
		do
		{
			Invalid02 = strcmp(newLB.ID, LB[a].ID);
			if (Invalid02 == 0)
			{
				printf("\n\t> This ID already existed.\n");
				break;
			}
			a++;
		} while (a != count);
	} while (Invalid01 == 1 || Invalid02 == 0);

	do
	{
		printf("\n\tEnter Password: ");
		scanf(" %[^\n]", &newLB.PASSWORD);
		invalid = stringlen(newLB.PASSWORD, 20) || xBlank(newLB.PASSWORD);
		if (invalid == 1)printf("\n\t> Cannot more than 20 characters and space(s).\n");
	} while (invalid == 1);

	do
	{
		printf("\n\tEnter Name: ");
		scanf(" %[^\n]", newLB.NAME);
		invalid = xDigit(newLB.NAME) || stringlen(newLB.NAME, 100);
		if (invalid == 1) printf("\n\t> No digit in name and no more than 100 words.\n");
	} while (invalid == 1);

	do
	{
		printf("\n\tEnter Position: ");
		scanf(" %[^\n]", newLB.POSITION);
		invalid = xDigit(newLB.POSITION) || stringlen(newLB.POSITION, 30);
		if (invalid == 1) printf("\n\t> No digit in position and no more than 30 words.\n");
		if (strcmp(newLB.POSITION, "Library Admin") == 0)
		{
			invalid = 1;
			printf("\n\t> You cannot add Library Admin.\n");
		}
	} while (invalid == 1);

	/*Comfirmation*/
	printf("\n\t[Confirm] to add new record(Y=YES)? ");
	scanf(" %c", &ANSWER);
	/*Append new record*/
	if (toupper(ANSWER) == 'Y')
	{
		LibrarianBIN = fopen("LibrarianData.bin", "ab");
		fwrite(&newLB, sizeof(Librarian), 1, LibrarianBIN);
		fclose(LibrarianBIN);
		printf("\n\t> Record added. ");
		PAUSE(); CLEAR();
		return;
	}
	return;
}

void LibrarianDELETE(int i)
{
	int count = 0, a, b, j = 0, CMP;
	char ID[21], KEY[21], x;
	/*Read records into arrays*/
	LibrarianBIN = fopen("LibrarianData.bin", "rb");
	do
	{
		fread(&LB[count], sizeof(Librarian), 1, LibrarianBIN);
		count++;
	} while (!feof(LibrarianBIN));
	fclose(LibrarianBIN);
	printf("\n\t< Search Librarian >");
	printf("\n\t* Enter 'x' to return.\n");
	/*Check ID*/
	do
	{
		/*Get ID*/
		do
		{
			printf("\n\tEnter the librarian's ID that want to delete: ");
			scanf(" %s", &ID);
			if (strcmp(ID, LB[i].ID) == 0)
				printf("\n\t> You can't delete yourself.\n");
			else if (strcmp(ID, "X") == 0 || strcmp(ID, "x") == 0)
			{
				CLEAR();
				return;
			}
		} while (strcmp(ID, LB[i].ID) == 0);
		/*Checking*/
		for (a = 0; a < count; a++)
		{
			CMP = strcmp(ID, LB[a].ID);
			if (CMP == 0)break;
		}
		if (a == count)printf("\n\t> Invalid ID.\n");
	} while (a == count);
	/*Confirm the user have the right to delete data*/
	printf("\n\tEnter the key to delete: ");
	/*Masking*/
	for (j = 0; (x = getch()) != '\r';)
	{
		if (x == 8) //'8' is backspace in ASCII
		{
			if (j <= 0); //for user to erase password input
			else
			{
				j--;
				printf("\b \b");
				KEY[j] = '\0';
			}
		}
		else if (j == 20)
			break;
		else
		{
			putch('*');
			KEY[j] = x;
			j++;
		}
	}
	KEY[j] = '\0';

	if (strcmp(LB[0].PASSWORD, KEY) != 0)
	{
		printf("\n\n\tWrong password ! ");
		PAUSE(); CLEAR();
		return;
	}
	else
	{
		printf("\n\n\tVerified !");
		/*Confirmation*/
		if (CMP == 0)
		{
			printf("\n\tID      : %s\t", LB[a].ID);
			printf("\n\tName    : %s\t", LB[a].NAME);
			printf("\n\tPosition: %s\n", LB[a].POSITION);
			printf("\n\tConfirm(Y=YES)? ");
			scanf(" %c", &ANSWER);
			/*Overwrite in arrays*/
			if (toupper(ANSWER) == 'Y')
				for (b = a; b < count - 1; b++)
					LB[b] = LB[b + 1];
			else return;
		}
		/*Rewrite in binary file*/
		LibrarianBIN = fopen("LibrarianData.bin", "wb");
		for (a = 0; a < count - 1; a++)
			fwrite(&LB[a], sizeof(Librarian), 1, LibrarianBIN);
		fclose(LibrarianBIN);
		/*Successfull*/
		printf("\n\t> Deleted successfully. ");
		PAUSE(); CLEAR();
		return;
	}
}

void LibrarianSEARCH()
{
	char ID[21];
	int count = 0, a, CMP;
	LibrarianBIN = fopen("LibrarianData.bin", "rb");
	do
	{
		fread(&LB[count], sizeof(Librarian), 1, LibrarianBIN);
		count++;
	} while (!feof(LibrarianBIN));
	fclose(LibrarianBIN);
	do
	{
		printf("\n\t< Search Librarian >\n");
		printf("\n\tID: ");
		scanf(" %s", &ID);
		for (a = 0; a < count; a++)
		{
			CMP = strcmp(ID, LB[a].ID);
			if (CMP == 0)break;
		}
		if (a != count)
		{
			/*Display information*/
			printf("\n\tID      : %s", LB[a].ID);
			printf("\n\tName    : %s", LB[a].NAME);
			printf("\n\tPosition: %s", LB[a].POSITION);
		}
		else
			printf("\n\tThis librarian doesn't existed.");
		/*Confirmation*/
		printf("\n\n\t[Continue] to search(Y=YES)? ");
		scanf(" %c", &ANSWER);
	} while (toupper(ANSWER) == 'Y');
	return;
}

void LibrarianDISPLAY()
{
	int count = 0, NO = 1, limit;

	LibrarianBIN = fopen("LibrarianData.bin", "rb");

	printf("\n\t%-3s %-21s %-21s %-101s", "NO.", "ID", "POSITION", "NAME");
	printf("\n\t%-3s %-21s %-21s %-101s", "===", "==", "========", "====");
	printf("\n\t%-3d %-21s %-21s %-101s", 1, "", LB[0].POSITION, LB[0].NAME);// To hide the admin ID
	do
	{
		fread(&LB[count], sizeof(Librarian), 1, LibrarianBIN);
		count++;
	} while (!feof(LibrarianBIN));
	limit = count;
	for (count = 1; count < limit; count++)
	{
		if (strcmp(LB[count].ID, "") != 0)
			printf("\n\t%-3d %-21s %-21s %-101s", NO++, LB[count].ID, LB[count].POSITION, LB[count].NAME);
	}
	fclose(LibrarianBIN);
	printf("\n\n\t");
	PAUSE(); CLEAR();
	return;
}