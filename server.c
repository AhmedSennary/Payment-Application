#include "server.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "app.h"


EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	if (!(isValidAccount(&transData->cardHolderData, &transData->place))) { //!(isValidAccount(&transData->cardHolderData, &place))
		if (!(isAmountAvailable(&transData->terminalData, transData->place))) { //!(isAmountAvailable(&transData->terminalData, place))
			FILE* input_file;
			FILE* output_file;
			input_file = fopen("Accounts DB.txt", "r");
			output_file = fopen("file.txt", "w");
			char line[40] = {'\0'};
			char BalDB[40] = { '\0' };
			int start = 0;
			float bal = 0.0;
			for (int i = 1; i <= transData->place; i++)
			{
				fgets(line, sizeof(line), input_file);
				if (i != transData->place) fputs(line, output_file);
			}
			for (int j = 0; j < 21; j++) {
				if (line[j] == ',') {
					if (line[j + 1] == ',') return DECLINED_STOLEN_CARD;
					start = j + 1;
					break;
				}
			}
			for (int k = 0; k < sizeof(line) - start - 2; k++) BalDB[k] = line[start + k];
			BalDB[strlen(BalDB) - 1] = '\0';
			bal = atof(BalDB);
			bal = bal - transData->terminalData.transAmount;
			sprintf(BalDB, "%f", bal);
			line[start] = '\0';
			strcat(line, BalDB);
			strcat(line, "\n");
			fputs(line, output_file);
			for (int i = 0; fgets(line, sizeof(line), input_file) != NULL; i++)
			{
				fputs(line, output_file);
			}
			fflush(output_file);
			fflush(input_file);
			return ok;
		}
		else if(isAmountAvailable(&transData->terminalData, transData->place))
		{
			return DECLINED_INSUFFECIENT_FUND;
		}
	}
	else return DECLINED_STOLEN_CARD;
}


EN_serverError_t isValidAccount(ST_cardData_t* cardData, int* place) {
	FILE* input_file;
	input_file = fopen("Accounts DB.txt", "r");
	char line[35];
	for (int i = 1; fgets(line, sizeof(line), input_file) != NULL; i++)
	{
		char PanDB[20]={'\0'};
		//printf("%s", line);
		for (int j = 0; j < 20; j++) {
			if (line[j] == ',') break;
			PanDB[j] = line[j];
		}
		if (!(strcmp(PanDB, cardData->primaryAccountNumber))) {
			*place = i;
			return ok;
		}
	}
	return DECLINED_STOLEN_CARD;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData , int place) {
	FILE* input_file; 
	input_file = fopen("Accounts DB.txt", "r");
	char line[35] = {'\0'};
	char BalDB[15] = { '\0' };
	int start = 0;
	float bal = 0.0;
	for (int i = 1; i <= place; i++)
	{
		fgets(line, sizeof(line), input_file);
		//printf("%s", line);
	}
	for (int j = 0; j < 21; j++) {
		if (line[j] == ',') {
			if (line[j + 1] == ',') {
				start = j + 2;
				break;
			}
			else {
				start = j + 1;
				break;
			}
		}
	}
	for  (int k = 0; k <sizeof(line)-start-2; k++) BalDB[k] = line[start + k];
	BalDB[strlen(BalDB)-1] = '\0';
	//puts(BalDB);
	bal = atof(BalDB);
	if (bal >= termData->transAmount)
		return ok;
	return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData ,int feedback) {
	FILE *output_file;
	FILE* output_file_temp;
	char name[] = "Transaction DB";
	char temp[] = "temp2";
	char line[60];
	char line_out[60] = {'\0'};
	char amount[7]={'\0'};
	char trans1[11];
	output_file = fopen("Transaction DB.txt", "a+");
	for (int i = 1; fgets(line, sizeof(line), output_file); i++);
	fputs("\n", output_file);
	transData->transactionSequenceNumber = strtol(line, NULL, 10);
	(transData->transactionSequenceNumber)++;
	itoa(transData->transactionSequenceNumber, trans1, 10);
	strcat(line_out, trans1);
	strcat(line_out, ",");
	strcat(line_out, transData->cardHolderData.primaryAccountNumber);
	strcat(line_out, ",");
	itoa(transData->terminalData.transAmount, amount, 10);
	strcat(line_out, amount);
	if (feedback == 0) {
		int a = fputs(line_out, output_file);
		printf("Transaction Suceeded: ");
		printf("%lu", transData->transactionSequenceNumber);
		printf("\n");
	}
	else if (feedback == 2) {
		strcat(line_out,",StolenCard");
		int a = fputs(line_out, output_file);
		printf("Transaction Failed: ");
		printf("%lu", transData->transactionSequenceNumber);
		printf("\n");
	}
	else if (feedback == 1) {
		strcat(line_out, ",NoFund");
		int a = fputs(line_out, output_file);
		printf("Transaction Failed: ");
		printf("%lu", transData->transactionSequenceNumber);
		printf("\n");
	}
	
	
	return ok;

}



void filecpy(void) {
	FILE* input_file;
	FILE* output_file;
	input_file =  fopen("file.txt", "r");
	output_file = fopen("Accounts DB.txt", "w");
	char line[40];
	for (int i = 0; fgets(line, sizeof(line), input_file) != NULL; i++)
	{
		fputs(line, output_file);
	}
}
