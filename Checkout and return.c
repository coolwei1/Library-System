/*
problem:
cannot save number of book borrowed after borrowing book, keep 0;
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Windows.h> //to get system time
#pragma warning(disable:4996)

void Borrow_Return_Menu(int), calculateDueDate(), Check_In(int), Check_Out(int);
void Display_CheckOut_Record_Student(int), Display_CheckIn_Record_Student(int);
void Display_CheckOut_Record_Librarian(), Display_CheckIn_Record_Librarian();
int CalculateFine(int, int, int, int, int, int, double *, int *);

SYSTEMTIME t;

typedef struct {
	int date, month, year;
}Date;
Date Return, Due;
Date borrow[100], due[100], RETURN[100];//reading the data from Out.txt and In.txt to the array

typedef struct {
	char BOOK_ID[50], BOOK_NAME[50], STUDENTID[50];
	int MON, DAY, YEAR, MON1, DAY1, YEAR1;
}CHECKOUT;
CHECKOUT checkoutData[100];//reading the data from out.txt to the array

typedef struct {
	char BOOK_ID[50], BOOK_NAME[50], STUDENTID[50];
	int MON, DAY, YEAR;
}CHECKIN;
CHECKIN checkinData[100];

FILE*BORROWtxt;
FILE*RETURNtxt;
FILE*BOOKtxt;
FILE*STUDENTbin;

typedef struct {
	char name[31], programme[4];
	int yearOfStudy, noBorrowedBook;
	char ID[11], password[11], contactNo[11], gender;
}Stud;
Stud student[100];//array for the data of studentdata.bin

typedef struct {
	char ID[10], name[30], author[30], publisher[30];
	int year;
	char status;
}book;
book Book[100];//array for the data of book.txt

char book_id[10], answer, d, studentid[11], e;
int BREAK, i = 0, RESPONSE, error, X, day, i, bookindex;
double fee;




void Borrow_Return_Menu(int StudentIndex)
{
	do
	{
		CLEAR();

		printf("\n\t< Check Out and Check In Menu >\n");
		printf("\n\t1. Check Out");
		printf("\n\t2. Check In");
		printf("\n\t3. Exit");
		printf("\n\tPlease enter your choice: ");
		scanf(" %d", &RESPONSE);//asking user for option
		switch (RESPONSE)
		{
		case 1:
		{
			CLEAR();
			Check_Out(StudentIndex);
			break;
		}
		case 2:
		{
			CLEAR();
			do
			{
				Check_In(StudentIndex);
				printf("\n\tContinue(Y/N)? ");
				scanf(" %c", &answer);

				if (toupper(answer) == 'Y')
					CLEAR();
			} while (toupper(answer) == 'Y');
			break;
		}
		case 3:return;
		default:printf("\n\tInvalid choice");
		}

		STUDENTbin = fopen("StudentData.bin", "wb");
		for (i = 0; i < 26; i++)
			fwrite(&student[i], sizeof(Stud), 1, STUDENTbin);
		fclose(STUDENTbin);

	} while (RESPONSE != 3);
	return;
}

void Check_Out(int StudentIndex)
{
	int count = 0, invalid = 0, BookIndex = 0, BookCount = 0, StudCount = 0;
	char ch;

	BOOKtxt = fopen("Book.txt", "r");

	do
	{
		fscanf(BOOKtxt, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", Book[BookCount].name, Book[BookCount].ID, Book[BookCount].publisher, Book[BookCount].author, &Book[BookCount].year, &Book[BookCount].status);//saving the data from book.txt to array
		BookCount++;
	} while (!feof(BOOKtxt));

	fclose(BOOKtxt);//copy the data from Book.txt to array


	STUDENTbin = fopen("StudentData.bin", "rb");
	fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin); 	//copy all data in file to array stud
	while (!feof(STUDENTbin))
	{
		++StudCount;
		fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin);
	}
	fclose(STUDENTbin);

	GetLocalTime(&t);

	CLEAR();

	printf("\n\t< Check Out >");
	if (student[StudentIndex].noBorrowedBook > 5)
	{
		invalid = 1;
	}

	printf("\n\t* Enter \"x\" to back\n");
	printf("\n\tEnter the book ID: ");
	scanf(" %s", book_id);

	for (i = 0; i < strlen(book_id); i++)// Change all character to uppercase
		book_id[i] = toupper(book_id[i]);

	for (count = 0; count < BookCount; count++) {
		if (strcmp(book_id, Book[count].ID) == 0) {
			BookIndex = count;
		}
		else
			invalid = 1;
	}

	if (strcmp(book_id, Book[BookIndex].ID) != 0) {
		printf("\tInvalid Id\n");
		PAUSE();
		return;
	}

	if (strcmp(book_id, Book[BookIndex].ID) == 0) {
		invalid = 0;
	}

	if (Book[BookIndex].status == 'E')//book existed
		invalid = 0;
	else if (Book[BookIndex].status == 'B') {
		printf("\n\tThis book has been borrowed!\n");
		PAUSE();
		return;
	}
	do {
		if (!invalid) {
			printf("\n\tThe Book Title                    : %s\n", Book[BookIndex].name);
			printf("\tThe Book ID                       : %s\n", Book[BookIndex].ID);
			printf("\tThe Book Publisher                : %s\n", Book[BookIndex].publisher);
			printf("\tThe Year of Publish               : %d\n", Book[BookIndex].year);
			printf("\tThe Book author                   : %s\n", Book[BookIndex].author);

			printf("\n\tIs this book(Y/N)? ");
			scanf(" %c", &answer);

			while ((ch = getchar()) != '\n' && ch != EOF);

			if (toupper(answer) == 'Y')
			{
				calculateDueDate();

				//writing data to the out.txt
				BORROWtxt = fopen("Out.txt", "a");
				fprintf(BORROWtxt, "%s|%s|%s|%d-%d-%d|%d-%d-%d\n", Book[BookIndex].name, Book[BookIndex].ID, student[StudentIndex].ID, t.wDay, t.wMonth, t.wYear, Due.date, Due.month, Due.year);
				fclose(BORROWtxt);

				//changing the book status to B, Borrowed
				Book[BookIndex].status = 'B';

				//writing the new data into Book.txt
				BOOKtxt = fopen("Book.txt", "w");
				for (count = 0; count < BookCount; count++)
					fprintf(BOOKtxt, "%s|%s|%s|%s|%d|%c\n", Book[count].name, Book[count].ID, Book[count].publisher, Book[count].author, Book[count].year, Book[count].status);
				fclose(BOOKtxt);

				//adding the number of borrow book of the student
				student[StudentIndex].noBorrowedBook += 1;

				printf("\n\tThis is your new record: ");
				printf("\n\tName: %s", student[StudentIndex].name);
				printf("\n\tNumber of book borrowed: %d", student[StudentIndex].noBorrowedBook);

				//after complete one function, save to binary file
				STUDENTbin = fopen("StudentData.bin", "wb");
				for (count = 0; count < StudCount; count++)
					fwrite(&student[count], sizeof(Stud), 1, STUDENTbin);
				fclose(STUDENTbin);

				printf("\n\n\t> Successful. ");
				PAUSE();
			}
			else if (toupper(answer) == 'N')
			{
				printf("\n\tOperation canceled");
				return;
			}
			else
				printf("\n\tPlease only enter Y or N");
		}
		else
			printf("\n\tError occured!");
	} while (toupper(answer) != 'N' && toupper(answer) != 'Y');
}

void Check_In(int StudentIndex)
{
	int i = 0, j = 0, k = 0, count = 0, OutCount, validToReturn = 1, forCheckInCounter, bookWantToReturn;
	int payFine, deleteIndex, dayOverdue, BookCount = 0, StudCount = 0;
	double fine;

	BOOKtxt = fopen("Book.txt", "r");

	do
	{
		fscanf(BOOKtxt, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", Book[BookCount].name, Book[BookCount].ID, Book[BookCount].publisher, Book[BookCount].author, &Book[BookCount].year, &Book[BookCount].status);//saving the data from book.txt to array
		BookCount++;
	} while (!feof(BOOKtxt));

	fclose(BOOKtxt);//copy the data from Book.txt to array


	STUDENTbin = fopen("StudentData.bin", "rb");
	fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin); 	//copy all data in file to array stud

	while (!feof(STUDENTbin))
	{
		++StudCount;
		fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin);
	}
	fclose(STUDENTbin);

	CHECKOUT forCheckIn[5];
	typedef struct
	{
		char BookID[7], StudentID[11];
	}CheckIn;
	CheckIn CIN;

	OutCount = 0;

	BORROWtxt = fopen("Out.txt", "r");
	do
	{
		fscanf(BORROWtxt, "%[^|]|%[^|]|%[^|]|%d-%d-%d|%d-%d-%d\n", checkoutData[OutCount].BOOK_NAME, checkoutData[OutCount].BOOK_ID, checkoutData[OutCount].STUDENTID, &checkoutData[OutCount].DAY, &checkoutData[OutCount].MON, &checkoutData[OutCount].YEAR, &checkoutData[OutCount].DAY1, &checkoutData[OutCount].MON1, &checkoutData[OutCount].YEAR1);//saving data from out.txt to array
		OutCount++;
	} while (!feof(BORROWtxt));
	fclose(BORROWtxt);


	forCheckInCounter = 0;
	for (i = 0; i < OutCount; i++) {
		if (strcmp(student[StudentIndex].ID, checkoutData[i].STUDENTID) == 0)
		{
			forCheckIn[forCheckInCounter] = checkoutData[i];
			forCheckInCounter++;
		}
	}

	if (forCheckInCounter == 0)
	{
		printf("\n\tYou have no book to return. ");
		PAUSE();
		return;
	}
	else
	{
		for (i = 0; i < forCheckInCounter; i++)
		{
			printf("\n\tRecord %d:", i + 1);
			printf("\n\t----------");
			printf("\n\tBook Name: %s", forCheckIn[i].BOOK_NAME);
			printf("\n\tBook ID  : %s\n", forCheckIn[i].BOOK_ID);
			printf("\n\tDue date : %02d-%02d-%02d", forCheckIn[i].DAY1, forCheckIn[i].MON1, forCheckIn[i].YEAR1);
			printf("\n\t-------------------------\n");
		}
	}

	printf("\n\tEnter Book ID for the book you want to return: ");
	scanf(" %s", book_id);
	for (i = 0; i < strlen(book_id); i++)
		book_id[i] = toupper(book_id[i]);

	for (i = 0; i < forCheckInCounter; i++)
	{
		if (strcmp(forCheckIn[i].BOOK_ID, book_id) == 0) {
			validToReturn = 1;
			bookWantToReturn = i;
			break;
		}
		else
			validToReturn = 0;
	}

	if (validToReturn) {
		payFine = CalculateFine(forCheckIn[bookWantToReturn].DAY, forCheckIn[bookWantToReturn].MON,
			forCheckIn[bookWantToReturn].YEAR, forCheckIn[bookWantToReturn].DAY1,
			forCheckIn[bookWantToReturn].MON1, forCheckIn[bookWantToReturn].YEAR1,
			&fine, &dayOverdue);
	}
	else {
		printf("\n\tThis book is unable to return.");
		PAUSE();
		return;
	}

	if (payFine) { //if need to pay fine,
		double cashPayment, balance;

		do {
			printf("\n\tYour book has overdue %d days, the fine you need to pay is %.2f", dayOverdue, fine);
			printf("\n\n\tCash Payment (enter -999 to cancel): RM ");
			scanf("%lf", &cashPayment);

			if (cashPayment == -999)									//user can enter -999 to quit program
			{
				printf("\n\tThank you for visiting us !!!");
				PAUSE();
				exit(0);
			}
			if (cashPayment < 0)										//user is not allowed to enter negative integer
			{
				printf("\n\tPlease enter positive integer only.");
			}
		} while (cashPayment < 0);
		balance = cashPayment - fine;
		if (balance >= 0)									//If user paid enough cash, show the change due.
		{
			printf("\n\tChange Due		=  RM %.2f", balance);
		}
		else if (balance < 0)								//If user paid not enough cash,
		{															//prompt user paid more cash.
			do
			{
				printf("\n\tYou have to pay more RM%6.2f !", fabs(balance));
				//fabs() function used here to temporary change
				printf("\n\tCash Payment		=  RM ");	//negative sign to positive sign for showing to user.
				scanf("%lf", &cashPayment);							//math header file must include to user fabs() function.
				if (cashPayment == -999)									//user can enter -999 to quit program
				{
					printf("\n\tThank you for visiting us !!!");
					PAUSE();
					exit(0);
				}
				if (cashPayment < 0)
				{
					printf("\n\tPlease enter positive integer only.");
				}
				//assign value in balance (which is negative value) to fine
				fine = balance;
				//So, balance can be calculate again by adding cash paid with fine.
				//For example, 200 (cash payment) + - 198 (total have to pay) = 2 (change due)
				balance = cashPayment + fine;
				fine = balance;					//Updating the total have to pay
			} while (balance < 0 || cashPayment < 0);
		}
		payFine = 0;

	}

	if (payFine == 0 || !payFine) { //if no need pay fine,

		RETURNtxt = fopen("In.txt", "a");
		fprintf(RETURNtxt, "%s|%s|%s|%d-%d-%d\n", forCheckIn[bookWantToReturn].BOOK_NAME, forCheckIn[bookWantToReturn].BOOK_ID, forCheckIn[bookWantToReturn].STUDENTID, t.wDay, t.wMonth, t.wYear);
		fclose(RETURNtxt);


		for (i = 0; i < OutCount; i++) {
			if (strcmp(forCheckIn[bookWantToReturn].BOOK_ID, checkoutData[i].BOOK_ID) == 0) {
				deleteIndex = i;
			}
		}

		for (i = deleteIndex; i < OutCount; i++) {
			checkoutData[i] = checkoutData[i + 1];
		}

		BORROWtxt = fopen("Out.txt", "w");//open Out.txt for writting

		for (i = 0; i < OutCount - 1; i++)//replacing new data to Out.txt
			fprintf(BORROWtxt, "%s|%s|%s|%d-%d-%d|%d-%d-%d\n", checkoutData[i].BOOK_NAME, checkoutData[i].BOOK_ID, checkoutData[i].STUDENTID, checkoutData[i].DAY, checkoutData[i].MON, checkoutData[i].YEAR, checkoutData[i].DAY1, checkoutData[i].MON1, checkoutData[i].YEAR1);
		fclose(BORROWtxt);


		for (i = 0; i < BookCount; i++) {
			if (strcmp(forCheckIn[bookWantToReturn].BOOK_ID, Book[i].ID) == 0)
				Book[i].status = 'E';				//change back the book status into E
		}

		BOOKtxt = fopen("Book.txt", "w");//change the new status for the book
		for (i = 0; i < BookCount; i++)
			fprintf(BOOKtxt, "%s|%s|%s|%s|%d|%c\n", Book[i].name, Book[i].ID, Book[i].publisher, Book[i].author, Book[i].year, Book[i].status);
		fclose(BOOKtxt);


		student[StudentIndex].noBorrowedBook -= 1;//reducing the no of borrow book of the student


		STUDENTbin = fopen("StudentData.bin", "wb"); //after complete one function, save to binary file
		for (i = 0; i < StudCount; i++)
		{
			fwrite(&student[i], sizeof(Stud), 1, STUDENTbin);
		}
		fclose(STUDENTbin);

		printf("\n\n\t> Return sucessfully!\n");

		printf("\n\tThis is your new record: ");
		printf("\n\tName: %s", student[StudentIndex].name);
		printf("\n\tNumber of book borrowed: %d\n", student[StudentIndex].noBorrowedBook);

	}
}

void calculateDueDate()//calculate the due date
{
	GetLocalTime(&t);//to get system time
	Due.date = t.wDay + 7;//to calculate the due date
	Due.month = t.wMonth;//to calculate the due month
	Due.year = t.wYear;//to calculate the due year

	if (Due.year % 4 == 0)//to know whether the year is leap year
	{
		switch (Due.month)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		{ // month for 31days
			if (Due.date > 31)
			{
				Due.date -= 31;
				Due.month += 1;
				if (Due.month > 12)
				{
					Due.month -= 12;
					Due.year += 1;
				}
			}
		}
		case 2:
		{//month for 28 days
			if (Due.date > 29)
			{
				Due.date -= 29;
				Due.month += 1;
			}
		}
		case 4:
		case 6:
		case 9:
		case 11:
		{//month for 30 days
			if (Due.date > 30)
			{
				Due.date -= 30;
				Due.month += 1;
			}
		}
		}
	}
	else
	{
		switch (Due.month)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		{//month for 31 days
			if (Due.date > 31)
			{
				Due.date -= 31;
				Due.month += 1;
				if (Due.month > 12)
				{
					Due.month -= 12;
					Due.year += 1;
				}
			}
		}
		case 2:
		{//month for 29 days
			if (Due.date > 28)
			{
				Due.date -= 28;
				Due.month += 1;
			}
		}
		case 4:
		case 6:
		case 9:
		case 11:
		{//month for 30 days
			if (Due.date > 30)
			{
				Due.date -= 30;
				Due.month += 1;
			}
		}
		}
	}
}

void Display_CheckOut_Record_Student(int StudentIndex)//display check out record function
{
	int StudCount = 0, BookCount = 0;
	BOOKtxt = fopen("Book.txt", "r");

	do
	{
		fscanf(BOOKtxt, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", Book[BookCount].name, Book[BookCount].ID, Book[BookCount].publisher, Book[BookCount].author, &Book[BookCount].year, &Book[BookCount].status);//saving the data from book.txt to array
		BookCount++;
	} while (!feof(BOOKtxt));

	fclose(BOOKtxt);//copy the data from Book.txt to array


	STUDENTbin = fopen("StudentData.bin", "rb");
	fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin); 	//copy all data in file to array stud

	while (!feof(STUDENTbin))
	{
		++StudCount;
		fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin);
	}
	fclose(STUDENTbin);

	CLEAR();
	CHECKOUT forDisplay[5];
	int count, OutCount, forCheckOutCounter;

	OutCount = 0;
	BORROWtxt = fopen("Out.txt", "r");
	do {
		fscanf(BORROWtxt, "%[^|]|%[^|]|%[^|]|%d-%d-%d|%d-%d-%d\n", checkoutData[OutCount].BOOK_NAME, checkoutData[OutCount].BOOK_ID, checkoutData[OutCount].STUDENTID, &checkoutData[OutCount].DAY, &checkoutData[OutCount].MON, &checkoutData[OutCount].YEAR, &checkoutData[OutCount].DAY1, &checkoutData[OutCount].MON1, &checkoutData[OutCount].YEAR1);//saving data from out.txt to array
		OutCount++;
	} while (!feof(BORROWtxt));//copy the data from Out.txt to the array
	fclose(BORROWtxt);


	forCheckOutCounter = 0;
	for (i = 0; i < OutCount; i++) {
		if (strcmp(student[StudentIndex].ID, checkoutData[i].STUDENTID) == 0)
		{
			forDisplay[forCheckOutCounter] = checkoutData[i];
			forCheckOutCounter++;
		}
	}

	if (forCheckOutCounter == 0) {
		printf("\n\tNo record found! ");
		PAUSE();
		return;
	}

	printf("\n\n\tBorrowed book detail: ");
	printf("\n\t===================== ");
	printf("\n\t%-50s%-10s%-15s%-15s%-15s\n", "Book Name", "Book ID", "Student ID", "Borrow Date", "Due Date");
	printf("\t%-50s%-10s%-15s%-15s%-15s\n", "---------", "-------", "----------", "-----------", "--------");

	for (i = 0; i < forCheckOutCounter; i++)
		printf("\t%-50s%-10s%-15s%02d-%02d-%02d     %02d-%02d-%02d\n", forDisplay[i].BOOK_NAME, forDisplay[i].BOOK_ID, forDisplay[i].STUDENTID,
			forDisplay[i].DAY, forDisplay[i].MON, forDisplay[i].YEAR, forDisplay[i].DAY1, forDisplay[i].MON1, forDisplay[i].YEAR1);

	PAUSE();
}

void Display_CheckIn_Record_Student(int StudentIndex)//display return record
{
	int StudCount = 0, BookCount = 0;
	BOOKtxt = fopen("Book.txt", "r");

	do
	{
		fscanf(BOOKtxt, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", Book[BookCount].name, Book[BookCount].ID, Book[BookCount].publisher, Book[BookCount].author, &Book[BookCount].year, &Book[BookCount].status);//saving the data from book.txt to array
		BookCount++;
	} while (!feof(BOOKtxt));

	fclose(BOOKtxt);//copy the data from Book.txt to array


	STUDENTbin = fopen("StudentData.bin", "rb");
	fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin); 	//copy all data in file to array stud

	while (!feof(STUDENTbin))
	{
		++StudCount;
		fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin);
	}
	fclose(STUDENTbin);

	CLEAR();
	CHECKIN forDisplay[5];
	int count, InCount, forCheckInCounter;

	InCount = 0;
	RETURNtxt = fopen("In.txt", "r");
	do {
		fscanf(RETURNtxt, "%[^|]|%[^|]|%[^|]|%d-%d-%d\n", checkinData[InCount].BOOK_NAME, checkinData[InCount].BOOK_ID, checkinData[InCount].STUDENTID, &checkinData[InCount].DAY, &checkinData[InCount].MON, &checkinData[InCount].YEAR);//saving data from out.txt to array
		InCount++;
	} while (!feof(RETURNtxt));//copy the data from Out.txt to the array
	fclose(RETURNtxt);


	forCheckInCounter = 0;
	for (i = 0; i < InCount; i++) {
		if (strcmp(student[StudentIndex].ID, checkinData[i].STUDENTID) == 0)
		{
			forDisplay[forCheckInCounter] = checkinData[i];
			forCheckInCounter++;
		}
	}

	if (forCheckInCounter == 0) {
		printf("\n\tNo record found! ");
		PAUSE();
		return;
	}

	printf("\n\n\tReturned book detail: ");
	printf("\n\t===================== ");
	printf("\n\t%-50s%-10s%-15s%-15s\n", "Book Name", "Book ID", "Student ID", "Return Date");
	printf("\t%-50s%-10s%-15s%-15s\n", "---------", "-------", "----------", "-----------");

	for (i = 0; i < forCheckInCounter; i++)
		printf("\t%-50s%-10s%-15s%02d-%02d-%02d\n", forDisplay[i].BOOK_NAME, forDisplay[i].BOOK_ID, forDisplay[i].STUDENTID,
			forDisplay[i].DAY, forDisplay[i].MON, forDisplay[i].YEAR);

	PAUSE();
}

void Display_CheckOut_Record_Librarian() {
	int StudCount = 0, BookCount = 0;
	BOOKtxt = fopen("Book.txt", "r");

	do
	{
		fscanf(BOOKtxt, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", Book[BookCount].name, Book[BookCount].ID, Book[BookCount].publisher, Book[BookCount].author, &Book[BookCount].year, &Book[BookCount].status);//saving the data from book.txt to array
		BookCount++;
	} while (!feof(BOOKtxt));

	fclose(BOOKtxt);//copy the data from Book.txt to array


	STUDENTbin = fopen("StudentData.bin", "rb");
	fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin); 	//copy all data in file to array stud

	while (!feof(STUDENTbin))
	{
		++StudCount;
		fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin);
	}
	fclose(STUDENTbin);

	CLEAR();
	CHECKOUT forDisplay[5];
	int count, OutCount, forCheckOutCounter;

	OutCount = 0;
	BORROWtxt = fopen("Out.txt", "r");

	printf("\n\n\tBorrowed book detail: ");
	printf("\n\t===================== ");
	printf("\n\t%-4s%-50s%-10s%-15s%-15s%-15s\n", "No", "Book Name", "Book ID", "Student ID", "Borrow Date", "Due Date");
	printf("\t%-4s%-50s%-10s%-15s%-15s%-15s\n", "--", "---------", "-------", "----------", "-----------", "--------");

	while (fscanf(BORROWtxt, "%[^|]|%[^|]|%[^|]|%d-%d-%d|%d-%d-%d\n", checkoutData[OutCount].BOOK_NAME, checkoutData[OutCount].BOOK_ID, checkoutData[OutCount].STUDENTID,
		&checkoutData[OutCount].DAY, &checkoutData[OutCount].MON, &checkoutData[OutCount].YEAR, &checkoutData[OutCount].DAY1, &checkoutData[OutCount].MON1, &checkoutData[OutCount].YEAR1) != EOF) {//saving data from out.txt to array){

		printf("\t%-4d%-50s%-10s%-15s%02d-%02d-%02d     %02d-%02d-%02d\n", OutCount + 1, checkoutData[OutCount].BOOK_NAME, checkoutData[OutCount].BOOK_ID, checkoutData[OutCount].STUDENTID,
			checkoutData[OutCount].DAY, checkoutData[OutCount].MON, checkoutData[OutCount].YEAR, checkoutData[OutCount].DAY1, checkoutData[OutCount].MON1, checkoutData[OutCount].YEAR1);

		OutCount++;
	}
	fclose(BORROWtxt);

	printf("\n\n\t%d record(s) found. ", OutCount);

	if (OutCount == 0) {
		CLEAR();
		printf("\n\tNo record found! ");
		PAUSE();
		return;
	}

	PAUSE();

}


void Display_CheckIn_Record_Librarian() {
	int StudCount = 0, BookCount = 0;
	BOOKtxt = fopen("Book.txt", "r");

	do
	{
		fscanf(BOOKtxt, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", Book[BookCount].name, Book[BookCount].ID, Book[BookCount].publisher, Book[BookCount].author, &Book[BookCount].year, &Book[BookCount].status);//saving the data from book.txt to array
		BookCount++;
	} while (!feof(BOOKtxt));

	fclose(BOOKtxt);//copy the data from Book.txt to array


	STUDENTbin = fopen("StudentData.bin", "rb");
	fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin); 	//copy all data in file to array stud

	while (!feof(STUDENTbin))
	{
		++StudCount;
		fread(&student[StudCount], sizeof(Stud), 1, STUDENTbin);
	}
	fclose(STUDENTbin);

	CLEAR();
	CHECKIN forDisplay[5];
	int count, InCount, forCheckInCounter;

	InCount = 0;
	RETURNtxt = fopen("In.txt", "r");

	printf("\n\n\tReturned book detail: ");
	printf("\n\t===================== ");
	printf("\n\t%-4s%-50s%-10s%-15s%-15s\n", "No", "Book Name", "Book ID", "Student ID", "Return Date");
	printf("\t%-4s%-50s%-10s%-15s%-15s\n", "--", "---------", "-------", "----------", "-----------");

	while (fscanf(RETURNtxt, "%[^|]|%[^|]|%[^|]|%d-%d-%d\n", checkinData[InCount].BOOK_NAME, checkinData[InCount].BOOK_ID, checkinData[InCount].STUDENTID,
		&checkinData[InCount].DAY, &checkinData[InCount].MON, &checkinData[InCount].YEAR) != EOF) {//saving data from out.txt to array

		printf("\t%-4d%-50s%-10s%-15s%02d-%02d-%02d\n", InCount + 1, checkinData[InCount].BOOK_NAME, checkinData[InCount].BOOK_ID, checkinData[InCount].STUDENTID,
			checkinData[InCount].DAY, checkinData[InCount].MON, checkinData[InCount].YEAR);
		InCount++;

	} while (!feof(RETURNtxt));//copy the data from Out.txt to the array
	fclose(RETURNtxt);

	printf("\n\n\t%d record(s) found. ", InCount);

	if (InCount == 0) {
		CLEAR();
		printf("\n\tNo record found! ");
		PAUSE();
		return;
	}

	PAUSE();
}


int CalculateFine(int borrowDay, int borrowMonth, int borrowYear, int dueDay, int dueMonth, int dueYear, double *fine, int *day) {
	int payFine = 0;
	GetLocalTime(&t);//to get system time

	if (t.wDay > dueDay && t.wMonth <= dueMonth && t.wYear <= dueYear) {
		payFine = 1;
		*fine = (t.wDay - dueDay)*0.50;
		*day = t.wDay - dueDay;
	}
	else if (t.wMonth > dueMonth || t.wYear > dueYear) {
		printf("\n\tPlease approach librarian for assistance.");
		PAUSE();
		return;
	}
	return payFine;
}