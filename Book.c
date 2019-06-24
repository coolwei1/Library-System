#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Windows.h>
#pragma warning(disable:4996)

void Add_Book(), Modify_Book(), Display_Book(), Search_Book(), Delete_Book();
int BookCounter();

FILE*fptr;

typedef struct {
	char booktitle[100], bookID[7], publisher[31], author[21];
	int yearpublish;
	char status;
}Book;
Book b;
Book BOOK[100];

void Add_Book() {
	fptr = fopen("Book.txt", "a");     // open the file for appendix
	int a = 0, i;
	char useranswer, response;
	printf("\n\t< Add Book >\n");

	printf("\tPlease enter the Book title       : ");             // prompt the user to enter the book title
	scanf(" %[^\n]", BOOK[a].booktitle);
	printf("\tPlease enter the book ID          : ");               // prompt the user to enter the book ID
	scanf(" %[^\n]", BOOK[a].bookID);
	printf("\tPlease enter the publisher        : ");        // prompt the user to enter the book publisher
	scanf(" %[^\n]", BOOK[a].publisher);
	printf("\tPlease enter year of publish      : ");           // prompt the user to enter the year of the publish of the book
	scanf(" %d", &BOOK[a].yearpublish);
	printf("\tPlease enter the name of author   : ");    // prompt the user to enter the author who written the book
	scanf(" %[^\n]", BOOK[a].author);

	BOOK[a].status = 'E'; //'E' stand for exist, 'B' stand for borrowed

												// Show the detail that the user just entered 
	printf("\n\tThe Book Title                    : %s\n", BOOK[a].booktitle);
	printf("\tThe Book ID                       : %s\n", BOOK[a].bookID);
	printf("\tThe Book Publisher                : %s\n", BOOK[a].publisher);
	printf("\tThe Year of Publish               : %d\n", BOOK[a].yearpublish);
	printf("\tThe Book author                   : %s\n", BOOK[a].author);

	do {
		printf("\n\tAre you sure you want to enter the record ???(Y for yes and N for no) : ");  // comfirm again with the user
		scanf(" %c", &useranswer);
		useranswer = tolower(useranswer);      // convert it to lower case
		switch (useranswer) {
		case 'y':
			printf("\n\t> Processing");
			for (i = 0; i < 3; i++)
			{
				Sleep(500);
				printf(".");
			}
			fprintf(fptr, "%s|%s|%s|%s|%d|%c\n", BOOK[a].booktitle, BOOK[a].bookID, BOOK[a].publisher, BOOK[a].author, BOOK[a].yearpublish, BOOK[a].status); // add the book to the text file
			printf("\n\n\tAdd Book Completed");
			break;
		case 'n':
			printf("\n\tcancel record\n");   // cancel to write into the text file
			break;
		default:
			printf("\n\t> Please enter Y or N only");  // the answer has to yes or no
		}
		fclose(fptr);
	} while (useranswer != 'y' && useranswer != 'n');

	printf("\n\n\tDo you still want to enter another book record :(Y for yes and N for no) : ");  // ask the user whether they want to add another book or not
	scanf(" %c", &response);
	response = tolower(response);   // convert it to lower case
	switch (response) {
	case 'y':
		printf("\n");
		Add_Book();    // run the function again
		break;
	case 'n':           // break out from the switch 
		break;
	default:
		printf("\n\t> Please enter Y or N only");   // only accept yes or no
	}

}

void Modify_Book() {
	char bookid[10], useranswer;
	int response, i, bookNo, book_index;
	printf("\n\t< Modify Book >\n");
	do {
		do {

			Display_Book();             // Display the book in the text file
			bookNo = BookCounter();

			// Prompt the user to enter the the book id that the book they want to modify 
			printf("\n\tPlease enter ID of the book that you want to modify ('x' to exit) : ");
			scanf(" %[^\n]", bookid);

			// Convert book id into upper case
			for (i = 0; i < strlen(bookid); i++)
			{
				bookid[i] = toupper(bookid[i]);
			}

			if (strcmp(bookid, "X") == 0)   // exit the function
				return;

			// Print out the detail of the book that the user chosen
			for (i = 0; i < bookNo; i++)
			{
				if (strcmp(bookid, BOOK[i].bookID) == 0)
				{
					book_index = i;
					printf("\n\t%s|%s|%s|%s|%d\n", BOOK[i].booktitle, BOOK[i].bookID, BOOK[i].publisher, BOOK[i].author, BOOK[i].yearpublish);
					break;
				}
			}

			// if can not find the book 
			if (strcmp(bookid, BOOK[i].bookID) != 0)
			{
				system("cls");
				printf("\n\t> Invalid ID\n");
			}


		} while (strcmp(bookid, BOOK[i].bookID) != 0);

		do {
			system("cls");
			Display_Book();

			// show the number to let the user to choose which section they want to modify
			printf("\n\n\tModify \"%s\" book:", BOOK[book_index].booktitle);
			printf("\n\n\t1. Book Title\n");
			printf("\t2. Book ID\n");
			printf("\t3. Book Publisher\n");
			printf("\t4. Book Author\n");
			printf("\t5. Book Year Publish\n");
			printf("\t6. Exit\n");

			// Get the response from the user
			printf("\n\t> Please enter the number between (1~6) : ");
			scanf(" %d", &response);

			switch (response)
			{
			case 1:
				printf("\n\tPlease enter the Book Title you want to change to : ");    //Prompt the user to enter what they want to change to
				scanf(" %[^\n]", b.booktitle);
				printf("\n\t> It will change from %s to %s\n", BOOK[book_index].booktitle, b.booktitle);
				printf("\tAre you sure you want to change the record ???(Y for yes and N for no) : ");   // ask the user to comfirm again
				scanf(" %c", &useranswer);
				useranswer = tolower(useranswer); // convet into lower case
				switch (useranswer) {
				case 'y':
					strcpy(BOOK[book_index].booktitle, b.booktitle);   //Copy into the array
					printf("\n\tChanged!");
					break;
				case 'n':
					printf("\n\t> Operation cancelled.");
					break;
				default:
					printf("\n\t> Please enter Y or N only");
				}
				break;
			case 2:
				printf("\n\tPlease enter the Book ID you want to change to : "); //Prompt the user to enter what they want to change to
				scanf(" %[^\n]", b.bookID);
				printf("\n\t> It will change from %s to %s\n", BOOK[book_index].bookID, b.bookID);
				printf("\n\tAre you sure you want to change the record ???(Y for yes and N for no)");  // ask the user to comfirm again
				scanf(" %c", &useranswer);
				useranswer = tolower(useranswer);    // convet into lower case
				switch (useranswer) {
				case 'y':
					strcpy(BOOK[book_index].bookID, b.bookID);      //Copy into the array
					printf("\n\t> Changed!");
					break;
				case 'n':
					break;
				default:
					printf("\n\t> Please enter Y or N only");
				}
				break;
			case 3:
				printf("\n\tPlease enter the Book Publisher you want to change to : ");   //Prompt the user to enter what they want to change to
				scanf(" %[^\n]", b.publisher);
				printf("\n\t> It will change from %s to %s", BOOK[book_index].publisher, b.publisher);
				printf("\n\tAre you sure you want to change the record ???(Y for yes and N for no)");  // ask the user to comfirm again
				scanf(" %c", &useranswer);
				useranswer = tolower(useranswer);   // convet into lower case
				switch (useranswer) {
				case 'y':
					strcpy(BOOK[book_index].publisher, b.publisher);   //Copy into the array
					printf("\n\t> Changed!");
					break;
				case 'n':
					break;
				default:
					printf("\n\t> Please enter Y or N only");
				}
				break;
			case 4:
				printf("\tPlease enter the Book Author you want to change to : ");   //Prompt the user to enter what they want to change to
				scanf(" %[^\n]", b.author);
				printf("\n\t> It will change from %s to %s", BOOK[book_index].author, b.author);
				printf("\n\tAre you sure you want to change the record ???(Y for yes and N for no)");  // ask the user to comfirm again
				scanf(" %c", &useranswer);
				useranswer = tolower(useranswer);   // convet into lower case
				switch (useranswer) {
				case 'y':
					strcpy(BOOK[book_index].author, b.author);   //Copy into the array
					printf("\n\t> Changed!");
					break;
				case 'n':
					break;
				default:
					printf("\n\t> Please enter Y or N only");
				}
				break;
			case 5:
				printf("\tPlease enter the Book Year Publish you want to change to : ");   //Prompt the user to enter what they want to change to
				scanf(" %d", &b.yearpublish);
				printf("\n\t> It will change from %d to %d", BOOK[book_index].yearpublish, b.yearpublish);
				printf("\n\tAre you sure you want to change the record ???(Y for yes and N for no)");  // ask the user to comfirm again
				scanf(" %c", &useranswer);
				useranswer = tolower(useranswer);   // convet into lower case
				switch (useranswer) {
				case 'y':
					BOOK[book_index].yearpublish = b.yearpublish;   //Copy into the array
					printf("\n\tChanged!");
					break;
				case 'n':
					break;
				default:
					printf("\n\t> Please enter Y or N only");
				}
				break;

			case 6:        // exit the loop
				break;
			default:
				printf("\n\t> Please enter 1 to 6 only");
			}

			fptr = fopen("Book.txt", "w");

			// Rewrite it into text file
			for (i = 0; i < bookNo; i++)
			{
				fprintf(fptr, "%s|%s|%s|%s|%d|%c\n", BOOK[i].booktitle, BOOK[i].bookID, BOOK[i].publisher, BOOK[i].author, BOOK[i].yearpublish, BOOK[i].status);
			}

			fclose(fptr);

			system("cls");

		} while (response != 6);

		system("cls");

	} while (strcmp(bookid, "X") != 0);

}

void Display_Book() {
	int a = 0;
	char status[9];

	fptr = fopen("Book.txt", "r");

	// Printf the title 
	printf("\n\tBook List\n\n");
	printf("\t%-60s%-20s%-30s%-25s%-20s%-20s\n", "Book Title", "Book ID", "Book Publisher", "Book Author", "Book Year Publish", "Status");
	printf("\t===================================================================================================================================================================\n");

	// Scan the book from text file and print it
	while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", BOOK[a].booktitle, BOOK[a].bookID, BOOK[a].publisher, BOOK[a].author, &BOOK[a].yearpublish, &BOOK[a].status) != EOF) {
		if (BOOK[a].status == 'E')
			strcpy(status, "Exist");
		else
			strcpy(status, "Borrowed");
		printf("\t%-60s%-20s%-30s%-25s%-20d%-20s\n", BOOK[a].booktitle, BOOK[a].bookID, BOOK[a].publisher, BOOK[a].author, BOOK[a].yearpublish, status);
		a++;
	}

	fclose(fptr);
}

int BookCounter() {
	int bookCounter = 0;

	fptr = fopen("Book.txt", "r");

	// Count the book in the text file
	while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", BOOK[bookCounter].booktitle, BOOK[bookCounter].bookID, BOOK[bookCounter].publisher, BOOK[bookCounter].author, &BOOK[bookCounter].yearpublish, &BOOK[bookCounter].status) != EOF) {
		bookCounter++;
	}

	fclose(fptr);

	return bookCounter;  // return the value
}

void Search_Book()
{
	char response1[10], response2;
	int i, a = 0;

	fptr = fopen("Book.txt", "r");

	//Scan the book from the text file into array
	do
	{
		fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", BOOK[a].booktitle, BOOK[a].bookID, BOOK[a].publisher, BOOK[a].author, &BOOK[a].yearpublish, &BOOK[a].status);
		a++;
	} while (!feof(fptr));
	printf("\n\t< Search Book >\n");
	do
	{
		printf("\tPlease enter the book ID to search :"); // prompt the user to enter the book ID they want to search
		scanf(" %[^\n]", response1);

		// Convert the book ID into Upper Case
		for (i = 0; i < strlen(response1); i++)
		{
			response1[i] = toupper(response1[i]);
		}

		for (a = 0; a < 100; a++)
		{
			if (strcmp(response1, BOOK[a].bookID) == 0)

			{

				printf("\n\tBook Tiltle          :%s\n", BOOK[a].booktitle);
				printf("\tBook ID              :%s\n", BOOK[a].bookID);
				printf("\tBook Publisher       :%s\n", BOOK[a].publisher);
				printf("\tBook Author          :%s\n", BOOK[a].author);
				printf("\tBook year of publish :%d\n", BOOK[a].yearpublish);
				printf("\tBook status is       :%c\n", BOOK[a].status);
				break;
			}
		}

		// If the book did not match with all the book inside the text file 
		if (strcmp(response1, BOOK[a].bookID) != 0)
		{
			printf("\n\t> Invalid ID\n\n");
		}

	} while (strcmp(response1, BOOK[a].bookID) != 0);

	// ask the user would they want to seacrh again
	printf("\n\tDo you still want to search another book??? :(Y for yes and N for no) :");
	scanf(" %c", &response2);

	response2 = tolower(response2); // convert into lower case

	switch (response2)
	{
	case 'y':
		system("cls");
		Search_Book();
		break;
	case 'n':
		break;
	default:
		printf("\n\t> Please enter Y or N only");
	}

	fclose(fptr);

}

void Delete_Book()
{
	int bookindex = 0, i, bookNo = 0;
	char response1[10], response2;

	Display_Book();  // Display the book in the text file

	fptr = fopen("Book.txt", "r");
	// Scan it into array
	do
	{
		fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%c\n", BOOK[bookNo].booktitle, BOOK[bookNo].bookID, BOOK[bookNo].publisher, BOOK[bookNo].author, &BOOK[bookNo].yearpublish, &BOOK[bookNo].status);
		bookNo++;
	} while (!feof(fptr));

	fclose(fptr);

	printf("\n\t< Delete Book >\n");

	do
	{
		// Prompt the user to enter the book id that the book they wish to delete
		printf("\n\tPlease enter the book ID of the book you wish to delete ('x' to exit) :");
		scanf(" %[^\n]", response1);

		// Convert into upper case
		for (i = 0; i < strlen(response1); i++)
		{
			response1[i] = toupper(response1[i]);
		}

		if (strcmp(response1, "X") == 0)
			return;

		// Compare the book in the array
		for (i = 0; i < bookNo; i++)
		{
			if (strcmp(BOOK[i].bookID, response1) == 0)
			{
				bookindex = i;
				break;
			}
		}
		if (strcmp(response1, BOOK[bookindex].bookID) == 0)
			break;
		else
			printf("\n\t Book id does not exist, please try again.");
	} while (strcmp(response1, BOOK[i].bookID) != 0);

	// Print out the detail of the book
	printf("\n\tBook Tiltle          :%s\n", BOOK[bookindex].booktitle);
	printf("\tBook ID              :%s\n", BOOK[bookindex].bookID);
	printf("\tBook Publisher       :%s\n", BOOK[bookindex].publisher);
	printf("\tBook Author          :%s\n", BOOK[bookindex].author);
	printf("\tBook year of publish :%d\n", BOOK[bookindex].yearpublish);
	printf("\tBook status is       :%c\n", BOOK[bookindex].status);

	// Comfirm again with the user
	do {
		printf("\n\tAre you sure you want to delete this book ???(Y for yes and N for no) :");
		scanf(" %c", &response2);
		response2 = tolower(response2);   // Convert into lower case
		switch (response2) {
		case 'y':
			for (i = bookindex; i <= bookNo; i++)
			{
				BOOK[i] = BOOK[i + 1];
			}

			printf("\n\t> Processing");
			for (i = 0; i < 3; i++)
			{
				Sleep(200);
				printf(".");
			}

			printf("\n\t> Record deleted.\n");
			PAUSE();

			fptr = fopen("Book.txt", "w");

			// Copy again into the text file
			for (i = 0; i < bookNo - 1; i++)
			{
				fprintf(fptr, "%s|%s|%s|%s|%d|%c\n", BOOK[i].booktitle, BOOK[i].bookID, BOOK[i].publisher, BOOK[i].author, BOOK[i].yearpublish, BOOK[i].status);
			}

			fclose(fptr);
			break;
		case 'n':
			break;
		default:
			printf("\n\t> Please enter Y or N only");
		}
	} while (response2 != 'n' && response2 != 'y');
}