#include <stdio.h>
#include <stdlib.h>

struct Account {
    int accountNo;
    char name[50];
    double balance;
};

// Check Account Number
int checkAccount(int accNo) {
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;

    if (fp == NULL)
        return 0;

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accountNo == accNo) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// Create Account
void createAccount() {
    struct Account a;
    FILE *fp;

    printf("\nEnter Account Number: ");
    scanf("%d", &a.accountNo);

    if (checkAccount(a.accountNo)) {
        printf("Account Already Exists!\n");
        return;
    }

    printf("Enter Account Holder Name: ");
    scanf(" %49[^\n]", a.name);

    printf("Enter Initial Balance: ");
    scanf("%lf", &a.balance);

    if (a.balance < 0) {
        printf("Invalid Balance!\n");
        return;
    }

    fp = fopen("bank.dat", "ab");

    if (fp == NULL) {
        printf("File Opening Error!\n");
        return;
    }

    if (fwrite(&a, sizeof(a), 1, fp) == 1)
        printf("\nAccount Created Successfully!\n");
    else
        printf("\nError Creating Account!\n");

    fclose(fp);
}

// Deposit Money
void depositMoney() {
    struct Account a;
    FILE *fp = fopen("bank.dat", "rb+");
    int accNo, found = 0;
    double amount;

    if (fp == NULL) {
        printf("No Accounts Found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accountNo == accNo) {
            printf("Enter Deposit Amount: ");
            scanf("%lf", &amount);

            if (amount <= 0) {
                printf("Invalid Amount!\n");
                fclose(fp);
                return;
            }

            if (a.balance > 1e12 - amount) {
                printf("Balance Limit Exceeded!\n");
                fclose(fp);
                return;
            }

            a.balance += amount;

            fseek(fp, -(long)sizeof(a), SEEK_CUR);

            if (fwrite(&a, sizeof(a), 1, fp) == 1) {
                printf("\nMoney Deposited Successfully!\n");
                printf("New Balance: %.2lf\n", a.balance);
            } else {
                printf("Transaction Failed!\n");
            }

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!\n");

    fclose(fp);
}

// Withdraw Money
void withdrawMoney() {
    struct Account a;
    FILE *fp = fopen("bank.dat", "rb+");
    int accNo, found = 0;
    double amount;

    if (fp == NULL) {
        printf("No Accounts Found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accountNo == accNo) {
            printf("Enter Withdrawal Amount: ");
            scanf("%lf", &amount);

            if (amount <= 0) {
                printf("Invalid Amount!\n");
                fclose(fp);
                return;
            }

            if (amount > a.balance) {
                printf("\nInsufficient Balance!\n");
                fclose(fp);
                return;
            }

            a.balance -= amount;

            fseek(fp, -(long)sizeof(a), SEEK_CUR);

            if (fwrite(&a, sizeof(a), 1, fp) == 1) {
                printf("\nWithdrawal Successful!\n");
                printf("Remaining Balance: %.2lf\n",
                       a.balance);
            } else {
                printf("Transaction Failed!\n");
            }

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!\n");

    fclose(fp);
}

// Balance Enquiry
void balanceEnquiry() {
    struct Account a;
    FILE *fp = fopen("bank.dat", "rb");
    int accNo, found = 0;

    if (fp == NULL) {
        printf("No Accounts Found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accountNo == accNo) {
            printf("\n========== ACCOUNT DETAILS ==========");
            printf("\nAccount Number: %d", a.accountNo);
            printf("\nAccount Holder: %s", a.name);
            printf("\nAvailable Balance: %.2lf\n", a.balance);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!\n");

    fclose(fp);
}

// Display All Accounts
void displayAccounts() {
    struct Account a;
    FILE *fp = fopen("bank.dat", "rb");
    int count = 0;

    if (fp == NULL) {
        printf("No Accounts Found!\n");
        return;
    }

    printf("\n========== ALL BANK ACCOUNTS ==========\n");

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        printf("\nAccount Number: %d", a.accountNo);
        printf("\nAccount Holder: %s", a.name);
        printf("\nBalance: %.2lf\n", a.balance);
        count++;
    }

    if (count == 0)
        printf("No Accounts Available!\n");

    fclose(fp);
}

// Main Function
int main() {
    int choice;

    do {
        printf("\n\n================================");
        printf("\n          BANKING SYSTEM");
        printf("\n================================");
        printf("\n1. Create Account");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Balance Enquiry");
        printf("\n5. Display All Accounts");
        printf("\n6. Exit");

        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
                depositMoney();
                break;

            case 3:
                withdrawMoney();
                break;

            case 4:
                balanceEnquiry();
                break;

            case 5:
                displayAccounts();
                break;

            case 6:
                printf("\nThank You For Using Banking System!\n");
                break;

            default:
                printf("\nInvalid Choice! Try Again.\n");
        }

    } while (choice != 6);

    return 0;
}
