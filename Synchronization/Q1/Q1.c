#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>

#define NUM_CUSTOMERS 5 // number of customers
#define NUM_TRANSACTION 6 // number of transactions/threads
#define INIT_STOCK 100 // the initial value for each person stock

// type transaction. you should pass an object of this type to the function arguments while creating thread
typedef struct
{
    double amount;
    int customer_id;
} transaction_t;

double safeBox = 1000.0; // shared variable for all customers

///////// Start your code here ///////////

/*1. define an array to store the stock of all customers
  2. define the necessary mutexes for each customer account and safe box*/
double g_customers_stock[NUM_CUSTOMERS];
pthread_mutex_t customerAccountMutex[NUM_CUSTOMERS];
pthread_mutex_t safeBoxMutex;



void* withdraw(void* arg){
    /*be aware to print the result in each situation.
      for example in one case that the stock isn't adequate and customer took money from safe box, you can report like the following:
      Customer <customerID> withdrew <transaction-amount> and <the amount taken from safe box> from safe-box. New balance: <new amount of customer stock>\tsafe-box value: <new amount of safe box>\n
      don't forget to report the new balance and new safe box value*/
    transaction_t *currentTransaction = (struct transaction_t*)arg;
    double amount = currentTransaction->amount;
    int customerID = currentTransaction -> customer_id;
    double barrow = 0;

    pthread_mutex_lock(&customerAccountMutex[customerID]);
    pthread_mutex_lock(&safeBoxMutex);

    if(amount <= g_customers_stock[customerID])
    {
        g_customers_stock[customerID] -= amount;
        printf("Withdraw transaction was successful.\n Customer %d withdrew %.2f and %.2f from safe-box.\n New balance: %.2f\n safe-box value: %.2f\n\n", customerID, amount, barrow, g_customers_stock[customerID]);
    }
    else
    {
        barrow = amount - g_customers_stock[customerID];
        if(barrow <= safeBox)
        {               
            safeBox -= barrow;
            g_customers_stock[customerID] -= amount;
            printf("Withdraw transaction was successful.\n Customer %d withdrew %.2f and %.2f from safe-box.\n New balance: %.2f\n safe-box value: %.2f\n\n", customerID, amount, barrow, g_customers_stock[customerID]);
        }
        else
        {
            printf("Withdraw transaction failed.\n Not enough balance in customer %d account.\n Not enough balanace in safe-box. safe-box value: %.2f\n\n", customerID, safeBox);
        }
    }

    pthread_mutex_unlock(&safeBoxMutex);
    pthread_mutex_unlock(&customerAccountMutex[customerID]);    

    pthread_exit(NULL);
}

void* deposit(void* arg){
    /*be aware to print the result in the following form:
      Customer <customerID> stock was charged by <transaction-amount>. New balance: <new amount of customer stock>\tsafe-box value: <new amount of safe box>\n
      don't forget to report the new balance and new safe box value*/
    transaction_t *currentTransaction = (struct transaction_t*)arg;
    double amount = currentTransaction->amount;
    int customerID = currentTransaction -> customer_id;
    double dept;
    
    pthread_mutex_lock(&customerAccountMutex[customerID]);
    pthread_mutex_lock(&safeBoxMutex);

    if(g_customers_stock[customerID] < 0)
    {
        dept = -g_customers_stock[customerID];
        g_customers_stock[customerID] += amount;
        
        if(amount >= dept)
        {
            safeBox += dept;  
            printf("Deposit transaction was successful.\n dept is payed.\nCurrent balance: %.2f\n deposit amount: %.2f", g_customers_stock[customerID], amount);
        }
        else
        {
            safeBox += amount;
            printf("Deposit transaction was successful.\n Part of customer %d dept is payed.\n remaining dept: %.2f\n deposit amount: %.2f\n\n", customerID, -g_customers_stock[customerID], amount);
        }
    }
    else
    {
        g_customers_stock[customerID] += amount;
        printf("Deposit transaction was successful.\n deposit amount: %.2f\n Current balance: %.2f\n\n");
    }

    pthread_mutex_unlock(&safeBoxMutex);
    pthread_mutex_unlock(&customerAccountMutex[customerID]);

    pthread_exit(NULL);
}

int generateRandomNumber(int min, int max) {
    if (min > max) {        
        int temp = min;
        min = max;
        max = temp;
    }
        
    int randomNumber = rand() % (max - min + 1) + min;

    return randomNumber;
}

int main(){    
    srand(time(NULL));
    /* for each customer, value the stock with INIT_STOCK
    init mutex for each account and safe box */
    pthread_t threads[NUM_TRANSACTION];
    for(int i = 0; i < NUM_CUSTOMERS; i++)
    {
        g_customers_stock[i] = INIT_STOCK;
    }    
    /* create thread for each transaction
    for each thread:
    if rand() % 2 == 0
        create thread for withdraw
    otherwise
        create thread for deposit*/
    for(int i = 0; i < NUM_TRANSACTION; i++)
    {
        int randomNumber = generateRandomNumber(1, INT_MAX);
        int randomCustomer = generateRandomNumber(1, NUM_CUSTOMERS);
        int randomAmount = generateRandomNumber(1, 500);

        
        transaction_t* inputArguements = (transaction_t*)malloc(sizeof(transaction_t));
        inputArguements -> amount = randomAmount;
        inputArguements -> customer_id = randomCustomer - 1; 

        for (size_t i = 0; i < NUM_CUSTOMERS; i++)
        {
            pthread_mutex_init(&customerAccountMutex[i], NULL);
        }
        pthread_mutex_init(&safeBox, NULL); 
        

        if( randomNumber % 2 == 0)
        {
            pthread_create(&threads[i], NULL, withdraw, (void*)inputArguements);
        }
        else
        {
            pthread_create(&threads[i], NULL, deposit, (void*)inputArguements);
        }
    }

    for (size_t i = 0; i < NUM_TRANSACTION; i++)
    {
        pthread_join(threads[i], NULL);
    }
        

    for (size_t i = 0; i < NUM_CUSTOMERS; i++)
    {
        pthread_mutex_destroy(&customerAccountMutex[i]);
    }
    pthread_mutex_destroy(&safeBoxMutex); 

    ///////// End of your code ///////////
    return 0;
}