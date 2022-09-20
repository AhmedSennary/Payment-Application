#include "stdio.h"
#include "terminal.h"
#include "card.h"
#include "string.h"
#include <time.h>
#include <stdlib.h>
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	int arr[4] = {0};
	time_t dat;
	struct tm nowDat;
	dat = time(NULL); //TIME NOW FROM WINDOWS
	nowDat = *localtime(&dat);  // LOCALTIME REUTNS THE DATE AND TIME TO VARIABLE NOWDAT
	//printf("%d", nowDat.tm_mon + 1);
	if (nowDat.tm_mday + 1 > 9) {
		termData->transactionDate[0] = (int)((nowDat.tm_mday ) / 10)+48;  //FROM NUMBER TO ASCII
		termData->transactionDate[1] = (int)((nowDat.tm_mday ) % 10)+48;   //FROM NUMBER TO ASCII
		termData->transactionDate[2] = '/'; // dd/mm/yy
	}
	else { 
		termData->transactionDate[0] = '0';        
		termData->transactionDate[1] = (nowDat.tm_mday + 1) + 48;		//FROM NUMBER TO ASCII
		termData->transactionDate[2] = '/';
	}
	if (nowDat.tm_mon > 9) {
		termData->transactionDate[3] = (int)((nowDat.tm_mon +1) / 10) + 48;  //FROM NUMBER TO ASCII
		termData->transactionDate[4] = (int)((nowDat.tm_mon +1) % 10) + 48;  //FROM NUMBER TO ASCII
		termData->transactionDate[5] = '/';
	}
	else {
		termData->transactionDate[3] = '0';
		termData->transactionDate[4] = (int)((nowDat.tm_mon + 1) % 10) + 48;   //FROM NUMBER TO ASCII
		termData->transactionDate[5] = '/';
	}
	termData->transactionDate[6] = '2';
	termData->transactionDate[7] = '0';
	termData->transactionDate[8] = (int)(((nowDat.tm_year) % 100)/10) + 48;  //FROM NUMBER TO ASCII
	termData->transactionDate[9] = (int)(((nowDat.tm_year) % 100) % 10) + 48;  //FROM NUMBER TO ASCII

	//puts(termData->transactionDate);
	return Ok;
}


EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {

	if (((int)((cardData.cardExpirationDate[3] - 48) * (10) + (cardData.cardExpirationDate[4] - 48))) > ((int)((termData.transactionDate[8] - 48) * (10) + (termData.transactionDate[9] - 48))))
		// YEAR COMPARISON year card > year now
		return Ok;
	else if (((int)((cardData.cardExpirationDate[3] - 48) * (10) + (cardData.cardExpirationDate[4] - 48))) == ((int)((termData.transactionDate[8] - 48) * (10) + (termData.transactionDate[9] - 48))))
		// IF NOW YEAR = CARD YEAR
		if (((int)((cardData.cardExpirationDate[0] - 48) * (10) + (cardData.cardExpirationDate[1] - 48))) >= ((int)((termData.transactionDate[3] - 48) * (10) + (termData.transactionDate[4] - 48))))
			//MONTH COMPARISON month now < month card
			return Ok;
	return EXPIRED_CARD;
}


EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
	int luhan[20] = { 0 };
	int PAN_length;
	PAN_length= strlen(cardData->primaryAccountNumber);
	for (int i = 0; i < PAN_length; i++)
	{
		luhan[i] = cardData->primaryAccountNumber[i];
		if (cardData->primaryAccountNumber[i] >= 65 && cardData->primaryAccountNumber[i] <= 90) {
			luhan[i] = cardData->primaryAccountNumber[i] - 65;
			if (luhan[i] >= 10 && luhan[i] <= 19) luhan[i] -= 10;
			else if (luhan[i] >= 20 && luhan[i] <= 26) luhan[i] -= 20;
		}
		else if (cardData->primaryAccountNumber[i] >= 97 && cardData->primaryAccountNumber[i] <= 122) {
			luhan[i] = cardData->primaryAccountNumber[i] - 97;
			if (luhan[i] >= 10 && luhan[i] <= 19) luhan[i] -= 10;
			else if (luhan[i] >= 20 && luhan[i] <= 26) luhan[i] -= 20;
		}
		else if (cardData->primaryAccountNumber[i] >= 48 && cardData->primaryAccountNumber[i] <= 57) {
			luhan[i] = cardData->primaryAccountNumber[i] - 48;
			if (luhan[i] >= 10 && luhan[i] <= 19) luhan[i] -= 10;
			else if (luhan[i] >= 20 && luhan[i] <= 26) luhan[i] -= 20;
		}
	}
	int sum = 0;
	if (!(PAN_length % 2)) {
		for (int i = 0; i < PAN_length - 1; i++)
		{
			//printf("%d", luhan[i]);
			if (!(i % 2)) {
				luhan[i] = luhan[i] * 2;
				if (luhan[i] > 9) {
					int d1 = 0;
					int d2 = 0;
					d1 = luhan[i] % 10;
					luhan[i] /= 10;
					d2 = (int)luhan[i];
					luhan[i] = d1 + d2;
				}
			}
			//printf("%d", luhan[i]);
			sum = sum + luhan[i];
		}
	}
	else
	{
		for (int i = 0; i < PAN_length - 1; i++)
		{
			//printf("%d", luhan[i]);
			if ((i % 2) && (i !=0)) {
				luhan[i] = luhan[i] * 2;
				if (luhan[i] > 9) {
					int d1 = 0;
					int d2 = 0;
					d1 = luhan[i] % 10;
					luhan[i] /= 10;
					d2 = (int)luhan[i];
					luhan[i] = d1 + d2;
				}
			}
			//printf("%d", luhan[i]);
			sum = sum + luhan[i];
		}
	}	
	//printf("%d", sum);
	int check_value = (((sum / 10) * 10) + 10) - sum;
	//printf("%d", check_value);
	if (luhan[PAN_length - 1] != check_value)
		return INVALID_CARD;
	else return Ok;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("Please Enter the Transcation Amount: ");
	scanf("%f",&(termData->transAmount));
	if ((termData->transAmount) <= 0.0)
		return INVALID_AMOUNT;
	else return Ok;
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	termData->maxTransAmount = 10000.0;
	return Ok;
}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {

	if (termData->maxTransAmount < termData->transAmount)
		return EXCEED_MAX_AMOUNT;
	else return Ok;
}