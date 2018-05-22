#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "stringtools.h"
#include "dbtools.h"

int createaccount() {
    BankAccount newaccount;
    cleanbuffer();
    printf("========================================\n");
    printf("CREATE NEW ACCOUNT:\n");
    printf("Name: ");
    safestringinput(newaccount.name, sizeof(newaccount.name));
    if (!isletter(newaccount.name)) {
        printf("Wrong name format\nAborting...\n");
        return 0;
    }
    remspaces(newaccount.name);
    printf("Surname: ");
    safestringinput(newaccount.surname, sizeof(newaccount.surname));
    if (!isletter(newaccount.surname)) {
        printf("Wrong surname format\nAborting...\n");
        return 0;
    }
    remspaces(newaccount.surname);
    printf("Address: ");
    safestringinput(newaccount.address, sizeof(newaccount.address));
    printf("Identification number (PESEL): ");
    safestringinput(newaccount.id, sizeof(newaccount.id));
    if (!isnumerical(newaccount.id) || strlen(newaccount.id) != 11) {
        printf("Wrong ID number format\nAborting...\n");
        return 0;
    }
    printf("Initial account balance: ");
    char balance_rawinput[20];
    safestringinput(balance_rawinput, sizeof(balance_rawinput));
    if (!isfloat(balance_rawinput)) {
        printf("Wrong account balance format\nAborting...\n");
        return 0;
    }

    newaccount.balance = atof(balance_rawinput);
    srand(time(NULL));

    for (int i = 0; i < 26; i++)
        newaccount.number[i] = randomInt(0, 9) + '0';
    newaccount.number[26] = '\0';

    printf("\nAre you sure you want to create a new account consisting of the following data?\n\n%s %s, %s\n%s\n\n[Y/N]: ", newaccount.name, newaccount.surname, newaccount.address, newaccount.id);
    char decision[10];
    safestringinput(decision, sizeof(decision));
    if (strcmp(decision, "Y") && strcmp(decision, "y")) {
        printf("Account adding cancelled.\n");
        return 0;
    }
    addaccounttodb(newaccount);
    return 1;
}

int displayallinfo() {
    //returns 0 if the database is empty, otherwise 1
    FILE * database;
    BankAccount account;

    database = fopen("database.dat", "r");
    if (database == NULL) {
        printf("The database is empty.\n");
        return 0;
    }
    printf("========================================\n");
    printf("<== DISPLAYING %d ACCOUNT(S) PRESENT IN THE DATABASE: ==>\n", numberofaccounts());
    while (fread(&account, sizeof(BankAccount), 1, database))
        printaccountinfo(&account);
    printf("========================================\n");
    fclose(database);
    return 1;
}

int showdata() {
    if (numberofaccounts() == 0) {
        printf("No accounts in database!\n");
        return -1;
    }
    char keyword[20];
    cleanbuffer();
    printf("Input a keyword to start searching: ");
    safestringinput(keyword, sizeof(keyword));
    finddata(keyword);
    return 1;
}

int finddata(char * keyword) {
    if (numberofaccounts() == 0) {
        printf("No accounts in database!\n");
        return -1;
    }
    FILE * database;
    BankAccount account;

    database = fopen("database.dat", "r");
    if (database == NULL) {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    int position = 0;
    bool resultfound = false;
    printf("\n===========SEARCH RESULTS===========\n");
    while (fread(&account, sizeof(BankAccount), 1, database)) {
        if (!resultfound)
            position++;
        if (strcasestr(account.name, keyword) != NULL ||
            strcasestr(account.surname, keyword) != NULL ||
            strcasestr(account.address, keyword) != NULL ||
            strcasestr(account.id, keyword) != NULL ||
            strcasestr(account.number, keyword) != NULL ||
            atof(keyword) == account.balance) {
                printaccountinfo(&account);
                resultfound = true;
            }
    }
    printf("====================================\n");

	if(!resultfound) {
		printf("\nCouldn't find a match for the keyword given\n");
        return -1;
	}
    return position;
}

int maketransfer() {
    //returns 1 if transfer succesful, otherwise 0
    if (numberofaccounts() < 2) {
        printf("There should be at least two accounts to make a transfer.\n");
        return 0;
    }
    char senderaccno[27], receiveraccno[27], amount_rawinput[20];
    float amount;

    printf("========================================\n");
    printf("<== NEW TRANSFER: ==>\n");
    printf("@SENDER DATA:\n");
    printf("Account number: ");
    cleanbuffer();
    safestringinput(senderaccno, sizeof(senderaccno));
    if (!isnumerical(senderaccno) || strlen(senderaccno) != 26) {
        printf("Wrong account number format!\nTransfer cancelled\n");
        return 0;
    }
    printf("Amount to be transferred: ");
    safestringinput(amount_rawinput, sizeof(amount_rawinput));
    if (!isfloat(amount_rawinput)) {
        printf("Wrong amount input!\nAborting...\n");
        return 0;
    }
    amount = atof(amount_rawinput);
    cleanbuffer();
    printf("\n@RECEIVER DATA:\n");
    printf("Account number: ");
    safestringinput(receiveraccno, sizeof(receiveraccno));
    if (!isnumerical(receiveraccno) || strlen(receiveraccno) != 26) {
        printf("Wrong account number format!\nTransfer cancelled\n");
        return 0;
    }

    long sender_offset = (finddata(senderaccno) - 1) * sizeof(BankAccount),
         receiver_offset = (finddata(receiveraccno) - 1) * sizeof(BankAccount);

    FILE * database = fopen ("database.dat", "r+");
    BankAccount indbsender, indbreceiver;

    int fseekcheck = fseek(database, sender_offset, SEEK_SET);
    if (!fseekcheck) {
        fread(&indbsender, sizeof(BankAccount), 1, database);
        fseekcheck = fseek(database, receiver_offset, SEEK_SET);
        if (!fseekcheck) {
            fread(&indbreceiver, sizeof(BankAccount), 1, database);
            printf("Successfully matched account numbers!\n");
        }
    } else {
        printf("Account numbers mismatch! Transfer cancelled.");
        fclose(database);
        return 0;
    }

    printf("WARNING: Are you sure you want to transfer %.2f from %s %s to %s %s? [Y/N] ",
            amount, indbsender.name, indbsender.surname,
            indbreceiver.name, indbreceiver.surname);

    char decision[10];
    safestringinput(decision, sizeof(decision));
    if (strcmp(decision, "Y") && strcmp(decision, "y")) {
        printf("Trasfer cancelled.\n");
        fclose(database);
        return 0;
    }

    if (indbsender.balance >= amount) {
        indbsender.balance -= amount;
        indbreceiver.balance += amount;
        fseek(database, sender_offset, SEEK_SET);
        fwrite(&indbsender, sizeof(BankAccount), 1, database);
        fseek(database, receiver_offset, SEEK_SET);
        fwrite(&indbreceiver, sizeof(BankAccount), 1, database);
    } else {
        printf("Insufficient funds! Transfer cancelled\n");
        fclose(database);
        return 0;
    }

    fclose(database);
    printf("Transfer complete!\n");
    return 1;
}

int makepayment(int mode) {
    //for mode == 1 --> payment
    //for mode == 0 --> withdrawal
    if (numberofaccounts() == 0) {
        printf("No accounts in database!\n");
        return 0;
    }

    char accountnumber[27];
    float amount;
    printf("\n===========PAYMENT===========\n");
    printf("Account number: ");
    cleanbuffer();
    safestringinput(accountnumber, sizeof(accountnumber));
    if (strlen(accountnumber) != 26 || !isnumerical(accountnumber)) {
        printf("Wrong amount input!\nAborting...\n");
        return 0;
    }
    printf("Amount: ");
    if (scanf("%f", &amount) != 1) {
        printf("Wrong amount input!\nAborting...\n");
        return 0;
    }

    if (strlen(accountnumber) != 26) {
        printf("Invalid account number input!\nPayment cancelled.\n");
        return 0;
    }

    long offset = (finddata(accountnumber) - 1) * sizeof(BankAccount);

    FILE * database = fopen("database.dat", "r+");
    int fseekcheck = fseek(database, offset, SEEK_SET);
    if (fseekcheck) {
        printf("No account like this exists in the database.\n");
        fclose(database);
        return 0;
    }

    BankAccount account;
    fread(&account, sizeof(BankAccount), 1, database);
    if (mode) {
        account.balance += amount;
    } else {
        if (account.balance >= amount)
            account.balance -= amount;
        else {
            printf("No sufficient funds for this operation!\nWithdrawal cancelled.\n");
            fclose(database);
            return 0;
        }
    }
    if (mode)
        printf("WARNING: Are you sure you want to grant %s %s with %.2f? [Y/N] ",
                account.name, account.surname, amount);
    else
    printf("WARNING: Are you sure you want to perform withdrawal of %.2f on %s %s? [Y/N] ",
            amount, account.name, account.surname);

    char decision[10];
    cleanbuffer();
    safestringinput(decision, sizeof(decision));
    if (strcmp(decision, "Y") && strcmp(decision, "y")) {
        printf("Trasfer cancelled.\n");
        fclose(database);
        return 0;
    }
#include <ctype.h>
#include <time.h>
#include <string.h>

    fseek(database, offset, SEEK_SET);
    fwrite(&account, sizeof(BankAccount), 1, database);

    printf("Payment complete!\n");
    fclose(database);
    return 1;
}

int randomInt(int range_start, int range_end) {
    //Generated random integer from the [range_start; range_end] interval
    int random_int = rand() % (range_end - range_start + 1) + range_start; //+1 to avoid black
    return random_int;
}

void addaccounttodb(BankAccount account) {
    FILE * database;
    database = fopen ("database.dat", "a");
    if (database == NULL) {
        fprintf(stderr, "\nError occured while opening file\n");
        exit(1);
    }
    fwrite (&account, sizeof(BankAccount), 1, database);
    if (fwrite != 0)
        printf("Contents to the file written successfully!\n");
    else
        printf("Error writing file!\n");
    fclose(database);
}

void printaccountinfo(BankAccount * account) {
    printf("\n%s %s, %s\nID: %s\nAccount number: %s\nCurrent balance: %.2f\n\n",
    account -> name, account -> surname, account -> address,
    account -> id, account -> number, account -> balance);
}

int numberofaccounts() {
    FILE * database = fopen("database.dat", "r");
    if (database == NULL) {
        printf("The database is empty.\n");
        return 0;
    }
    BankAccount account;
    int numberofaccounts = 0;
    while (fread(&account, sizeof(BankAccount), 1, database))
        numberofaccounts++;
    return numberofaccounts;
}
