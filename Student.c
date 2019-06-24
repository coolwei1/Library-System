#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>
#pragma warning (disable:4996)

SYSTEMTIME t;

//Declaration of functions
//In student module
int StudentLogin(int studentCount);
int StudentMenu(int studentIndex);
void ModifyStudentRecordbyStudent(int studentIndex);
int StudentLogout(int studentIndex);

//In librarian module
void AddStudentRecord();
void DisplayStudentRecord();
void DeleteStudentRecord();
void ModifyStudentRecordbyLibrarian(int studentIndex);

//Declaration of structure for student data
typedef struct {
	char name[31], programme[4];
	int yearOfStudy, noBorrowedBook;
	char ID[11], password[11], contactNo[11], gender;
}Stud;

//used to store data in binary file before transfer to stud[i] array
Stud student;

//used to receive data in student structure variable
Stud stud[100];

void STUDENT()
{
	//Declaration of variable
	int action = 1, studentIndex, studentCount = 0, i, dontWantLogout;
	char yesORno;

	//Transfer data in binary file to structure array, stud
	FILE*fptr;
	fptr = fopen("StudentData.bin", "rb");
	fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	while (!feof(fptr))
	{
		studentCount++;
		fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	}
	fclose(fptr);

	//studentIndex indicate which student in array login.
	studentIndex = StudentLogin(studentCount);

	//if StudentLogin() return -1, return to main manu
	//StudentLogin() return -1 when user want to go back to main page
	if (studentIndex == -1) {
		CLEAR();
		return;
	}

	do {
		//action indicate what action user want to perform, return by StudentMenu()
		action = StudentMenu(studentIndex);

		switch (action)
		{
		case 1:
			Display_Book();
			PAUSE();
			break;
		case 2:
			Search_Book();
			break;
		case 3:
			Display_CheckOut_Record_Student(studentIndex);
			break;
		case 4:
			Display_CheckIn_Record_Student(studentIndex);
			break;
		case 5:
			Borrow_Return_Menu(studentIndex);
			break;
		case 6:
			ModifyStudentRecordbyStudent(studentIndex);
			break;
		case 7:
			dontWantLogout = StudentLogout(studentIndex);
			break;
		default:
			printf("\n\t*Please only enter number 1 to 7!\n");
			PAUSE();
		}

		//Save all data in stud array to file after each function (except logout function) completed
		if (action != 5) {
			fptr = fopen("StudentData.bin", "wb");
			for (i = 0; i < studentCount; i++) {
				fwrite(&stud[i], sizeof(Stud), 1, fptr);
			}
			fclose(fptr);
		}
		//if user did not logout (enter value 5), loop is continue
	} while (action != 7 || dontWantLogout == -1);

	printf("\n");
}

//Functions below are called in student module >> STUDENT() function
int StudentLogin(int studentCount)
{
	//Declaration of variable
	char studentID[11], studentPass[11], x;
	int i, studentIndex = 0, ch;

	do {
		//Validation of student ID
		printf("\n\t\tStudent Login");
		printf("\n\tPlease enter your ID (e.g.16WAD02116) (enter 'x' to go back): ");
		scanf(" %s", &studentID);

		//Change all character in ID to upper case
		for (i = 0; i < strlen(studentID); i++)
			studentID[i] = toupper(studentID[i]);

		//if user enter 'x', will return -1 to STUDENT()
		if (strcmp(studentID, "X") == 0) {
			CLEAR();
			return -1;
		}

		//compare ID user entered with ID in array
		for (i = 0; i < studentCount; i++) {
			if (strcmp(stud[i].ID, studentID) == 0) {
				//studentIndex indicate the student's index in array for the student login.
				studentIndex = i;
				break;
			}
		}

		//break from loop if there is ID same in array
		if (strcmp(stud[studentIndex].ID, studentID) == 0)
			break;
		//Message is display if no ID in array match with the ID user entered
		else
			printf("\n\n\t> ID does not exist, please try again.");
	} while (strcmp(stud[i].ID, studentID) != 0 || strcmp(studentID, "X") != 0);

	//validation of password
	do {
		printf("\n\n\tPlease enter your password (enter 'x' to go back): ");

		//Password mask
		for (i = 0; (x = getch()) != '\r';) {
			if (x == '\b') {
				if (i <= 0); //for user to erase password input
				else
				{
					i--;
					printf("\b \b");
					studentPass[i] = '\0';
				}
			}
			else if (i == 10) //if user enter more than 10 characters, break from loop
				break;
			else {
				putch(x); // to show their password
				printf("\b \b");
				putch('*');
				studentPass[i] = x;
				i++;
			}
		}
		studentPass[i] = '\0';

		//return -1 to STUDENT to go back
		if (strcmp(studentPass, "X") == 0 || strcmp(studentPass, "x") == 0)
			return -1;

		if (strcmp(stud[studentIndex].password, studentPass) == 0)
			break;

		else
			printf("\n\n\t> Incorrect password, please try again.");

	} while (strcmp(stud[studentIndex].password, studentPass) != 0);

	return studentIndex;
}

int StudentMenu(int studentIndex)
{
	GetLocalTime(&t);

	int doWhat;
	char ch, dayOFWeek[10];

	CLEAR();

	switch (t.wDayOfWeek) {
	case 0:
		strcpy(dayOFWeek, "Sunday");
		break;
	case 1:
		strcpy(dayOFWeek, "Monday");
		break;
	case 2:
		strcpy(dayOFWeek, "Tuesday");
		break;
	case 3:
		strcpy(dayOFWeek, "Wednesday");
		break;
	case 4:
		strcpy(dayOFWeek, "Thursday");
		break;
	case 5:
		strcpy(dayOFWeek, "Friday");
		break;
	case 6:
		strcpy(dayOFWeek, "Saturday");
	}

	//Display welcome message and time
	printf("\n\tWelcome, %s", stud[studentIndex].name);
	printf("\n\tDate: %d/%d/%d (%s)\t\tTime: %02d:%02d:%02d", t.wDay, t.wMonth, t.wYear, dayOFWeek, t.wHour, t.wMinute, t.wSecond);

	printf("\n\n\tAction menu: ");
	printf(
		"\n\t1. Display book"
		"\n\t2. Search book"
		"\n\t3. Display borrowed book detail"
		"\n\t4. Display returned book detail"
		"\n\t5. Checkout or return book"
		"\n\t6. Modify your password and detail"
		"\n\t7. Logout"
	);

	printf("\n\n\tPlease choose a action: ");
	scanf(" %d", &doWhat);
	while ((ch = getchar()) != '\n' && ch != EOF);

	return(doWhat);
}

void ModifyStudentRecordbyStudent(int studentIndex) {
	int modifyWhat, invalid, i;
	char newContactNo[11], ch, x;
	char oldPassword[11], newPassword_1[11], newPassword_2[11], yesORno;
	
	do {
		CLEAR();
		printf(
			"\n\tList of Detail you can modify: "
			"\n\t1. Password"
			"\n\t2. Contact number"
			"\n\t3. Go back"
			"\n\n\tPlease enter respective number of detail that you wish to modify: "
		);
		scanf("%d", &modifyWhat);

		switch (modifyWhat) {
			//Modify password
		case 1:
		{
			system("cls");
			printf("\n\tWelcome, %s", stud[studentIndex].name);
			do {
				printf("\n\n\tPlease enter your old password (or enter 'x' to exit): ");

				//Password mask
				for (i = 0; (x = getch()) != '\r';)
				{
					if (x == '\b')
					{
						if (i <= 0); //for user to erase password input
						else
						{
							i--;
							printf("\b \b");
							oldPassword[i] = '\0';
						}//End if-else
					}
					else if (i == 10)
						break;
					else
					{
						putch(x); // to show their password
						printf("\b \b");
						putch('*');
						oldPassword[i] = x;
						i++;
					}//End if else
				} //End for
				oldPassword[i] = '\0';

				if (strcmp(oldPassword, stud[studentIndex].password) == 0)
					printf("\n\t> Verified!");

				else if (strcmp(oldPassword, "x") == 0 || strcmp(oldPassword, "X") == 0) {
					CLEAR();
					break;
				}

				else
					printf("\n\n\t> Wrong password entered! Please try again.");
			} while (strcmp(oldPassword, stud[studentIndex].password) != 0);

			if (strcmp(oldPassword, "x") == 0)
				break;

			do {
				do {
					printf("\n\n\tPlease enter your new password: ");

					//Password mask
					for (i = 0; (x = getch()) != '\r';)
					{
						if (x == '\b')
						{
							if (i <= 0); //for user to erase password input
							else
							{
								i--;
								printf("\b \b");
								newPassword_1[i] = '\0';
							}//End if-else
						}
						else if (i == 10)
							break;
						else
						{
							putch(x); // to show their password
							printf("\b \b");
							putch('*');
							newPassword_1[i] = x;
							i++;
						}
					}
					newPassword_1[i] = '\0';

					invalid = xBlank(newPassword_1) || xPunct(newPassword_1) || stringlen(newPassword_1, 10);
					if (invalid)
						printf("\n\t> Please don't entered any blank space, punctuation characters or entered more than 10 characters.\n");

				} while (invalid);

				do {
					printf("\n\n\tPlease again enter your new password: ");

					//Password mask
					for (i = 0; (x = getch()) != '\r';)
					{
						if (x == '\b')
						{
							if (i <= 0); //for user to erase password input
							else
							{
								i--;
								printf("\b \b");
								newPassword_2[i] = '\0';
							}//End if-else
						}
						else
						{
							putch(x); // to show their password
							printf("\b \b");
							putch('*');
							newPassword_2[i] = x;
							i++;
						}//End if else
					} //End for
					newPassword_2[i] = '\0';

					invalid = xBlank(newPassword_2) || xPunct(newPassword_2) || stringlen(newPassword_2, 10);

					if (invalid)
						printf("\n\t> Please don't entered any blank space, punctuation characters or entered more than 10 characters.\n");

				} while (invalid);

				if (strcmp(newPassword_1, newPassword_2) == 0)
					printf("\n\t> Matched!");
				else
					printf("\n\t> Does not matched, please try again!");

			} while (strcmp(newPassword_1, newPassword_2) != 0);

			do {
				printf("\n\n\tDo you really want to change your password? (Y/N): ");
				scanf(" %c", &yesORno);

				while ((ch = getchar()) != '\n' && ch != EOF);

				if (tolower(yesORno) == 'y') {
					strcpy(stud[studentIndex].password, newPassword_1);
					printf("\n\n\t> Your password has been changed! ");
					PAUSE();
				}

				else if (tolower(yesORno) == 'n') {
					printf("\n\t> Operation canceled. ");
					PAUSE();
				}
				else
					printf("\n\t> Please enter Y or N only");
			} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');

			break;
		}

		//Modify contact number
		case 2:
		{
			system("cls");
			printf("\n\tThis is your old contact number: ");
			printf("\n\n\t\t%s", stud[studentIndex].contactNo);

			do {
				printf("\n\n\tPlease enter your new contact number (or 'x' to exit): ");
				scanf("%s", newContactNo);

				if (strcmp(newContactNo, "x") == 0)
					break;

				invalid = xBlank(newContactNo) || xAlpha(newContactNo) || stringlen(newContactNo, 10);

				if (invalid)
					printf("\n\t> Please don't entered: space, alphabetic character, or entered more than 10 characters.\n");

			} while (invalid);


			if (strcmp(newContactNo, "x") == 0)
				break;

			do {
				printf("\n\t> Your contact number will change to ---> %s", newContactNo);
				printf("\n\n\tDo you really want to change your contact number? (Y/N): ");
				scanf(" %c", &yesORno);

				while ((ch = getchar()) != '\n' && ch != EOF);

				if (tolower(yesORno) == 'y') {
					strcpy(stud[studentIndex].contactNo, newContactNo);
					printf("\n\n\t> Your contact number has been changed! ");
					PAUSE();
				}
				else if (tolower(yesORno) == 'n') {
					printf("\n\t> Operation canceled. ");
					PAUSE();
				}
				else
					printf("\n\t> Please enter Y or N only");
			} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
		}

		//Go back
		case 3:
			CLEAR();
			return;

		default:
			printf("\n\tPlease enter 1 to 3 only.");
		}
	} while (strcmp(oldPassword, "x") == 0 || strcmp(newContactNo, "x") == 0 || modifyWhat != 1 && modifyWhat != 2);
}

int StudentLogout(int studentIndex)
{
	char confirm = ' ', ch;
	do {
		printf("\n\tConfirm logout? (Y/N): ");
		scanf(" %c", &confirm);

		while ((ch = getchar()) != '\n' && ch != EOF);

		confirm = tolower(confirm);

		if (confirm == 'y') {
			CLEAR();
			printf("\n\t> Please come back again!\n");
			break;
		}
		//return -1 to STUDENT()
		else if (confirm == 'n') {
			return -1;
		}
		else
			printf("\n\t> Please enter only Y or N!");

	} while (confirm != 'y');
}

//Functions below are called in librarian module
void SearchStudentRecord()
{
	FILE*fptr;
	fptr = fopen("StudentData.bin", "rb");

	int i, invalid, status, studentCount = 0, counter;
	char yesORno, ch, searchMethod;

	fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	while (!feof(fptr)) //copy all data in file to array stud
	{
		studentCount++;
		fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	}
	fclose(fptr);



	invalid = 0;
	do {

		strcpy(student.name, "");
		strcpy(student.ID, "");
		strcpy(student.programme, "");
		strcpy(student.contactNo, "");
		student.gender = NULL;
		student.yearOfStudy = 0;
		student.noBorrowedBook = 0;

		do {
			CLEAR();

			if (invalid) {
				ch = getchar();
				printf("\n\tPlease don't entered anything other than 1 to 8\n");
			}
			printf(
				"\n\tSearch Student by: "
				"\n\t1. Name"
				"\n\t2. ID"
				"\n\t3. Programme"
				"\n\t4. Year of study"
				"\n\t5. Contact number"
				"\n\t6. Gender"
				"\n\t7. Number of book borrowed"
				"\n\t8. Go back"
				"\n\n\tPlease enter respective number of method you want to search with: "
			);
			scanf(" %c", &searchMethod);
			fflush(stdin);

			invalid = Digit(searchMethod);

		} while (invalid);

		switch (searchMethod) {
			//Name
		case '1': {
			do {
				invalid = 0;
				printf("\n\tPlease enter student name you want to search (max 30 characters) (enter 'x' to exit): ");
				scanf(" %[^\n]", &student.name);
				fflush(stdin);

				invalid = xDigit(student.name) || xPunct(student.name) || stringlen(student.name, 30);

				if (invalid) {
					CLEAR();
					printf("\n\tPlease don't entered any digits, punctuation characters or entered more than 30 characters.\n");
				}

			} while (invalid);

			for (i = 0; i < strlen(student.name); i++) {
				student.name[i] = toupper(student.name[i]);
			}

			if (strcmp(student.name, "X") == 0)
				break;

			counter = 0;

			printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
			printf("\n\t==================================================================================================================================");

			for (i = 0; i < studentCount; i++) {
				if (strcmp(stud[i].name, student.name) == 0) {
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, 
									stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
				}
			}

			if (counter == 0) {
				system("cls");
				printf("\n\t***No record found!\n");
			}
			else
				printf("\n\n\t%d record(s) found.\n\n", counter);

			PAUSE();
			break;
		}

							//ID
		case '2': {
			do {
				invalid = 0;
				printf("\n\tPlease enter student ID you want to search(max 10 characters) (enter 'x' to exit): ");
				scanf(" %[^\n]", &student.ID);
				fflush(stdin);

				invalid = xPunct(student.ID) || stringlen(student.ID, 10);

				if (invalid) {
					CLEAR();
					printf("\n\tPlease don't entered punctuation characters or entered more than 10 characters.\n");
				}

			} while (invalid);

			for (i = 0; i < strlen(student.ID); i++) {
				student.ID[i] = toupper(student.ID[i]);
			}

			if (strcmp(student.ID, "X") == 0)
				break;

			counter = 0;

			printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
			printf("\n\t==================================================================================================================================");

			for (i = 0; i < studentCount; i++) {
				if (strcmp(stud[i].ID, student.ID) == 0) {
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
				}
			}

			if (counter == 0) {
				system("cls");
				printf("\n\t***No record found!\n");
			}
			else
				printf("\n\n\t%d record(s) found.\n\n", counter);

			PAUSE();
			break;
		}

							//Programme
		case '3': {
			do {
				invalid = 0;
				printf("\n\tPlease enter programme you want to search (max 3 characters) (enter 'x' to exit): ");
				scanf(" %[^\n]", &student.programme);
				fflush(stdin);

				invalid = xDigit(student.programme) || xPunct(student.programme) || stringlen(student.programme, 3);

				if (invalid) {
					CLEAR();
					printf("\n\tPlease don't entered any digits, punctuation characters or entered more than 3 characters.\n");
				}

			} while (invalid);

			for (i = 0; i < strlen(student.programme); i++) {
				student.programme[i] = toupper(student.programme[i]);
			}

			if (strcmp(student.programme, "X") == 0)
				break;

			counter = 0;

			printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
			printf("\n\t==================================================================================================================================");

			for (i = 0; i < studentCount; i++) {
				if (strcmp(stud[i].programme, student.programme) == 0) {
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
				}
			}

			if (counter == 0) {
				system("cls");
				printf("\n\t***No record found!\n");
			}
			else
				printf("\n\n\t%d record(s) found.\n\n", counter);

			PAUSE();
			break;
		}

							//Year of study
		case '4': {
			do {
				invalid = 0;
				printf("\n\tPlease enter year of study you want to search (number 1 to 3 only) (enter '-1' to exit): ");
				scanf(" %d", &student.yearOfStudy);
				fflush(stdin);

				switch (student.yearOfStudy) {
				case 1:
				case 2:
				case 3:
				case -1:
					break;
				default:
					ch = getchar();
					invalid = 1;
				}


				if (invalid) {
					printf("\n\tPlease only enter number 1 to 3 only.\n");
					CLEAR();
				}

			} while (invalid);

			if (student.yearOfStudy == -1)
				break;

			counter = 0;

			printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
			printf("\n\t==================================================================================================================================");

			for (i = 0; i < studentCount; i++) {
				if (stud[i].yearOfStudy == student.yearOfStudy) {
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
				}
			}

			if (counter == 0) {
				system("cls");
				printf("\n\t***No record found!\n");
			}
			else
				printf("\n\n\t%d record(s) found.\n\n", counter);

			PAUSE();
			break;
		}

							//Contact number
		case '5': {
			do {
				invalid = 0;
				printf("\n\tPlease enter contact number you want to search (digit OR '-' only) (enter 'x' to exit): ");
				scanf(" %[^\n]", &student.contactNo);
				fflush(stdin);

				for (i = 0; i < strlen(student.contactNo); i++) {
					student.contactNo[i] = toupper(student.contactNo[i]);
				}

				if (strcmp(student.contactNo, "X") == 0)
					break;

				invalid = xBlank(student.contactNo) || xAlpha(student.contactNo) || stringlen(student.contactNo, 10);

				if (invalid) {
					CLEAR();
					printf("\n\tPlease don't entered: space, alphabetic character or entered more than 10 characters.\n");
				}

			} while (invalid);

			if (strcmp(student.contactNo, "X") == 0)
				break;

			counter = 0;

			printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
			printf("\n\t==================================================================================================================================");

			for (i = 0; i < studentCount; i++) {
				if (strcmp(stud[i].contactNo, student.contactNo) == 0) {
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
				}
			}

			if (counter == 0) {
				system("cls");
				printf("\n\t***No record found!\n");
			}
			else
				printf("\n\n\t%d record(s) found.\n\n", counter);

			PAUSE();
			break;
		}

							//Gender
		case '6': {
			do {
				invalid = 0;
				printf("\n\tPlease enter gender you want to search (max 1 characters - 'M' or 'F') (enter 'x' to exit): ");
				scanf(" %c", &student.gender);
				fflush(stdin);
				student.gender = toupper(student.gender);

				switch (student.gender) {
				case 'M':
				case 'F':
				case 'X':
					break;
				default:
					printf("\n\tPlease enter M or F only\n");
					while ((ch = getchar()) != '\n' && ch != EOF);
					invalid = 1;
				}
			} while (invalid);

			if (student.gender == 'X')
				break;

			counter = 0;

			printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
			printf("\n\t==================================================================================================================================");

			for (i = 0; i < studentCount; i++) {
				if (stud[i].gender == student.gender) {
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
				}
			}

			if (counter == 0) {
				system("cls");
				printf("\n\t***No record found!\n");
			}
			else
				printf("\n\n\t%d record(s) found.\n\n", counter);

			PAUSE();
			break;
		}

							//Number of borrowed book
		case '7': {
			do {
				invalid = 0;
				printf("\n\tPlease enter number of book borrowed by student you want to search (0 - 5 only) (enter -1 to exit): ");
				scanf(" %d", &student.noBorrowedBook);
				fflush(stdin);

				switch (student.noBorrowedBook) {
				case -1:
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					break;
				default:
					ch = getchar();
					invalid = 1;
				}

				if (invalid) {
					printf("\n\tPlease enter number from 0 to 5 only.");
					printf("\n\t*Student can borrow maximum 5 books.\n");
					CLEAR();
				}

			} while (invalid);

			if (student.noBorrowedBook == -1)
				break;

			counter = 0;

			printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
			printf("\n\t==================================================================================================================================");

			for (i = 0; i < studentCount; i++) {
				if (stud[i].noBorrowedBook == student.noBorrowedBook) {
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
				}
			}

			if (counter == 0) {
				system("cls");
				printf("\n\t***No record found!\n\n");
			}
			else
				printf("\n\n\t%d record(s) found.\n\n", counter);

			PAUSE();
			break;
		}

							//Go back
		case '8':
			CLEAR();
			return;

		default:
			CLEAR();
			printf("\n\tPlease enter 1 to 8 only\n");
		}
	} while (searchMethod > 7 && searchMethod < 1 || strcmp(student.name, "X") == 0 || strcmp(student.ID, "X") == 0
		|| strcmp(student.programme, "X") == 0 || student.yearOfStudy == -1 || strcmp(student.contactNo, "X") == 0
		|| student.gender == 'X' || student.noBorrowedBook == -1 || counter >= 0);
}

void AddStudentRecord() {

	int i, invalid, status, studentCount = 0, counter = 0;
	char yesORno = NULL , ch;

	FILE*fptr;
	fptr = fopen("StudentData.bin", "rb");
	fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	while (!feof(fptr))
	{
		studentCount++;
		fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	}
	fclose(fptr);


	//Enter name
	{
		do {
			invalid = 0;
			printf("\n\tPlease enter student name (max 30 characters) (enter 'x' to go back): ");
			scanf(" %[^\n]", &student.name);
			fflush(stdin);

			invalid = xDigit(student.name) || xPunct(student.name) || stringlen(student.name, 30);

			if (invalid)
				printf("\n\t> Please don't entered any digits, punctuation characters or entered more than 30 characters.\n");

			for (i = 0; i < strlen(student.name); i++) {
				student.name[i] = toupper(student.name[i]);
			}

			if (strcmp(student.name, "X") == 0) {
				CLEAR();
				return;
			}

			for (i = 0; i < studentCount; i++) {
				if (strcmp(student.name, stud[i].name) == 0) {
					printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
					printf("\n\t==============================================================================================================================");
					printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
					do {
						printf("\n\n\tThis name, \"%s\" existed in student records, do you still want to continue (Y/N)? ", student.name);

						scanf(" %c", &yesORno);
						while ((ch = getchar()) != '\n' && ch != EOF);

						if (tolower(yesORno) == 'y')
							break;
						else if (tolower(yesORno) == 'n') {
							invalid = 1;
							printf("\n\tPlease reenter student name ...");
						}
						else
							printf("\n\tPlease only enter Y or N!");
					} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
				}
				if (tolower(yesORno) == 'y')
					break;
			}
		} while (invalid);
	}
	//Enter ID
	{
		do {
			invalid = 0;
			printf("\n\tPlease enter student ID (max 10 characters) (enter 'x' to go back): ");
			scanf(" %[^\n]", &student.ID);
			fflush(stdin);

			invalid = xBlank(student.ID) || xPunct(student.ID) || stringlen(student.ID, 10);

			if (invalid)
				printf("\n\t> Please don't entered: space, punctuation characters, or entered more than 10 characters.\n");

			for (i = 0; i < strlen(student.ID); i++) {
				student.ID[i] = toupper(student.ID[i]);
			}

			if (strcmp(student.ID, "X") == 0) {
				CLEAR();
				return;
			}

			for (i = 0; i < studentCount; i++) {
				if (strcmp(student.ID, stud[i].ID) == 0) {
					printf("\n\n\tThis ID, \"%s\" existed in student records, please reenter.", student.ID);
					invalid = 1;
					break;
				}
			}


		} while (invalid);
	}
	//Enter password
	{
		do {
			invalid = 0;
			printf("\n\tPlease enter student password (max 10 characters) (enter 'x' to go back): ");
			scanf(" %s", &student.password);
			fflush(stdin);

			invalid = xBlank(student.password) || xPunct(student.password) || stringlen(student.password, 10);

			if (invalid)
				printf("\n\t> Please don't entered: space, punctuation characters, or entered more than 10 characters.\n");

			if (strcmp(student.password, "X") == 0 || strcmp(student.password, "x") == 0) {
				CLEAR();
				return;
			}

		} while (invalid);
	}

	//Enter programme
	{
		do {
			invalid = 0;
			printf("\n\tPlease enter student programme (max 3 characters) (enter 'x' to go back): ");
			scanf(" %[^\n]", &student.programme);
			fflush(stdin);

			invalid = xBlank(student.programme) || xPunct(student.programme) || stringlen(student.programme, 3) || xDigit(student.programme);

			if (invalid)
				printf("\n\t> Please don't entered: space, punctuation characters, digit, or entered more than 3 characters.\n");

			for (i = 0; i < strlen(student.programme); i++) {
				student.programme[i] = toupper(student.programme[i]);
			}

			if (strcmp(student.programme, "X") == 0) {
				CLEAR();
				return;
			}

		} while (invalid);
	}

	//Enter year of study
	{
		do {
			invalid = 0;
			printf("\n\tPlease enter student year Of study (1 to 3 only) (enter '-1' to go back): ");
			scanf(" %d", &student.yearOfStudy);

			while ((ch = getchar()) != '\n' && ch != EOF);

			if (student.yearOfStudy == -1) {
				CLEAR();
				return;
			}

			switch (student.yearOfStudy) {
			case 1:
			case 2:
			case 3:
				break;
			default:
				invalid = 1;
			}

			if (invalid)
				printf("\n\t> Please only enter 1 to 3.\n");

		} while (invalid);
	}

	//Enter gender
	{
		do {
			invalid = 0;
			printf("\n\tPlease enter student gender (M/F) (max 1 characters) (enter 'x' to go back): ");
			scanf(" %c", &student.gender);
			fflush(stdin);
			student.gender = toupper(student.gender);

			if (student.gender == 'X' || student.gender == 'x') {
				CLEAR();
				return;
			}

			switch (student.gender) {
			case 'M':
			case 'F':
				break;
			default:
				printf("\n\t> Please enter M or F only\n");
				while ((ch = getchar()) != '\n' && ch != EOF);
				invalid = 1;
			}
		} while (invalid);
	}

	//Enter contact number
	{
		do {
			invalid = 0;
			printf("\n\tPlease enter student contact number (enter '-' if unapplicable) (max 10 characters) (enter 'x' to go back): ");
			scanf(" %[^\n]", &student.contactNo);
			fflush(stdin);

			if (strcmp(student.contactNo, "X") == 0 || strcmp(student.contactNo, "x") == 0) {
				CLEAR();
				return;
			}

			invalid = xBlank(student.contactNo) || xAlpha(student.contactNo) || stringlen(student.contactNo, 10);

			if (invalid)
				printf("\n\t> Please don't entered: space, alphabetic character, or entered more than 10 characters.\n");

			for (i = 0; i < studentCount; i++) {
				if (strcmp(student.contactNo, "-") == 0) {
					invalid = 0;
					break;
				}
				else if (strcmp(student.contactNo, stud[i].contactNo) == 0) {
					printf("\n\n\tThis contact number, \"%s\" existed in student records, please reenter.", student.contactNo);
					invalid = 1;
					break;
				}
			}
		} while (invalid);
	}
	yesORno = NULL;

	//default value of number book borrowed
	student.noBorrowedBook = 0;

	//print the record will be save, ask confirmation
	printf("\n\n\t%-20s%-15s%-15s%-15s%-15s%-10s%-15s", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number");
	printf("\n\t========================================================================================================");
	printf("\n\t%-20s%-15s%-15s%-15s%-15d%-10c%-15s", student.name, student.ID, student.password, student.programme, student.yearOfStudy, student.gender, student.contactNo);
	do {
		invalid = 0;
		printf("\n\n\tAre you sure you want to save this student record? (Y/N): ");
		scanf(" %c", &yesORno);

		while ((ch = getchar()) != '\n' && ch != EOF);

		fflush(stdin);

		//Write new record to file
		if (tolower(yesORno) == 'y') {
			FILE*fptr;
			fptr = fopen("StudentData.bin", "ab");
			fwrite(&student, sizeof(Stud), 1, fptr);
			fclose(fptr);

			printf("\n\t> Record added! ");
			PAUSE();
		}
		else if (tolower(yesORno) == 'n') {
			printf("\n\t> Operation canceled. ");
		}
		else {
			invalid = 1;
			printf("\n\t> Please only enter Y or N. ");
		}
	} while (invalid);

	PAUSE();
	CLEAR();
}

void DeleteStudentRecord() {

	int studentCount = 0, i, choice, studentIndex;
	char yesORno, studentName[31], ch;

	//copy all data in file to array stud
	FILE*fptr;
	fptr = fopen("StudentData.bin", "rb");
	fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	while (!feof(fptr))
	{
		studentCount++;
		fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	}
	fclose(fptr);

	//Display student list
	CLEAR();
	DisplayStudentRecord();

	do {
		printf("\n\n\tPlease enter student name to delete (or enter 'x' to go back): ");
		scanf(" %[^\n]", studentName);

		for (i = 0; i < strlen(studentName); i++) {
			studentName[i] = toupper(studentName[i]);
		}

		if (strcmp(studentName, "X") == 0) {
			CLEAR();
			return;
		}

		for (i = 0; i < studentCount; i++) {
			if (strcmp(stud[i].name, studentName) == 0) {
				studentIndex = i;
				break;
			}
		}

		if (strcmp(stud[i].name, studentName) == 0)
			break;
		else
			printf("\n\t> Student name does not exist, please try again.");

	} while (strcmp(stud[i].name, studentName) != 0);

	printf("\n\t%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
	printf("\n\t==============================================================================================================================");
	printf("\n\t%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", stud[studentIndex].name, stud[studentIndex].ID, stud[studentIndex].password, stud[studentIndex].programme, stud[studentIndex].yearOfStudy, stud[studentIndex].gender, stud[studentIndex].contactNo, stud[studentIndex].noBorrowedBook);

	//Confirmation
	do {
		printf("\n\n\tAre you sure you want to delete %s student record? (Y/N): ", stud[studentIndex].name);
		scanf(" %c", &yesORno);

		while ((ch = getchar()) != '\n' && ch != EOF);

		if (tolower(yesORno) == 'y')
		{
			//Delete process
			for (i = studentIndex; i < studentCount; i++)
				stud[i] = stud[i + 1];

			printf("\n\t> Record deleted. ");
			PAUSE();

			//after delete one record, save to binary file
			fptr = fopen("StudentData.bin", "wb");
			for (i = 0; i < studentCount - 1; i++)
			{
				fwrite(&stud[i], sizeof(Stud), 1, fptr);
			}
			fclose(fptr);
			CLEAR();
		}
		else if (tolower(yesORno) == 'n') {
			printf("\n\t> Operation canceled. ");
			PAUSE();
			CLEAR();
		}
		else
			printf("\n\t> Please only enter Y or N");
	} while (tolower(yesORno) != 'n' && tolower(yesORno) != 'y');
}

void DisplayStudentRecord() {
	FILE*fptr;
	int counter = 0;

	fptr = fopen("StudentData.bin", "rb");
	printf("\n\tList of Student Record: "
		"\n\t-----------------------"
	);

	printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
	printf("\n\t==================================================================================================================================");

	//copy all data in file to array stud
	fread(&student, sizeof(Stud), 1, fptr);
	while (!feof(fptr))
	{
		printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, student.name, student.ID, student.password, student.programme, student.yearOfStudy, student.gender, student.contactNo, student.noBorrowedBook);
		fread(&student, sizeof(Stud), 1, fptr);
	}
	fclose(fptr);

	printf("\n");
}

void ModifyStudentRecordbyLibrarian(int a) {

	char newName[31], newPassword[11], newID[11], newProgramme[4], newGender, newContactNumber[11], yesORno, ch;
	char answer, studentName[31];

	int newYearOfStudy, invalid = 0, counter;
	int studentCount = 0, i, choice, studentIndex;

	FILE*fptr;
	fptr = fopen("StudentData.bin", "rb");
	fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	//copy all data in file to array stud
	while (!feof(fptr))
	{
		studentCount++;
		fread(&stud[studentCount], sizeof(Stud), 1, fptr);
	}
	fclose(fptr);

	int modifyWhat;
	char newContactNo[11];

	do {


		CLEAR();
		DisplayStudentRecord();

		do {
			printf("\n\n\tPlease enter student name to modify (or enter 'x' to go back): ");
			scanf(" %[^\n]", studentName);

			for (i = 0; i < strlen(studentName); i++) {
				studentName[i] = toupper(studentName[i]);
			}

			if (strcmp(studentName, "X") == 0) { CLEAR(); return; }
			for (i = 0; i < studentCount; i++) {
				if (strcmp(stud[i].name, studentName) == 0) {
					studentIndex = i;
					break;
				}
			}
			if (strcmp(stud[i].name, studentName) == 0)
				break;
			else
				printf("\n\t> Student name does not exist, please try again.");
		} while (strcmp(stud[i].name, studentName) != 0);

		do {
			do {
				//reset all variable data to prevent sentinel value remain in variable if user enter 'x' or -1 to go back.
				strcpy(newName, "");
				strcpy(newID, "");
				strcpy(newPassword, "");
				strcpy(newProgramme, "");
				strcpy(newContactNumber, "");
				newYearOfStudy = 0;
				newGender = NULL;

				CLEAR();
				DisplayStudentRecord();
				printf("\n\n\tList of Detail you can modify for student %s: ", stud[studentIndex].name);
				printf(
					"\n\t1. Name"
					"\n\t2. ID"
					"\n\t3. Password"
					"\n\t4. Programme"
					"\n\t5. Year of study"
					"\n\t6. Gender"
					"\n\t7. Contact number"
					"\n\t8. Go back"
					"\n\n\tPlease enter respective number of detail that you wish to modify: "
				);
				scanf(" %d", &modifyWhat);

				switch (modifyWhat) {

					//1. Modify name
				case 1: {
					CLEAR();
					printf("\n\tModify student %s's name", stud[studentIndex].name);

					yesORno = NULL;
					do {
						do {
							printf("\n\n\tPlease enter new name (enter 'x' to go back): ");
							scanf(" %[^\n]", &newName);
							fflush(stdin);

							invalid = xDigit(newName) || xPunct(newName) || stringlen(newName, 30);

							if (invalid)
								printf("\n\t> Please don't entered any digits, punctuation characters or entered more than 30 characters.");

						} while (invalid);

						for (i = 0; i < strlen(studentName); i++) {
							newName[i] = toupper(newName[i]);
						}

						if (strcmp(newName, "X") == 0)
							break;

						counter = 0;
						for (i = 0; i < studentCount; i++) {
							if (strcmp(newName, stud[i].name) == 0) {
								printf("\n\t%-4s%-20s%-15s%-15s%-15s%-15s%-10s%-20s%-20s", "No", "Name", "ID", "Password", "Programme", "Year Of Study", "Gender", "Contact Number", "No Borrowed Book");
								printf("\n\t==============================================================================================================================");
								printf("\n\t%-4d%-20s%-15s%-15s%-15s%-15d%-10c%-20s%-20d", ++counter, stud[i].name, stud[i].ID, stud[i].password, stud[i].programme, stud[i].yearOfStudy, stud[i].gender, stud[i].contactNo, stud[i].noBorrowedBook);
								do {
									printf("\n\n\tThis name, \"%s\" existed in student records, do you still want to continue (Y/N)? ", newName);

									scanf(" %c", &yesORno);
									while ((ch = getchar()) != '\n' && ch != EOF);

									if (tolower(yesORno) == 'y')
										break;
									else if (tolower(yesORno) == 'n') {
										invalid = 1;
										printf("\n\tPlease reenter student name ...");
									}
									else
										printf("\n\tPlease only enter Y or N!");
								} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
							}
							if (tolower(yesORno) == 'y')
								break;
						}
					} while (invalid);

					if (strcmp(newName, "X") == 0)
						break;

					do {
						printf("\n\n\tDo you really want to change %s's name to %s? (Y/N): ", stud[studentIndex].name, newName);
						scanf(" %c", &yesORno);
						while ((ch = getchar()) != '\n' && ch != EOF);

						if (tolower(yesORno) == 'y') {
							strcpy(stud[studentIndex].name, newName);
							printf("\n\n\t> Student's name has been changed! ");
							PAUSE();
						}
						else if (tolower(yesORno) == 'n')
							printf("\n\t> Operation canceled");
						else
							printf("\n\t> Please only enter Y or N");
					} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
					break;
				}

				//2. Modify ID
				case 2: {
					CLEAR();
					invalid = 0;
					printf("\n\tModify student %s's ID", stud[studentIndex].name);

					do {
						printf("\n\n\tPlease enter new ID (enter 'x' to go back): ");
						scanf(" %s", &newID);
						fflush(stdin);

						invalid = xBlank(newID) || xPunct(newID) || stringlen(newID, 10);

						if (invalid)
							printf("\n\t> Please don't entered: space, punctuation characters, or entered more than 10 characters.");

						for (i = 0; i < strlen(newID); i++) {
							newID[i] = toupper(newID[i]);
						}

						for (i = 0; i < studentCount; i++) {
							if (strcmp(newID, stud[i].ID) == 0) {
								printf("\n\n\tThis ID, \"%s\" existed in student records, please reenter.", newID);
								invalid = 1;
								break;
							}
						}

					} while (invalid);

					if (strcmp(newID, "X") == 0)
						break;

					do {
						printf("\n\n\tDo you really want to change %s's ID(%s) to %s? (Y/N): ", stud[studentIndex].name, stud[studentIndex].ID, newID);
						scanf(" %c", &yesORno);
						while ((ch = getchar()) != '\n' && ch != EOF);

						if (tolower(yesORno) == 'y') {
							strcpy(stud[studentIndex].ID, newID);
							printf("\n\n\t> Student's ID has been changed! ");
							PAUSE();
						}
						else if (tolower(yesORno) == 'n')
							printf("\n\t> Operation canceled");
						else
							printf("\n\t> Please only enter Y or N");
					} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
					break;
				}

								//3. Modify password
				case 3: {
					CLEAR();
					invalid = 0;
					printf("\n\tModify student %s's password", stud[studentIndex].name);

					do {
						printf("\n\n\tPlease enter new password (enter 'x' to go back): ");
						scanf(" %s", &newPassword);
						fflush(stdin);

						invalid = xBlank(newPassword) || xPunct(newPassword) || stringlen(newPassword, 10);

						if (invalid)
							printf("\n\t> Please don't entered: space, punctuation characters, or entered more than 10 characters.");

					} while (invalid);

					if (strcmp(newPassword, "x") == 0 || strcmp(newPassword, "X") == 0)
						break;

					do {
						printf("\n\n\tDo you really want to change %s's password(%s) to %s? (Y/N): ", stud[studentIndex].name, stud[studentIndex].password, newPassword);
						scanf(" %c", &yesORno);
						while ((ch = getchar()) != '\n' && ch != EOF);

						if (tolower(yesORno) == 'y') {
							strcpy(stud[studentIndex].password, newPassword);
							printf("\n\n\t> Student's password has been changed! ");
							PAUSE();
						}
						else if (tolower(yesORno) == 'n')
							printf("\n\t> Operation canceled");
						else
							printf("\n\t> Please only enter Y or N");
					} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
					break;
				}

								//4. Modify programme
				case 4:
					{
						system("cls");
						invalid = 0;
						printf("\n\tModify student %s's programme", stud[studentIndex].name);

						do
						{
							printf("\n\n\tPlease enter new programme (enter 'x' to go back): ");
							scanf(" %s", &newProgramme);
							fflush(stdin);

							invalid = xBlank(newProgramme) || xPunct(newProgramme) || stringlen(newProgramme, 3) || xDigit(newProgramme);

							if (invalid)
								printf("\n\t> Please don't entered: space, punctuation characters, digit, or entered more than 3 characters.");

						} while (invalid);

						for (i = 0; i < strlen(newProgramme); i++) {
							newProgramme[i] = toupper(newProgramme[i]);
						}

						if (strcmp(newProgramme, "X") == 0)
							break;

						do {
							printf("\n\n\tDo you really want to change %s's programme(%s) to %s? (Y/N): ", stud[studentIndex].name, stud[studentIndex].programme, newProgramme);
							scanf(" %c", &yesORno);
							while ((ch = getchar()) != '\n' && ch != EOF);

							if (tolower(yesORno) == 'y') {
								strcpy(stud[studentIndex].programme, newProgramme);
								printf("\n\n\t> Student's programme has been changed! ");
								PAUSE();
							}
							else if (tolower(yesORno) == 'n')
								printf("\n\t> Operation canceled");
							else
								printf("\n\t> Please only enter Y or N");
						} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
						break;
					}

								//5. Modify year of study
				case 5: {
					do {
						CLEAR();
						if (invalid) {
							printf("\n\t***You only allow change to 1, 2 or 3.\n");
						}

						invalid = 0;

						printf("\n\tModify student %s's year of study", stud[studentIndex].name);

						printf("\n\n\tPlease enter new year of study (1, 2 or 3 only) (enter -1 to go back): ");
						scanf(" %d", &newYearOfStudy);
						while ((ch = getchar()) != '\n' && ch != EOF);

						switch (newYearOfStudy) {
						case -1:
						case 1:
						case 2:
						case 3:
							break;
						default:
							invalid = 1;
						}
					} while (invalid);

					if (newYearOfStudy == -1)
						break;

					do {
						printf("\n\n\tDo you really want to change %s's year of study(%d) to %d? (Y/N): ", stud[studentIndex].name, stud[studentIndex].yearOfStudy, newYearOfStudy);
						scanf(" %c", &yesORno);
						while ((ch = getchar()) != '\n' && ch != EOF);

						if (tolower(yesORno) == 'y') {
							stud[studentIndex].yearOfStudy = newYearOfStudy;
							printf("\n\n\t> Student's year of study has been changed! ");
							PAUSE();
						}
						else if (tolower(yesORno) == 'n')
							printf("\n\t> Operation canceled ");
						else
							printf("\n\t> Please only enter Y or N");
					} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
					break;
				}

								//6. Modify gender
				case 6: {
					CLEAR();
					invalid = 0;
					printf("\n\tModify student %s's gender", stud[studentIndex].name);
					do
					{
						printf("\n\n\tPlease enter new gender (enter 'x' to go back): ");
						scanf(" %c", &newGender);
						while ((ch = getchar()) != '\n' && ch != EOF);

						newGender = toupper(newGender);

						switch (newGender) {
						case 'M':
						case 'F':
						case 'X':
							invalid = 0; // BREAK
							break;
						default:
							printf("\n\t> Please enter M or F only");
							invalid = 1;
						}

					} while (invalid);

					if (newGender == 'X')
						break;

					do
					{
						printf("\n\n\tDo you really want to change %s's gender(%c) to %c? (Y/N): ", stud[studentIndex].name, stud[studentIndex].gender, newGender);
						scanf(" %c", &yesORno);
						while ((ch = getchar()) != '\n' && ch != EOF);

						if (tolower(yesORno) == 'y') {
							stud[studentIndex].gender = newGender;
							printf("\n\n\t> Student's gender has been changed! ");
							PAUSE();
						}
						else if (tolower(yesORno) == 'n')
							printf("\n\t> Operation canceled");
						else
							printf("\n\t> Please only enter Y or N");
					} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
					break;
				}

								//7. Modify contact number
				case 7: {
					CLEAR();
					invalid = 0;
					printf("\n\tModify student %s's contact number", stud[studentIndex].name);

					do {
						printf("\n\n\tPlease enter new contact number (enter 'x' to go back): ");
						scanf(" %[^\n]", &newContactNumber);

						if (strcmp(newContactNumber, "x") == 0 || strcmp(newContactNumber, "X") == 0)
							break;

						invalid = xBlank(newContactNumber) || xAlpha(newContactNumber) || stringlen(newContactNumber, 10);

						if (invalid)
							printf("\n\t> Please don't entered: space, alphabetic character, or entered more than 10 characters.");

						for (i = 0; i < studentCount; i++) {
							if (strcmp(newContactNumber, "-") == 0) {
								invalid = 0;
								break;
							}
							else if (strcmp(newContactNumber, stud[i].contactNo) == 0) {
								printf("\n\n\tThis contact number, \"%s\" existed in student records, please reenter.", newContactNumber);
								invalid = 1;
								break;
							}
						}
					} while (invalid);

					if (strcmp(newContactNumber, "x") == 0 || strcmp(newContactNumber, "X") == 0)
						break;

					do
					{
						printf("\n\n\tDo you really want to change %s's contact number(%s) to %s? (Y/N): ", stud[studentIndex].name, stud[studentIndex].contactNo, newContactNumber);
						scanf(" %c", &yesORno);
						while ((ch = getchar()) != '\n' && ch != EOF);

						if (tolower(yesORno) == 'y') {
							strcpy(stud[studentIndex].contactNo, newContactNumber);
							printf("\n\n\t> Student's contact number has been changed! ");
							PAUSE();
						}
						else if (tolower(yesORno) == 'n')
							printf("\n\t> Operation canceled ");
						else
							printf("\n\t> Please only enter Y or N ");
					} while (tolower(yesORno) != 'y' && tolower(yesORno) != 'n');
					break;
				}

								//8. Go back
				case 8: {
					break;
				}

				default:
					printf("\n\t> Please enter number between 1 to 8");
				}
			} while (modifyWhat < 0 && modifyWhat > 9 || strcmp(newName, "X") == 0 || strcmp(newID, "X") == 0
				|| strcmp(newPassword, "x") == 0 || strcmp(newPassword, "X") == 0 || strcmp(newProgramme, "X") == 0
				|| newYearOfStudy == -1 || newGender == 'X' || strcmp(newContactNumber, "x") == 0 || strcmp(newContactNumber, "X") == 0);

			//after modify one data, save to binary file
			fptr = fopen("StudentData.bin", "wb");
			for (i = 0; i < studentCount; i++)
				fwrite(&stud[i], sizeof(Stud), 1, fptr);
			fclose(fptr);

			if (modifyWhat == 8)
				break;

			//Confirmation
			invalid = 0;
			do {
				printf("\n\n\t[Continue] to modify this student's information(Y/N)? ");
				scanf(" %c", &answer);
				while ((ch = getchar()) != '\n' && ch != EOF);

				if (toupper(answer) == 'Y' || toupper(answer) == 'N')
					break;
				else
					invalid = 1;

			} while (invalid);
		} while (toupper(answer) == 'Y');

	} while (modifyWhat == 8 || toupper(answer) == 'N');

	CLEAR();
	return;
}

//		while ((ch = getchar()) != '\n' && ch != EOF);
