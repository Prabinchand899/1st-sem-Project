#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILENAME "students.dat"

typedef struct {
    int id;
    char name[50];
    char faculty[30];
    float marks[5];      // 5 subjects
    float percentage;
    char division[15];
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

// Functions
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void calculateResult(Student *s);
void saveToFile();
void loadFromFile();
void menu();

int main() {
    loadFromFile();
    menu();
    return 0;
}

// main menu:
void menu() {
    int choice;
    while(1) {
        printf("\n==============================\n");
        printf("   STUDENT RECORD SYSTEM\n");
        printf("==============================\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("==============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: 
                saveToFile();
                printf("Thank you! Data saved successfully.\n");
                exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

//result calculations:
void calculateResult(Student *s) {
    float total = 0;
    for(int i = 0; i < 5; i++) {
        total += s->marks[i];
    }
    s->percentage = total / 5.0;

    if(s->percentage >= 80) strcpy(s->division, "Distinction");
    else if(s->percentage >= 65) strcpy(s->division, "First");
    else if(s->percentage >= 50) strcpy(s->division, "Second");
    else if(s->percentage >= 40) strcpy(s->division, "Third");
    else strcpy(s->division, "Fail");
}

// add student data:
void addStudent() {
    if(studentCount >= MAX_STUDENTS) {
        printf("Maximum limit reached!\n");
        return;
    }

    Student s;
    printf("\nEnter Student ID: ");
    scanf("%d", &s.id);
    getchar();  // consume newline

    printf("Enter Name: ");
    fgets(s.name, 50, stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter Faculty: ");
    fgets(s.faculty, 30, stdin);
    s.faculty[strcspn(s.faculty, "\n")] = 0;

    printf("Enter marks of 5 subjects:\n");
    for(int i = 0; i < 5; i++) {
        printf("Subject %d: ", i+1);
        scanf("%f", &s.marks[i]);
    }

    calculateResult(&s);

    students[studentCount++] = s;
    printf("Student added successfully!\n");
}

//display all data:
void displayAllStudents() {
    if(studentCount == 0) {
        printf("No records found!\n");
        return;
    }
    printf("\n%-5s %-25s %-20s %-10s %-12s\n", "ID", "Name", "Faculty", "Percentage", "Division");
    printf("-----------------------------------------------------------------\n");
    
    for(int i = 0; i < studentCount; i++) {
        printf("%-5d %-25s %-20s %-10.2f %-12s\n", 
               students[i].id, students[i].name, students[i].faculty, 
               students[i].percentage, students[i].division);
    }
}

//search student data:
void searchStudent() {
    int id, found = 0;
    char name[50];
    int choice;

    printf("Search by: 1. ID  2. Name\n");
    scanf("%d", &choice);

    if(choice == 1) {
        printf("Enter ID: ");
        scanf("%d", &id);
        for(int i = 0; i < studentCount; i++) {
            if(students[i].id == id) {
                printf("\nStudent Found!\n");
                printf("ID: %d\nName: %s\nFaculty: %s\nPercentage: %.2f\nDivision: %s\n",
                       students[i].id, students[i].name, students[i].faculty,
                       students[i].percentage, students[i].division);
                found = 1;
                break;
            }
        }
    } else {
        printf("Enter Name: ");
        getchar();
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0;
        for(int i = 0; i < studentCount; i++) {
            if(strcmp(students[i].name, name) == 0) {
                printf("\nStudent Found!\n");
                printf("ID: %d\nName: %s\nFaculty: %s\nPercentage: %.2f\nDivision: %s\n",
                       students[i].id, students[i].name, students[i].faculty,
                       students[i].percentage, students[i].division);
                found = 1;
            }
        }
    }
    if(!found) printf("Student not found!\n");
}

//update student data:
void updateStudent() {
    int id, found = 0;
    printf("Enter Student ID to update: ");
    scanf("%d", &id);

    for(int i = 0; i < studentCount; i++) {
        if(students[i].id == id) {
            printf("Enter new Name: ");
            getchar();
            fgets(students[i].name, 50, stdin);
            students[i].name[strcspn(students[i].name, "\n")] = 0;

            printf("Enter new Faculty: ");
            fgets(students[i].faculty, 30, stdin);
            students[i].faculty[strcspn(students[i].faculty, "\n")] = 0;

            printf("Enter new marks:\n");
            for(int j = 0; j < 5; j++) {
                printf("Subject %d: ", j+1);
                scanf("%f", &students[i].marks[j]);
            }
            calculateResult(&students[i]);
            printf("Student updated successfully!\n");
            found = 1;
            break;
        }
    }
    if(!found) printf("Student not found!\n");
}

//delete student data:
void deleteStudent() {
    int id, found = 0;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    for(int i = 0; i < studentCount; i++) {
        if(students[i].id == id) {
            for(int j = i; j < studentCount - 1; j++) {
                students[j] = students[j+1];
            }
            studentCount--;
            printf("Student deleted successfully!\n");
            found = 1;
            break;
        }
    }
    if(!found) printf("Student not found!\n");
}

//save the data to file:
void saveToFile() {
    FILE *fp = fopen(FILENAME, "wb");
    if(fp == NULL) {
        printf("Error saving file!\n");
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), studentCount, fp);
    fclose(fp);
}

//read data from file:
void loadFromFile() {
    FILE *fp = fopen(FILENAME, "rb");
    if(fp == NULL) return;

    fread(&studentCount, sizeof(int), 1, fp);
    fread(students, sizeof(Student), studentCount, fp);
    fclose(fp);
}