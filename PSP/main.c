#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

struct Expense {
    int id;
    char description[50];
    float amount;
    char category[20];
    char date[11];
};

// Function declarations
void addExpense();
void viewAllExpenses();
void viewByCategory();
void editExpense();
void deleteExpense();
void monthlySummary();
void setBudget();
void checkBudgetStatus();
void searchExpenses();
void toLowerCase(char *str);
int main() {
    int choice;
    
    do {
        // Clear screen (optional)
        system("clear"); // Use system("cls") for Windows
        
        printf("===== EXPENSE TRACKER =====\n");
        printf("1. Add New Expense\n");
        printf("2. View All Expenses\n");
        printf("3. View by Category\n");
        printf("4. Edit Expense\n");
        printf("5. Delete Expense\n");
        printf("6. Monthly Summary\n");
        printf("7. Set Budget\n");
        printf("8. Check Budget Status\n");
        printf("9. Search Expenses\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addExpense();
                break;
            case 2:
                viewAllExpenses();
                break;
            case 3:
                viewByCategory();
                break;
            case 4:
                editExpense();
                break;
            case 5:
                deleteExpense();
                break;
            case 6:
                monthlySummary();
                break;
            case 7:
                setBudget();
                break;
            case 8:
                checkBudgetStatus();
                break;
            case 9:
                searchExpenses();
                break;
            case 0:
                printf("Thank you for using Expense Tracker!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
        if(choice != 0) {
            printf("\nPress Enter to continue...");
            getchar(); // Clear newline from scanf
            getchar(); // Wait for user input
        }
        
    } while(choice != 0);
    
    return 0;
}

// Implement each function one by one
void addExpense() {

    int getNextId(){
    FILE *file;
    struct Expense exp;
    int maxId = 0;
    
    file = fopen("expenses.dat", "rb");
    if(file == NULL) {
        return 1; // First expense
    }
    
    while(fread(&exp, sizeof(struct Expense), 1, file)) {
        if(exp.id > maxId) {
            maxId = exp.id;
        }
    }
    
    fclose(file);
    return maxId + 1;
    };

// Then in addExpense(), replace exp.id = 1; with:

    FILE *file;
    struct Expense exp;

    exp.id = getNextId();
    
    printf("\n--- Add New Expense ---\n");
    
    printf("Enter description: ");
    getchar(); // Clear buffer
    fgets(exp.description, 50, stdin);
    exp.description[strcspn(exp.description, "\n")] = 0; // Remove newline
    
    printf("Enter amount: ");
    scanf("%f", &exp.amount);
    
    printf("Enter category (Food/Transport/Shopping/Bills/Entertainment/Education/Healthcare/Others): ");
    
    getchar();
    fgets(exp.category, 20, stdin);
    exp.category[strcspn(exp.category, "\n")] = 0;
    toLowerCase(exp.category);

    
    printf("Enter date (DD-MM-YYYY): ");
    fgets(exp.date, 11, stdin);
    exp.date[strcspn(exp.date, "\n")] = 0;
    
    // Save to file
    file = fopen("expenses.dat", "ab"); // Append binary mode
    if(file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    fwrite(&exp, sizeof(struct Expense), 1, file);
    fclose(file);
    
    printf("\nExpense added successfully!\n");
    
}

void viewAllExpenses() {
    FILE *file;
    struct Expense exp;
    int count = 0;
    float total = 0;
    
    printf("\n--- All Expenses ---\n");
    
    file = fopen("expenses.dat", "rb"); // Read binary mode
    if(file == NULL) {
        printf("No expenses found or error opening file!\n");
        return;
    }
    
    printf("\n%-5s %-20s %-15s %-15s %-12s\n", "ID", "Description", "Category", "Amount", "Date");
    printf("--------------------------------------------------------------------\n");
    
    // Read all expenses from file
    while(fread(&exp, sizeof(struct Expense), 1, file)) {
        printf("%-5d %-20s %-15s ₹%-14.2f %-12s\n", 
               exp.id, exp.description, exp.category, exp.amount, exp.date);
        total += exp.amount;
        count++;
    }
    
    fclose(file);
    
    if(count == 0) {
        printf("No expenses recorded yet.\n");
    } else {
        printf("--------------------------------------------------------------------\n");
        printf("Total Expenses: %d\n", count);
        printf("Total Amount: ₹%.2f\n", total);
    }
}

void viewByCategory() {


    FILE *file;
    struct Expense exp;
    int count = 0;
    float total = 0;
    char cat[20];

    printf("\n--- View by Category ---\n");
    printf("Enter category (Food/Transport/Shopping/Bills/Entertainment/Education/Healthcare/Others): ");
    getchar();
    fgets(cat, 20, stdin);
    cat[strcspn(cat, "\n")] = 0;
    toLowerCase(cat);

    
    file = fopen("expenses.dat", "rb"); // Read binary mode
    if(file == NULL) {
        printf("No expenses found or error opening file!\n");
        return;
    }

    printf("\n%-5s %-20s %-15s %-15s %-12s\n", "ID", "Description", "Category", "Amount", "Date");
    printf("--------------------------------------------------------------------\n");
        // Read category expenses from file
    
    while(fread(&exp, sizeof(struct Expense), 1, file)) {

        if(strcmp( cat, exp.category) == 0){
          printf("%-5d %-20s %-15s ₹%-14.2f %-12s\n", 
                 exp.id, exp.description, exp.category, exp.amount, exp.date);
          total += exp.amount;
          count++;
        }
      }

    
    fclose(file);
    
    if(count == 0) {
        printf("No expenses recorded yet.\n");
    } else {
        printf("--------------------------------------------------------------------\n");
        printf("Total Expenses: %d\n", count);
        printf("Total Amount: ₹%.2f\n", total);
    }
    
}

void editExpense() {
    FILE *file, *tempFile;
    struct Expense exp;
    int editId, found = 0;
    
    printf("\n--- Edit Expense ---\n");
    printf("Enter expense ID to edit: ");
    scanf("%d", &editId);
    
    file = fopen("expenses.dat", "rb");
    if(file == NULL) {
        printf("No expenses found!\n");
        return;
    }
    
    tempFile = fopen("temp.dat", "wb");
    if(tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }
    
    while(fread(&exp, sizeof(struct Expense), 1, file)) {
        if(exp.id == editId) {
            found = 1;
            printf("\nCurrent Expense Details:\n");
            printf("Description: %s\n", exp.description);
            printf("Amount: ₹%.2f\n", exp.amount);
            printf("Category: %s\n", exp.category);
            printf("Date: %s\n\n", exp.date);
            
            printf("Enter new description: ");
            getchar();
            fgets(exp.description, 50, stdin);
            exp.description[strcspn(exp.description, "\n")] = 0;
            
            printf("Enter new amount: ");
            scanf("%f", &exp.amount);
            
            printf("Enter new category: ");
            getchar();
            fgets(exp.category, 20, stdin);
            exp.category[strcspn(exp.category, "\n")] = 0;
            
            printf("Enter new date (DD-MM-YYYY): ");
            fgets(exp.date, 11, stdin);
            exp.date[strcspn(exp.date, "\n")] = 0;
            
            printf("\nExpense updated successfully!\n");
        }
        fwrite(&exp, sizeof(struct Expense), 1, tempFile);
    }
    
    fclose(file);
    fclose(tempFile);
    
    remove("expenses.dat");
    rename("temp.dat", "expenses.dat");
    
    if(!found) {
        printf("Expense ID %d not found!\n", editId);
    }


}

void deleteExpense() {
   FILE *file, *tempFile;
    struct Expense exp;
    int deleteId, found = 0;
    
    printf("\n--- Delete Expense ---\n");
    printf("Enter expense ID to delete: ");
    scanf("%d", &deleteId);
    
    file = fopen("expenses.dat", "rb");
    if(file == NULL) {
        printf("No expenses found!\n");
        return;
    }
    
    tempFile = fopen("temp.dat", "wb");
    if(tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }
    
    // Copy all expenses except the one to delete
    while(fread(&exp, sizeof(struct Expense), 1, file)) {
        if(exp.id != deleteId) {
            fwrite(&exp, sizeof(struct Expense), 1, tempFile);
        } else {
            found = 1;
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    // Replace original file with temp file
    remove("expenses.dat");
    rename("temp.dat", "expenses.dat");
    
    if(found) {
        printf("Expense deleted successfully!\n");
    } else {
        printf("Expense ID %d not found!\n", deleteId);
    }

}

void monthlySummary() {
    printf("\n--- Monthly Summary ---\n");
    // Your code here
}

void setBudget() {
    printf("\n--- Set Budget ---\n");
    // Your code here
}

void checkBudgetStatus() {
    printf("\n--- Budget Status ---\n");
    // Your code here
}

void searchExpenses() {
    printf("\n--- Search Expenses ---\n");
    // Your code here
}
void toLowerCase(char *str) {
    for(int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}
