#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 100
#define MAX_FILE_NAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    int id;
    int balance;
} Student;

// Function prototypes
void add_student(FILE *file);
void deposit_money(FILE *file);
void withdraw_money(FILE *file);
void display_students(FILE *file);
void display_balance(FILE *file);

int main() {
    int choice;
    FILE *file;

    // Open file in read/write mode, create if not exists
    file = fopen("students.dat", "r+b");
    if (!file) {
        file = fopen("students.dat", "w+b");
        if (!file) {
            perror("Failed to open file");
            return 1;
        }
    }

    do {
        printf("\n--- Student Money Management ---\n");
        printf("1. Add Student\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Display Students\n");
        printf("5. Display Balance\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_student(file);
                break;
            case 2:
                deposit_money(file);
                break;
            case 3:
                withdraw_money(file);
                break;
            case 4:
                display_students(file);
                break;
            case 5:
                display_balance(file);
                break;
            case 6:
                fclose(file);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void add_student(FILE *file) {
    Student student;
    printf("Enter student ID: ");
    scanf("%d", &student.id);
    printf("Enter student name: ");
    getchar();  // Consume newline character left in the buffer
    fgets(student.name, MAX_NAME_LENGTH, stdin);
    student.name[strcspn(student.name, "\n")] = '\0';  // Remove newline character
    student.balance = 0;

    fseek(file, 0, SEEK_END);
    fwrite(&student, sizeof(Student), 1, file);
    printf("Student added successfully.\n");
}

void deposit_money(FILE *file) {
    int id, amount;
    Student student;
    printf("Enter student ID: ");
    scanf("%d", &id);
    printf("Enter amount to deposit: ");
    scanf("%d", &amount);

    fseek(file, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            student.balance += amount;
            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);
            printf("Deposit successful. New balance: %d\n", student.balance);
            return;
        }
    }

    printf("Student ID not found.\n");
}

void withdraw_money(FILE *file) {
    int id, amount;
    Student student;
    printf("Enter student ID: ");
    scanf("%d", &id);
    printf("Enter amount to withdraw: ");
    scanf("%d", &amount);

    fseek(file, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            if (student.balance >= amount) {
                student.balance -= amount;
                fseek(file, -sizeof(Student), SEEK_CUR);
                fwrite(&student, sizeof(Student), 1, file);
                printf("Withdrawal successful. New balance: %d\n", student.balance);
            } else {
                printf("Insufficient funds.\n");
            }
            return;
        }
    }

    printf("Student ID not found.\n");
}

void display_students(FILE *file) {
    Student student;
    fseek(file, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("ID: %d, Name: %s, Balance: %d\n", student.id, student.name, student.balance);
    }
}

void display_balance(FILE *file) {
    int id;
    Student student;
    printf("Enter student ID: ");
    scanf("%d", &id);

    fseek(file, 0, SEEK_SET);
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            printf("Current balance for ID %d: %d\n", id, student.balance);
            return;
        }
    }

    printf("Student ID not found.\n");
}
