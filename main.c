#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct Donor
{
    int id;
    char name[50];
    char bloodType[4];
    char Number[15];
    char gender;
    int age;
};

// GLOBAL BLOOD STOCK
int stock[8] = {0};
char *bloodTypes[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};

// FUNCTION PROTOTYPES
int adminLogin();
void printMenu();
void add_donor(struct Donor donor[], int *count);
void search_donor(struct Donor donor[], int count);
void view_donor(struct Donor donor[], int count);
void delete_donor(struct Donor donor[], int *count);
void update_donor(struct Donor donor[], int count);
void viewBloodStock();
void SavetoFile(struct Donor donor[], int count);
int LoadToFile(struct Donor donor[]);
void searchByName(struct Donor donor[], int count);
void normalizeBloodType(char blood[]);
void updateStockFromDonors(struct Donor donor[], int count);
void searchByBloodGroup(struct Donor donor[], int count);

// HELPER FUNCTION: Normalize blood type
void normalizeBloodType(char blood[])
{
    if (blood[0] >= 'a' && blood[0] <= 'z')
        blood[0] = blood[0] - 'a' + 'A';

    if (blood[1] == 'p' || blood[1] == 'P')
        blood[1] = 'P';
    else if (blood[1] == 'n' || blood[1] == 'N')
        blood[1] = 'N';
}

// VALIDATION FUNCTIONS
int isValidID(int id, struct Donor donor[], int count)
{
    if (id <= 0)
        return 0;
    for (int i = 0; i < count; i++)
        if (donor[i].id == id)
            return 0;
    return 1;
}

int isValidName(char name[])
{
    for (int i = 0; name[i]; i++)
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    return 1;
}

int isValidAge(int age)
{
    return (age >= 18 && age <= 65);
}

int isValidPhone(char phone[])
{
    int len = strlen(phone);
    if (len != 11)
        return 0;
    for (int i = 0; i < len; i++)
        if (!isdigit(phone[i]))
            return 0;
    return 1;
}

int isValidGender(char gender)
{
    gender = toupper(gender);
    return (gender == 'M' || gender == 'F');
}

int isValidBlood(char blood[])
{
    normalizeBloodType(blood);
    for (int i = 0; i < 8; i++)
        if (strcmp(blood, bloodTypes[i]) == 0)
            return 1;
    return 0;
}

// MAIN FUNCTION
int main()
{
    if (!adminLogin())
        return 0;

    struct Donor donors[1000];
    int donorCount = LoadToFile(donors);
    updateStockFromDonors(donors, donorCount);

    int choice;

    do
    {
        printMenu();
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add_donor(donors, &donorCount);
            updateStockFromDonors(donors, donorCount);
            SavetoFile(donors, donorCount);
            break;

        case 2:
            search_donor(donors, donorCount);
            break;

        case 3:
            update_donor(donors, donorCount);
            updateStockFromDonors(donors, donorCount);
            SavetoFile(donors, donorCount);
            break;

        case 4:
            view_donor(donors, donorCount);
            break;

        case 5:
            delete_donor(donors, &donorCount);
            updateStockFromDonors(donors, donorCount);
            SavetoFile(donors, donorCount);
            break;

        case 6:
            viewBloodStock();
            break;

        case 7:
            searchByName(donors, donorCount);
            break;

        case 8:
            searchByBloodGroup(donors, donorCount);
            break;

        case 9:
            SavetoFile(donors, donorCount);
            printf("Exiting the program. Goodbye!\n");
            return 0;

        default:
            printf("Invalid Input Or Choice\n");
        }

    } while (choice != 9);

    return 0;
}

// PRINT MENU FUNCTION
void printMenu()
{
    printf("\n================== Welcome to Blood Management System =========================\n");
    printf("\t Enter The choice(1-9): \n");
    printf("\t 1) Add Donor\n");
    printf("\t 2) Search Donor by ID\n");
    printf("\t 3) Update Donor\n");
    printf("\t 4) View Donor\n");
    printf("\t 5) Delete Donor\n");
    printf("\t 6) View Blood Stock\n");
    printf("\t 7) Search by Name\n");
    printf("\t 8) Search by Blood Group\n");
    printf("\t 9) Exit\n");
}

// ADD DONOR FUNCTION
void add_donor(struct Donor donor[], int *count)
{
    if (*count >= 1000)
    {
        printf("Cannot Add More Donor \n");
        return;
    }

    struct Donor newDonor;

    do
    {
        printf("Enter ID Donor : ");
        scanf("%d", &newDonor.id);
        if (!isValidID(newDonor.id, donor, *count))
            printf("Invalid ID or ID already exists. Try again.\n");
        else
            break;
    } while (1);

    do
    {
        printf("Enter the Name : ");
        scanf(" %[^\n]", newDonor.name);
        if (!isValidName(newDonor.name))
            printf("Invalid Name. Only letters and spaces allowed.\n");
        else
            break;
    } while (1);

    do
    {
        printf("Enter the Age : ");
        scanf("%d", &newDonor.age);
        if (!isValidAge(newDonor.age))
            printf("Invalid Age. Must be 18-65.\n");
        else
            break;
    } while (1);

    do
    {
        printf("Enter the Phone Number : ");
        scanf("%s", newDonor.Number);
        if (!isValidPhone(newDonor.Number))
            printf("Invalid Phone. Must be 11 digits.\n");
        else
            break;
    } while (1);

    do
    {
        printf("Enter the Gender (M/F) : ");
        scanf(" %c", &newDonor.gender);
        if (!isValidGender(newDonor.gender))
            printf("Invalid Gender. Must be M or F.\n");
        else
            break;
    } while (1);

    do
    {
        printf("Enter the BloodType : ");
        scanf("%s", newDonor.bloodType);
        if (!isValidBlood(newDonor.bloodType))
            printf("Invalid Blood Type. Must be A+, A-, B+, B-, O+, O-, AB+, AB-\n");
        else
            break;
    } while (1);

    donor[*count] = newDonor;
    (*count)++;
    printf("New Donor Added Successfully\n");
}

// UPDATE DONOR FUNCTION
void update_donor(struct Donor donor[], int count)
{
    if (count == 0)
    {
        printf("No Donor Found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter the Donor ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (donor[i].id == id)
        {
            found = 1;
            printf("Donor Found. Current Details:\nID: %d\nName: %s\nAge: %d\nPhone: %s\nGender: %c\nBlood Type: %s\n",
                   donor[i].id, donor[i].name, donor[i].age, donor[i].Number, donor[i].gender, donor[i].bloodType);

            printf("\nEnter new details:\n");

            do
            {
                printf("Enter new Name: ");
                scanf(" %[^\n]", donor[i].name);
                if (!isValidName(donor[i].name))
                    printf("Invalid Name. Only letters and spaces allowed.\n");
                else
                    break;
            } while (1);

            do
            {
                printf("Enter new Age: ");
                scanf("%d", &donor[i].age);
                if (!isValidAge(donor[i].age))
                    printf("Invalid Age. Must be 18-65.\n");
                else
                    break;
            } while (1);

            do
            {
                printf("Enter new Phone Number: ");
                scanf("%s", donor[i].Number);
                if (!isValidPhone(donor[i].Number))
                    printf("Invalid Phone. Must be 11 digits.\n");
                else
                    break;
            } while (1);

            do
            {
                printf("Enter new Gender (M/F): ");
                scanf(" %c", &donor[i].gender);
                if (!isValidGender(donor[i].gender))
                    printf("Invalid Gender. Must be M or F.\n");
                else
                    break;
            } while (1);

            do
            {
                printf("Enter new Blood Type: ");
                scanf("%s", donor[i].bloodType);
                if (!isValidBlood(donor[i].bloodType))
                    printf("Invalid Blood Type. Must be A+, A-, B+, B-, O+, O-, AB+, AB-\n");
                else
                    break;
            } while (1);

            printf("Donor updated successfully!\n");
            return;
        }
    }

    if (!found)
        printf("No donor found with ID %d\n", id);
}

// UPDATE BLOOD STOCK
void updateStockFromDonors(struct Donor donor[], int count)
{
    for (int i = 0; i < 8; i++)
        stock[i] = 0;

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (strcmp(donor[i].bloodType, bloodTypes[j]) == 0)
            {
                stock[j]++;
                break;
            }
        }
    }
}

// SEARCH, DELETE, VIEW, SAVE, LOAD, NAME/BLOOD search remain mostly same
void search_donor(struct Donor donor[], int count)
{
    if (count == 0)
    {
        printf("No Donor Found\n");
        return;
    }

    int id;
    printf("Enter the ID of the donor to search: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (donor[i].id == id)
        {
            printf("Donor Found:\nID: %d\nName: %s\nAge: %d\nPhone Number: %s\nGender: %c\nBlood Type: %s\n",
                   donor[i].id, donor[i].name, donor[i].age, donor[i].Number, donor[i].gender, donor[i].bloodType);
            return;
        }
    }
    printf("Donor with ID %d not found.\n", id);
}

void delete_donor(struct Donor donor[], int *count)
{
    if (*count == 0)
    {
        printf("No Donor Found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter the Donor ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++)
    {
        if (donor[i].id == id)
        {
            found = 1;
            char confirm;
            printf("Are you sure you want to delete this donor? (Y/N): ");
            scanf(" %c", &confirm);

            if (confirm == 'Y' || confirm == 'y')
            {
                for (int j = i; j < *count - 1; j++)
                    donor[j] = donor[j + 1];
                (*count)--;
                printf("Donor deleted successfully.\n");
            }
            else
            {
                printf("Deletion cancelled.\n");
            }
            return;
        }
    }
    if (!found)
        printf("No donor found with ID %d\n", id);
}

void view_donor(struct Donor donor[], int count)
{
    if (count == 0)
    {
        printf("No donors available.\n");
        return;
    }

    printf("\n==== Donor List ====\n");
    for (int i = 0; i < count; i++)
        printf("ID: %d\nName: %s\nAge: %d\nNumber: %s\nGender: %c\nBlood Type: %s\n\n",
               donor[i].id, donor[i].name, donor[i].age, donor[i].Number, donor[i].gender, donor[i].bloodType);
}

// BLOOD STOCK MANAGEMENT WITH VALIDATION
void viewBloodStock()
{
    int choice;
    while (1)
    {
        printf("\n===== BLOOD STOCK MANAGEMENT =====\n");
        printf("1. View Blood Stock\n2. Add Blood Units\n3. Remove Blood Units\n4. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        int opt, units;
        if (choice == 4)
            break;
        else if (choice == 1)
        {
            printf("\n--- Current Blood Stock ---\n");
            for (int i = 0; i < 8; i++)
                printf("%s : %d units\n", bloodTypes[i], stock[i]);
        }
        else if (choice == 2)
        {
            printf("\nSelect Blood Type to Add:\n");
            for (int i = 0; i < 8; i++)
                printf("%d. %s\n", i + 1, bloodTypes[i]);
            do
            {
                printf("Enter option (1-8): ");
                scanf("%d", &opt);
                if (opt < 1 || opt > 8)
                    printf("Invalid option.\n");
            } while (opt < 1 || opt > 8);
            do
            {
                printf("Enter units to add: ");
                scanf("%d", &units);
                if (units <= 0)
                    printf("Invalid number of units.\n");
            } while (units <= 0);
            stock[opt - 1] += units;
            printf("%d units added to %s.\n", units, bloodTypes[opt - 1]);
        }
        else if (choice == 3)
        {
            printf("\nSelect Blood Type to Remove:\n");
            for (int i = 0; i < 8; i++)
                printf("%d. %s (%d units)\n", i + 1, bloodTypes[i], stock[i]);
            do
            {
                printf("Enter option (1-8): ");
                scanf("%d", &opt);
                if (opt < 1 || opt > 8)
                    printf("Invalid option.\n");
            } while (opt < 1 || opt > 8);
            do
            {
                printf("Enter units to remove: ");
                scanf("%d", &units);
                if (units <= 0 || units > stock[opt - 1])
                    printf("Invalid number of units.\n");
            } while (units <= 0 || units > stock[opt - 1]);
            stock[opt - 1] -= units;
            printf("%d units removed from %s.\n", units, bloodTypes[opt - 1]);
        }
        else
            printf("Invalid choice.\n");
    }
}

// SAVE AND LOAD
void SavetoFile(struct Donor donor[], int count)
{
    FILE *file = fopen("donors.txt", "w");
    if (!file)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++)
        fprintf(file, "%d,%s,%d,%s,%c,%s\n",
                donor[i].id, donor[i].name, donor[i].age, donor[i].Number, donor[i].gender, donor[i].bloodType);

    fclose(file);
    printf("\nData saved successfully to donors.txt\n");
}

int LoadToFile(struct Donor donor[])
{
    FILE *file = fopen("donors.txt", "r");
    if (!file)
    {
        printf("No existing data found.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%d,%49[^,],%d,%14[^,],%c,%4s\n",
                  &donor[count].id,
                  donor[count].name,
                  &donor[count].age,
                  donor[count].Number,
                  &donor[count].gender,
                  donor[count].bloodType) == 6)
        count++;

    fclose(file);
    printf("\nData loaded successfully (%d donors).\n", count);
    return count;
}

// SEARCH BY NAME
void searchByName(struct Donor donor[], int count)
{
    if (count == 0)
    {
        printf("No donors available.\n");
        return;
    }

    char name[50];
    printf("Enter the name to search: ");
    scanf(" %[^\n]", name);

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcasecmp(donor[i].name, name) == 0)
        {
            printf("\nDonor Found:\nID: %d\nName: %s\nAge: %d\nPhone: %s\nGender: %c\nBlood Type: %s\n\n",
                   donor[i].id, donor[i].name, donor[i].age, donor[i].Number, donor[i].gender, donor[i].bloodType);
            found = 1;
        }
    }
    if (!found)
        printf("No donor found with the name: %s\n", name);
}

// SEARCH BY BLOOD GROUP
void searchByBloodGroup(struct Donor donor[], int count)
{
    if (count == 0)
    {
        printf("No donors available.\n");
        return;
    }

    char blood[4];
    printf("Enter Blood Group: ");
    scanf("%s", blood);
    normalizeBloodType(blood);

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(donor[i].bloodType, blood) == 0)
        {
            printf("\nID: %d\nName: %s\nAge: %d\nPhone: %s\nGender: %c\nBlood: %s\n\n",
                   donor[i].id, donor[i].name, donor[i].age, donor[i].Number, donor[i].gender, donor[i].bloodType);
            found = 1;
        }
    }
    if (!found)
        printf("No donor found.\n");
}

// ADMIN LOGIN
int adminLogin()
{
    char password[20];
    while (1)
    {
        printf("Enter admin password: ");
        scanf("%s", password);
        if (strcmp(password, "123") == 0)
        {
            printf("Access granted.\n");
            return 1;
        }
        else
            printf("Access denied. Try again!\n");
    }
}