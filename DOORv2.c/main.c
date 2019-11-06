#define _CRT_SECURE_NO_WARNINGS	
#include <stdio.h>
#include <stdlib.h>
#include "safeinput.h"
#include <stdbool.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include "serial.h"




typedef struct
{
	int number;
	bool access;
	struct tm date;
}CARD;


typedef struct {
	CARD* allCard;
	int totalCard;
	SERIALPORT port;
}SYSTEM_STATE;

void RemoteDoor(SYSTEM_STATE* state) {
	SerialWritePort(state->port, "1", sizeof(char));
	printf("\nCURRENT LAMP IS: green");
	Sleep(3000);
	system("cls");
}

void GetNumBetween(int* selection, int min, int max) {
	while (true) {
		GetInputInt("\nVal ", selection);
		if (*selection >= min && *selection <= max)
			return;
		else
			printf("Chose between %d and %d", min, max);
	}
}


bool GetAccess(int access) {
	if (access == 1) {
		return true;
	}
	else if (access == 2) {
		return false;
	}
}


int charToInt(char* scan) {
	for (int i = 0; i < strlen(scan); i++) {
		if (isdigit(scan[i]) == 0)
			return NULL;
	}
	int card;
	sscanf(scan, "%d", &card);
	return card;


}

CARD* GetCardNum(int num, SYSTEM_STATE* state) {
	for (int i = 0; i < state->totalCard; i++) {
		if (num == state->allCard[i].number)
			return &state->allCard[i];
	}
	return NULL;
}

void List(SYSTEM_STATE* state) {
	printf("***Cards in system***\n");
	printf("Total amount of cards: %d", state->totalCard);
	for (int i = 0; i < state->totalCard; i++) {
		printf("\nCardNo; %d, %s, Card added to system; %d-%02d-%02d %02d:%02d ", state->allCard[i].number, state->allCard[i].access ? "access" : "no access",
			state->allCard[i].date.tm_year + 1900, state->allCard[i].date.tm_mon + 1, state->allCard[i].date.tm_mday, state->allCard[i].date.tm_hour, state->allCard[i].date.tm_min);
	}
	getch();
	system("cls");
}


void AddCard(SYSTEM_STATE* state) {
	printf("Add Card");
	printf("\nCard Number");
	int newCard;
	GetNumBetween(&newCard, 1000, 9999);

	CARD* cardToEdit = GetCardNum(newCard, state);
	if (cardToEdit == NULL)
	{
		int index;
		if (state->totalCard == 0) {
			state->allCard = malloc(sizeof(CARD) * 1);
			state->totalCard = 1;
			index = 0;
		}
		else {
			state->allCard = realloc(state->allCard, sizeof(CARD) * (state->totalCard + 1));
			index = state->totalCard;
			state->totalCard++;
		}
		state->allCard[index].number = newCard;
		int access;
		printf("\n1 for access 2 for no access");
		GetNumBetween(&access, 1, 2);
		state->allCard[index].access = GetAccess(access);
		time_t t = time(NULL);
		state->allCard[index].date = *localtime(&t);
		if (access == 1) {
			char cardToA[20];
			sprintf(cardToA, "<ADDCARD%d>", newCard);
			SerialWritePort(state->port, cardToA, sizeof(cardToA));
			
		}
	}
	else
	{
		printf("\nYou have %s. ", cardToEdit->access ? "access" : "no access");
		int access;
		printf("\n1 for access 2 for no access");
		GetNumBetween(&access, 1, 2);
		cardToEdit->access = GetAccess(access);
	}
	system("cls");

}




void FakeTest(SYSTEM_STATE* state) {
	printf("SCAN CARD\n");

	char scan[15];
	char scanToA[15];
	GetInput("Scan Card\n", &scan, sizeof(scan));
	strcpy(scanToA, "<TEST");
	strcat(scanToA, scan);
	strcat(scanToA, ">");
	SerialWritePort(state->port, scanToA, sizeof(scanToA));
	return;
}




void CardOnAarduino(SYSTEM_STATE* state){
	SerialWritePort(state->port, "<LIST>", strlen("<LIST>"));
	char buf[50];
	SerialReadPort(state->port, &buf, 50);
	printf("%s", buf);
	return;

}


void Huvudmeny(SYSTEM_STATE* state)
{
	while (true)
	{
		printf("\n***ADMIN MENY***\n");
		printf("1. Remote open door\n2. List all cards in system\n3. Add/remove access\n4. Exit\n5. FAKE TEST SCAN\n6. Cards on Arduino\n7. Clear list on Arduino");
		int selection = 0;
		if (!GetInputInt("\nAnge val:>", &selection))
			continue;
		switch (selection)
		{
		case 1:
			system("cls");
			RemoteDoor(state);
			break;
		case 2:
			system("cls");
			List(state);
			break;
		case 3:
			system("cls");

			AddCard(state);
			break;
		case 4:
			system("cls");
			return;
		case 5:
			system("cls");
			FakeTest(state);
			break;
		case 6:
			system("cls");
			CardOnAarduino(state);
		case 7:
			system("cls");
			SerialWritePort(state->port, "<CLEAR>", strlen("<CLEAR>"));

		}

	}

}


void main() {

	
	SYSTEM_STATE state;
	state.port= SerialInit("\\\\.\\COM5");
	state.allCard = NULL;
	state.totalCard = 0;
	if (!SerialIsConnected(state.port))
	{
		return;
	}
	Huvudmeny(&state);

}