#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stringtools.h"
#include "dbtools.h"

void showmenu();
void printoptions();

int main(int argc, char const *argv[]) {
    showmenu();
    return 0;
}

void showmenu() {
    int operationID;
    enum operationOptions {
        CREATEACCOUNT = 1,
        DISPLAYALLINFO = 2,
        FINDDATA = 3,
        MAKETRANSFER = 4,
        MAKEPAYMENT = 5,
        MAKEWITHDRAWAL = 6
    };

    while (true) {
        printoptions();
        while (scanf("%d", &operationID) != 1) {
            printf("Wrong input detected! Try again.\n");
            printoptions();
            cleanbuffer();
        }

        switch (operationID) {
            case CREATEACCOUNT:
                createaccount();
                break;
            case DISPLAYALLINFO:
                displayallinfo();
                break;
            case FINDDATA:
                showdata();
                break;
            case MAKETRANSFER:
                maketransfer();
                break;
            case MAKEPAYMENT:
                makepayment(1);
                break;
            case MAKEWITHDRAWAL:
                makepayment(0);
                break;
            default:
                printf("Oops... Try again!\n");
                break;
        }
    }
}

void printoptions() {
    printf("\n===== BANKING SYSTEM =====\n\
    \r1. Create account \n\
    \r2. Display database contents \n\
    \r3. Search for accounts \n\
    \r4. Make a transfer \n\
    \r5. Make a payment \n\
    \r6. Make a withdrawal\n\n\
    \rPerform action: ");
}
