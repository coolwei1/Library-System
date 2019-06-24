#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable:4996)

//not allow 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
int xDigit(char input[])
{
	int invalid = 0, i;
	for (i = 0; i < strlen(input); i++) {
		if (isdigit(input[i]) > 0)
		{
			invalid = 1;
			break;
		}
	}
	return invalid;
}

//allow digit
int Digit(char input)
{
	int invalid = 0;

	if (isdigit(input) > 0)
		invalid = 0;
	else if(isdigit(input) == 0)
		invalid = 1;
	return invalid;
}

//not allow ! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` {
int xPunct(char input[])
{
	int invalid = 0, i;
	for (i = 0; i < strlen(input); i++) {
		if (ispunct(input[i]) > 0)
		{
			invalid = 1;
			break;
		}
	}
	return invalid;
}

//not allow whitespace.
int xBlank(char input[])
{
	int invalid = 0, i;
	for (i = 0; i < strlen(input); i++) {
		if (isspace(input[i]) > 0)
		{
			invalid = 1;
			break;
		}
	}
	return invalid;
}

//not allow Lowercase letters and Uppercase letters.
int xAlpha(char input[])
{
	int invalid = 0, i;
	for (i = 0; i < strlen(input); i++) {
		if (isalpha(input[i]) > 0)
		{
			invalid = 1;
			break;
		}
	}
	return invalid;
}

//restrict the length of string entered by user.
int stringlen(char input[], int max) {
	int invalid = 0;
	if (strlen(input) > max)
	{
		invalid = 1;
	}
	return invalid;
}