#include "stdio.h"
#include "app.h"
#include "card.h"
#include "server.h"
#include "terminal.h"
#include "string.h"
#include "stdlib.h"
#include "windows.h"
void appStart()
{

	int feedback_Name; 
	int feedback_Date;
	int feedback_PAN;
	int feedback_Expired;
	int feedback_Valid;
	int feedback_Amount;
	int feedback_sucess;
	int feedback_below;
	ST_transaction_t data = { {"","",""},{0,0,""} };
	data.place = 0;
	//---------------------------------------------------------------------------------------------------

	PlaySound(TEXT("CardName.wav.wav"), NULL, SND_FILENAME|SND_ASYNC);
	feedback_Name = getCardHolderName(&data.cardHolderData);
	while (feedback_Name != 0) {
		PlaySound(TEXT("WNAME.wav"), NULL, SND_FILENAME | SND_ASYNC);
		printf("Wrong Name, Please re-enter CardName\n\n");
		feedback_Name = getCardHolderName(&data.cardHolderData);
	}
	
	//----------------------------------------------------------------------------------------------------

	PlaySound(TEXT("ExpDate.wav"), NULL, SND_FILENAME | SND_ASYNC);
	feedback_Date = getCardExpiryDate(&data.cardHolderData);
	while (feedback_Date != 0)
	{
		PlaySound(TEXT("WEXPIRE.wav"), NULL, SND_FILENAME | SND_ASYNC);
		printf("Wrong Expiry Date format, Please re-enter Expiry Date\n\n");
		feedback_Date = getCardExpiryDate(&data.cardHolderData);
	}

	//----------------------------------------------------------------------------------------------------
	
	PlaySound(TEXT("PAN.wav"), NULL, SND_FILENAME | SND_ASYNC);
	feedback_PAN = getCardPAN(&data.cardHolderData);
	while (feedback_PAN !=0)
	{
		PlaySound(TEXT("WPAN.wav"), NULL, SND_FILENAME | SND_ASYNC);
		printf("Wrong PAN, Please re-enter PAN\n\n");
		feedback_PAN = getCardPAN(&data.cardHolderData);
	}

	//---------------------------------------------------------------------------------------------------

	getTransactionDate(&data.terminalData);

	//---------------------------------------------------------------------------------------------------

	feedback_Expired = isCardExpired(data.cardHolderData, data.terminalData);
	if (feedback_Expired !=0)
	{
		printf("\nEXPIRED CARD!");
		PlaySound(TEXT("beep.wav"), NULL, SND_FILENAME | SND_SYNC);
		PlaySound(TEXT("EXP.wav"), NULL, SND_FILENAME | SND_SYNC);
		return 0;
	}

	//---------------------------------------------------------------------------------------------------
	
	feedback_Valid = isValidCardPAN(&data.cardHolderData);
	if (feedback_Valid != 0)
	{
		printf("\nINVALID CARD!");
		PlaySound(TEXT("beep.wav"), NULL, SND_FILENAME | SND_SYNC);
		PlaySound(TEXT("invalid.wav"), NULL, SND_FILENAME | SND_SYNC);
		return 0;
	}
	
	//---------------------------------------------------------------------------------------------------

	setMaxAmount(&data.terminalData);

	//---------------------------------------------------------------------------------------------------

	PlaySound(TEXT("EAmount.wav"), NULL, SND_FILENAME | SND_ASYNC);
	feedback_Amount = getTransactionAmount(&data.terminalData);
	feedback_below = isBelowMaxAmount(&data.terminalData);
	while ((feedback_Amount != 0) || (feedback_below !=0))
	{	
		if (feedback_Amount != 0)
		{
			PlaySound(TEXT("Amount.wav"), NULL, SND_FILENAME | SND_ASYNC);
			printf("INVALID AMOUNT, Please re-enter transaction amount\n\n");
			feedback_Amount = getTransactionAmount(&data.terminalData);
			feedback_below = isBelowMaxAmount(&data.terminalData);
		}
		if (feedback_below != 0)
		{
			PlaySound(TEXT("allow.wav"), NULL, SND_FILENAME | SND_ASYNC);
			printf("Exceeded Max. Allaowbale Amount, Please re-enter transaction amount\n\n");
			feedback_Amount = getTransactionAmount(&data.terminalData);
			feedback_below = isBelowMaxAmount(&data.terminalData);
		}
	}
	
	//----------------------------------------------------------------------------------------------------

	

	//----------------------------------------------------------------------------------------------------

	

	//----------------------------------------------------------------------------------------------------
	
	feedback_sucess = recieveTransactionData(&data);
	if (feedback_sucess == 0) {
		PlaySound(TEXT("pass.wav"), NULL, SND_FILENAME | SND_SYNC);
		saveTransaction(&data, 0);
		filecpy();
	}
	else if (feedback_sucess == 2) {
		PlaySound(TEXT("fail.wav"), NULL, SND_FILENAME | SND_SYNC);
		saveTransaction(&data, 2);
	}
	else if (feedback_sucess == 1) {
		PlaySound(TEXT("fail.wav"), NULL, SND_FILENAME | SND_SYNC);
		saveTransaction(&data, 1);
	}
	//----------------------------------------------------------------------------------------------------
	
	return 0;
}