#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100


typedef struct Transaction {
    char type[20];
    float amount;
    struct Transaction *next;
} Transaction;

typedef struct Customer {
    int accNo;
    char name[50];
    char password[20];
    float balance;
    Transaction *history;  
    struct Customer *next;
} Customer;

Customer *head = NULL;


int queue[MAX];
int front = -1, rear = -1;

void clearScreen() {
#ifdef _WIN32
    system("cls");   
#endif
}



int isQueueEmpty() {
    return (front == -1 || front > rear);
}

void enqueue(int accNo) {
    if (rear == MAX - 1) {
        printf("Queue is full!\n");
        return;
    }
    if (front == -1) front = 0;
    queue[++rear] = accNo;
    printf("Customer %d added to the queue.\n", accNo);
}

int dequeue() {
    if (isQueueEmpty()) {
        printf("Queue is empty!\n");
        return -1;
    }
    int accNo = queue[front++];
    if (front > rear) front = rear = -1;
    printf("Dequeued Customer: %d\n", accNo);
    return accNo;
}

int isInQueue(int accNo) {
    for (int i = front; i <= rear && !isQueueEmpty(); i++) {
        if (queue[i] == accNo) return 1;
    }
    return 0;
}


void addTransaction(Customer *cust, char *type, float amount) {
    Transaction *newT = (Transaction *)malloc(sizeof(Transaction));
    strcpy(newT->type, type);
    newT->amount = amount;
    newT->next = cust->history;
    cust->history = newT;
}


Customer* searchCustomer(int accNo, char *password) {
    Customer *temp = head;
    while (temp != NULL) {
        if (temp->accNo == accNo)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

Customer* searchCustomerByAcc(int accNo) {
    Customer *temp = head;
    while (temp != NULL) {
        if (temp->accNo == accNo) return temp;
        temp = temp->next;
    }
    return NULL;
}

void addCustomer() {
    Customer *newC = (Customer *)malloc(sizeof(Customer));
    printf("Enter Account Number: ");
    scanf("%d", &newC->accNo);
    printf("Enter Name: ");
    scanf("%s", newC->name);
    printf("Enter Password: ");
    scanf("%s", newC->password);
    printf("Enter Initial Balance: ");
    scanf("%f", &newC->balance);
    newC->history = NULL;
    newC->next = head;
    head = newC;
    printf("Customer added successfully.\n");
}

void viewAllCustomers() {
    Customer *temp = head;
    while (temp != NULL) {
        printf("AccNo: %d, Name: %s, Balance: %.2f\n", temp->accNo, temp->name, temp->balance);
        temp = temp->next;
    }
}

void removeCustomer() {
    int acc;
    printf("Enter Account Number to remove: ");
    scanf("%d", &acc);

    Customer *temp = head;
    Customer *prev = NULL;

    while (temp != NULL) {
        if (temp->accNo == acc) {
            // Remove kora hoiche linked list theke
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }

            Transaction *t = temp->history;
            while (t != NULL) {
                Transaction *nextT = t->next;
                free(t);
                t = nextT;
            }

            free(temp);
            printf("Customer %d removed successfully.\n", acc);
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Customer not found.\n");
}

// ---------------------- Customer Menu ----------------------
void customerMenu(Customer *cust) {
    int choice;
    do {
        printf("\n--- Customer Menu ---\n");
        printf("1. View Balance\n2. Deposit\n3. Withdraw\n4. View Transactions\n0. Logout\n");
        printf("Enter option: ");
        scanf("%d", &choice);
        if (choice<0 || choice>4)
        {
           printf("invalid choice");
           while(getchar() != '\n'); // clear buffer korsi
            continue;
        }

        else{
        if (choice == 1) {
            printf("Balance: %.2f\n", cust->balance);
        } else if (choice == 2) {
            float amount;
            printf("Enter deposit amount: ");
            scanf("%f", &amount);
            cust->balance += amount;
            addTransaction(cust, "Deposit", amount);
            printf("Deposit successful.\n");
        } else if (choice == 3) {
            float amount;
            printf("Enter withdraw amount: ");
            scanf("%f", &amount);
            if (cust->balance >= amount) {
                cust->balance -= amount;
                addTransaction(cust, "Withdraw", amount);
                printf("Withdrawal successful.\n");
            } else {
                printf("Insufficient balance!\n");
            }
        } else if (choice == 4) {
            Transaction *t = cust->history;
            printf("Transaction History:\n");
            while (t != NULL) {
                printf("%s: %.2f\n", t->type, t->amount);
                t = t->next;
            }
        } else if (choice != 0) {
            printf("invalid choice\n");
        }
    }
    } while (choice != 0);
}

// ---------------------- Officer Menu ----------------------
void officerMenu() {
    int choice;
    do {
        printf("   \n--- Bank Officer Menu ---\n");
        printf("      1. Add Customer\n");
        printf("      2. Search Customer\n");
        printf("      3. Enqueue Service\n");
        printf("      4. Dequeue Service\n");
        printf("      5. View All Customers\n");
        printf("      6. Remove Customer\n"); 
        printf("      0. Logout\n");
        printf("      Enter option: ");
        scanf("%d", &choice);
        if (choice<0 || choice>6)
        {
           printf("invalid choice");
           while(getchar() != '\n'); // clear buffer korsi
            continue;
        }

        else{
        if (choice == 1) addCustomer();
        else if (choice == 2) {
            int acc; char pass[20];
            printf("     Enter Account No: ");
            scanf("%d", &acc);
            Customer *c = searchCustomer(acc, pass);
            clearScreen();
            if (c) printf("Customer found:\nName: %s, Balance: %.2f\n", c->name, c->balance);
            else printf("Customer not found.\n");
        }
        else if (choice == 3) {
            int acc; printf("Enter Account Number to enqueue: ");
            scanf("%d", &acc);
            if (searchCustomerByAcc(acc)) enqueue(acc);
            else printf("No such customer.\n");
        }
        else if (choice == 4) dequeue();
        else if (choice == 5) viewAllCustomers();
        else if (choice == 6) removeCustomer(); 

        else if (choice != 0) printf("invalid choice\n");
        }
    } while (choice != 0);
}

// -----main function-----
int main() {
    int choice;
    int r = 233, g = 150, b = 122;
    
    while (1) {
        printf("\033[38;2;%d;%d;%dm", r, g, b);
        //new design for project
        printf("\n");
        printf("            #########        \n");
        printf("           ###########       \n");
        printf("          #####   #####      \n");
        printf("         #####     #####     \n");
        printf("        #####       #####    \n");
        printf("       ###################   \n");
        printf("         ##  ##  ##  ##      \n");
        printf("         ##  ##  ##  ##      \n");
        printf("         ##  ##  ##  ##      \n");
        printf("         ##  ##  ##  ##      \n");
        printf("         ##  ##  ##  ##      \n");
        printf("      ####################   ");
        printf("\033[0m\n");
        printf("------ WELCOME TO AL-BARAKA ---------\n");
        printf("---Trust In Us, That's all We Care About---\n\n");
        
        
        printf("    1. Bank Officer Login\n");
        printf("    2. Customer Service\n");       
        printf("    0. Exit\n");
        printf("    Enter option: ");
        scanf("%d",&choice);
        if (choice<0 || choice>2)
        {
           printf("invalid choice");
           while(getchar() != '\n'); // clear buffer korsi jate garbage value nei
            continue;
        }

        else{
          if (choice == 1) {
            char User[50],pass[50];
            char id[] = "admin100";
            char key[] = "pass10";
            printf("Enter Username: ");
            scanf("%s",User);
            printf("Enter Password: ");
            scanf("%s",pass);
            if(strcmp(User,id)== 0 && strcmp(pass,key)==0){
            clearScreen();
            officerMenu();
            
            }
            else {printf("INVALID USERNAME OR PASS.\n");
                  printf("Plzz try again....");}
        } else if (choice == 2) {
            int acc; char pass[20];
            printf("  Enter Account Number: ");
            scanf("%d", &acc);
            printf("  Enter Password: ");
            scanf("%s", pass);
            Customer *cust = searchCustomer(acc, pass);
            if (cust) {
                if (isInQueue(acc)) {
                    customerMenu(cust);
                    clearScreen();
                } else {
                    printf("You are not in the service queue. Please wait.\n");
                }
            } else {
                printf("  Invalid login.\n");
                printf("  Plzz try again....\n");
            }
        } else if (choice == 0) {
            printf("Exiting...\n");
            break;
        } else {
            printf("invalid choice\n");
        }
    }
    }
    return 0;
}
