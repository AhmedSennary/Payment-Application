#include "stdio.h"
#include "card.h"
#include "string.h"
EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	printf("Please Enter Your Card Name: ");
	gets(cardData->cardHolderName);
	int name_length = strlen(cardData->cardHolderName);   
	//printf("%d",name_length);
	if (name_length > 24 || name_length	<20)  // CHECKING LENTGTH WHETHER IT IS WITHING 20 TO 24 CHARS OR NOT
		return WRONG_NAME;
	else
		return OK;
	}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	printf("Please Enter Card Expiration Date(MM/YY): ");
	gets(cardData->cardExpirationDate);
	int date_length = strlen(cardData->cardExpirationDate);
	//printf("%d", date_length);
	for (int i = 0; i < 5; i++) 
	{
		if (i == 2)
			i++;
		if (cardData->cardExpirationDate[i] < 48 || cardData->cardExpirationDate[i] > 57) { // CHECK THAT ALL THE DIGITS EXCPET DIGIT 3 ARE NUMBERS
			return WRONG_EXP_DATE;
		}
	}
	if (date_length > 5 || date_length < 5 || cardData->cardExpirationDate[2] != '/') { // CHECK LENGTH OF DATE AND CHAR "/"
		return WRONG_EXP_DATE;
	}
	else
		return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	int counter =0;
	printf("Please Enter PAN: ");
	gets(cardData->primaryAccountNumber);
	int PAN_length = strlen(cardData->primaryAccountNumber);
	//puts(cardData->primaryAccountNumber);
	if (PAN_length < 16 || PAN_length > 19) {
		return WRONG_PAN;
	}


	for (int i = 0; i < PAN_length; i++) // CHECK THAT THE PAN ARE NUMBERS AND ALPHAPETIC
	{
		if (cardData->primaryAccountNumber[i] >= 65 && cardData->primaryAccountNumber[i] <= 90); 
		else if (cardData->primaryAccountNumber[i] >= 97 && cardData->primaryAccountNumber[i] <= 122);
		else if (cardData->primaryAccountNumber[i] >= 48 && cardData->primaryAccountNumber[i] <= 57);
		else return WRONG_PAN;
	}
	return OK;
}