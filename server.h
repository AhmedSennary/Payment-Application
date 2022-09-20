#ifndef server
#define server
#include "terminal.h"

typedef unsigned long uint32_t;
typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
	int place;
}ST_transaction_t;



typedef enum EN_serverError_t
{
	ok, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;




EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData,int *place);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData,int place);
EN_serverError_t saveTransaction(ST_transaction_t* transData, int feedback);
void filecpy(void);
//EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);
#endif // !app