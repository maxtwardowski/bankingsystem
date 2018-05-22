#ifndef __DBTOOLS_H__
#define __DBTOOLS_H__

typedef struct BankAccount {
    char number[27],
         name[30],
         surname[30],
         address[30],
         id[12];
    float balance;
} BankAccount;

int createaccount();
int displayallinfo();
int finddata(char * keyword);
int showdata();
int maketransfer();
int makepayment();
int randomInt(int range_start, int range_end);
void addaccounttodb(BankAccount account);
void printaccountinfo(BankAccount * account);
int numberofaccounts();

#endif
