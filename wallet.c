#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "wallet.txt"

void updateDashboard() {
    FILE *file = fopen(DATA_FILE, "r");//opening file in reading mode
    float balance = 0.0, amount = 0.0;
    char type[10] = "";
    char desc[101] = "No history";
    char lastType[10] = "";
    char lastDesc[101] = "No transactions yet";//description of last transaction
    float lastAmount = 0.0;

    if (file != NULL) {
        while (fscanf(file, "%s %f %[^\n]", type, &amount, desc) == 3) {
            if (strcmp(type, "Earning") == 0) balance += amount;
            else balance -= amount;
            
            strcpy(lastType, type);
            strcpy(lastDesc, desc);
            lastAmount = amount;
        }
        fclose(file);
    }

    // \033[u => resets the cursor back to the saved position before retyping
    // \033[K => clears the entire line from left to right to remove old characters
    printf("\033[u"); 
    
    if (strlen(lastType) == 0) {
        printf("\n[BALANCE: Rs.%.2f] | Last: %s\033[K\n", balance, lastDesc);
    } else {
        printf("\n[BALANCE: Rs.%.2f] | Last: %s Rs.%.2f (%s)\033[K\n", 
                balance, strcmp(lastType, "Earning") == 0 ? "[+]" : "[-]", lastAmount, lastDesc);
    }

    //printing lines with \033[K clear safeguards
    printf("<<<<<<<<>>>>>>>>\033[K\n");
    printf("Menu\033[K\n");
    printf("1. Add Earning\033[K\n");
    printf("2. Add Expense\033[K\n");
    printf("3. Reset Wallet\033[K\n");
    printf("4. Exit App\033[K\n");
    printf("Enter Choice -> \033[K");
    fflush(stdout);
}

int main() {
    int choice;
    float amount;
    char desc[101];

    // \033[s saves the baseline cursor position right there before the loop begins
    printf("\n\033[s"); 

    while (1) {
        updateDashboard();//re-prints the stats

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); 

        if (choice == 1 || choice == 2) {
            //request inputs cleanly on lines below the menu
            printf("Amount: \033[K"); 
            scanf("%f", &amount); 
            while (getchar() != '\n');

            printf("Where: \033[K"); 
            fgets(desc, sizeof(desc), stdin); 
            desc[strcspn(desc, "\n")] = 0; 

            FILE *file = fopen(DATA_FILE, "a");//opening file in appened mode
            if (file != NULL) {
                fprintf(file, "%s %.2f %s\n", (choice == 1) ? "Earning" : "Expense", amount, desc);
                fclose(file);
            }
        } else if (choice == 3) {
            FILE *file = fopen(DATA_FILE, "w"); //opening file in write mode
            if (file != NULL) fclose(file);
        } else if (choice == 4) {
            //clear out the menu block area before exiting to leave the terminal tidy
            printf("\033[u\033[K[THANK YOU (:>).... App Exited Cleanly....]\n\033[K\033[K\033[K\033[K\033[K");
            break;
        }
    }
    return 0;
}