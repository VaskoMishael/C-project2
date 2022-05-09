#include <stdio.h>
#include <string.h>
#include "stdlib.h"

typedef struct author {
    char name[100];
    char surname[100];
    struct author *next;
} AUTHOR;

typedef struct record {
    int ID;
    char name[150];
    AUTHOR *authors;
    char presentation[3];
    int time;
    int date;
    struct record *next;
} RECORD;

void addNewAuthor(AUTHOR **firstAuthor, char name[100], char surname[100]) {
    AUTHOR *newAuthor = (AUTHOR *) malloc(sizeof(AUTHOR));
    AUTHOR *last = *firstAuthor;

    strcpy(newAuthor->name, name);
    strcpy(newAuthor->surname, surname);
    newAuthor->next = NULL;

    if (*firstAuthor == NULL) {
        *firstAuthor = newAuthor;
        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = newAuthor;
}

void
addNewRecord(RECORD **first, AUTHOR **firstAuthor, int ID, char name[150], char presentation[3], int time, int date) {
    RECORD *newRecord = (RECORD *) malloc(sizeof(RECORD));
    RECORD *last = *first;
    newRecord->ID = ID;                                                         //input data to new element of linked list
    strcpy(newRecord->name, name);
    newRecord->authors = *firstAuthor;
    strcpy(newRecord->presentation, presentation);
    newRecord->time = time;
    newRecord->date = date;

    newRecord->next = NULL;

    if (*first ==
        NULL) {                                                         // if there's no head in list, make new element head
        *first = newRecord;
        return;
    }

    while (last->next !=
           NULL) {                                                  // else find last element in linked list
        last = last->next;
    }
    last->next = newRecord;                                                       // and link it to the new element
}

void n(FILE **f, int *quantity, RECORD **first) {
    char str[200], name[150], authorsName[100], authorsSurname[100], presentation[3], *token;
    int i = 0, ID = 0, time = 0, date = 0, word = 0;
    AUTHOR *firstAuthor = NULL;
    RECORD *tmp;
    *quantity = 0;
    if (*f == NULL) {
        *f = fopen("./OrganizacnePodujatia2.txt", "r+");  //open file
    }
    if (*f == NULL) {
        printf("Zaznamy neboli nacitane!\nSkontrolujte cestu k suboru\n");
        return;
    }
    while (*first != NULL) {        //free memory
        tmp = *first;
        (*first) = (*first)->next;
        free(tmp);
//        printf("Done\n");
    }
    fseek(*f, 0, SEEK_SET);                                                       //read from the file
    while (fgets(str, 200, *f) != NULL) {
        int length = strlen(str);
        if (str[length - 1] == '\n') {
            str[length - 1] = '\0';
        }
        switch (i) {
            case 0:
                (*quantity)++;
                i++;
                break;
            case 1:
                ID = atol(str);                                                         //write data to local variables
                i++;
                break;
            case 2:
                strcpy(name, str);
                i++;
                break;
            case 3:                                                         //create linked list of authors
                firstAuthor = NULL;
                word = 1;
                token = strtok(str, " #");
                strcpy(authorsName, token);
                while (token != NULL) {
                    if (word % 2 == 0) {
                        strcpy(authorsSurname, token);
                        addNewAuthor(&firstAuthor, authorsName, authorsSurname);
                    } else {
                        strcpy(authorsName, token);
                    }
                    token = strtok(NULL, " #");
                    word++;
                }
                i++;
                break;
            case 4:
                strcpy(presentation, str);
                i++;
                break;
            case 5:
                time = atol(str);
                i++;
                break;
            case 6:
                date = atol(str);
                addNewRecord(first, &firstAuthor, ID, name, presentation, time,
                             date);      //add to the end of the list new node(element)
                i = 0;
                break;
            default:
                printf("Error\n");
                break;
        }
    }
    printf("Nacitalo sa %d zaznamov\n", *quantity);
}

void v(RECORD *first) {
    RECORD *temp = first;
    int num = 1, list = 1, l = 0;
    if (temp == NULL) {
        printf("Prazdny zoznam zaznamov\n");
        return;
    }
    while (temp != NULL) {
        printf("%d.\n", list);
        printf("ID cislo: %d\n", temp->ID);
        printf("Nazov prispevku: %s\n", temp->name);
        printf("Mena autorov:\n");
        AUTHOR *tempAuthor = temp->authors;
        while (tempAuthor != NULL) {
            printf("    %d: %s %s\n", num, tempAuthor->name, tempAuthor->surname);
            num++;
            tempAuthor = tempAuthor->next;
        }
        num = 1;
        printf("Typ prezentovania: %s\n", temp->presentation);
        printf("Cas prezentovania: %d\n", temp->time);
        printf("Datum: %d\n", temp->date);
        temp = temp->next;
        list++;
    }
}

void p(int *quantity, RECORD **first) {
    char str[200], name[150], authorsName[100], authorsSurname[100], presentation[3], *token;
    int number = 100, ID = 0, time = 0, date = 0, word = 0, node = 1;
    AUTHOR *firstAuthor;
    RECORD *newRecord = (RECORD *) malloc(
            sizeof(RECORD)), *temp;                                         //allocate memory for new node
    newRecord->next = NULL;
    printf("Enter after which node to insert:\n");
    scanf("%d", &number);
    while (getchar() != '\n');

    if (number <= 0) {
        printf("Number must be more than 0\n");
        return;
    }

    scanf("%d %[^\n] %[^\n] %s %d %d", &ID, name, str, presentation, &time, &date);
    while (getchar() != '\n');

    firstAuthor = NULL;                                                         //create new linked list of authors
    word = 1;
    token = strtok(str, " #");
    strcpy(authorsName, token);
    while (token != NULL) {
        if (word % 2 == 0) {
            strcpy(authorsSurname, token);
            addNewAuthor(&firstAuthor, authorsName, authorsSurname);
        } else {
            strcpy(authorsName, token);
        }
        token = strtok(NULL, " #");
        word++;
    }
    newRecord->ID = ID;                                                         //enter data to new node
    strcpy(newRecord->name, name);
    newRecord->authors = firstAuthor;
    strcpy(newRecord->presentation, presentation);
    newRecord->time = time;
    newRecord->date = date;

    if (number > *quantity) {
        addNewRecord(first, &firstAuthor, ID, name, presentation, time, date);     //add to the end
    } else if (number == 1) {                                                         //add to the start
        newRecord->next = *first;
        *first = newRecord;
    } else {                                                         //add to the position
        temp = *first;
        while (node != number - 1) {
            temp = temp->next;
            node++;
        }
        newRecord->next = temp->next;
        temp->next = newRecord;
    }
    (*quantity)++;
}

void z(RECORD **first, int *quantity) {
    if ((*first) == NULL) {
        printf("Struct has no data\n");
        return;
    }
    int found = 0;
    char str[200], fullName[200];
    char nameToDelete[100];
    char surnameToDelete[100];
    RECORD *temp = *first, *previous = *first;

    printf("Enter name:\n");
    scanf("%s %s", nameToDelete, surnameToDelete);
    while (getchar() != '\n');

    int counter = 0;
    while (temp != NULL) {//loop through all nodes
        found = 0;
        AUTHOR *tempAuthor = temp->authors;
        while (tempAuthor != NULL) {       //loop through all authors
            if (strcasecmp(tempAuthor->name, nameToDelete) == 0 &&
                strcasecmp(tempAuthor->surname, surnameToDelete) == 0) {
                found = 1;
                printf("Prispevok s nazvom %s bol vymazany\n", temp->name);
                (*quantity)--;
            }
            tempAuthor = tempAuthor->next;
        }
        if (found) {
            if (counter == 0) {
                *first = temp->next;
                free(temp);
                temp = *first;
            } else {
                previous->next = temp->next;
                free(temp);
                temp = previous->next;
            }
        } else {
            previous = temp;
            temp = temp->next;
            counter++;
        }

    }
}

void h(RECORD *first) {
    int num = 1, list = 1, exists = 0;
    RECORD *temp = first;
    char type[3];
    scanf("%s", type);
    while (getchar() != '\n');
    while (temp != NULL) {
        if (strcmp(temp->presentation, type) ==
            0) {//in every node compare type, output data of node, if it suits demands
            exists = 1;
            printf("%d.\n", list);
            printf("ID cislo: %d\n", temp->ID);
            printf("Nazov prispevku: %s\n", temp->name);
            printf("Mena autorov:\n");
            AUTHOR *tempAuthor = temp->authors;
            while (tempAuthor != NULL) {
                printf("    %d: %s %s\n", num, tempAuthor->name, tempAuthor->surname);
                num++;
                tempAuthor = tempAuthor->next;
            }
            num = 1;
            printf("Typ prezentovania: %s\n", temp->presentation);
            printf("Cas prezentovania: %d\n", temp->time);
            printf("Datum: %d\n", temp->date);
            list++;
        }
        temp = temp->next;
    }
    if (exists == 0) {
        printf("Pre typ:%s nie su ziadne zaznamy\n", type);
    }
}

void a(RECORD **first) {
    int id, run = 1;
    RECORD *temp = *first;
    char newType[3], prevType[3];

    while (run) {                                             //loop till user inputs valid data
        scanf("%d %s", &id, newType);
        if (id % 15 == 0 && (strcmp(newType, "UP") == 0 || strcmp(newType, "UD") == 0 || strcmp(newType, "PD") == 0 ||
                             strcmp(newType, "PP") == 0)) {
            run = 0;
        } else printf("Zadane udaje nie su korektne, zadaj novy retazec:\n");
    }
    while (temp->ID != id) { //get node corresponding to it's ID
        temp = temp->next;
    }
//    printf("Found record, it's %s\n", temp->name);
    strcpy(prevType, temp->presentation);
    strcpy(temp->presentation, newType);                 //change data
    printf("Prispevok s nazvom %s sa bude prezentovat %s [%s]\n", temp->name, newType, prevType);
}

void r(RECORD **first, int *quantity) {
    RECORD *aPrev = NULL, *bPrev = NULL, *aCurr = *first, *bCurr = *first, *buff = NULL;
    int a = 0, b = 0, m = 0, n = 0, run = 1;
    while (run) {
        printf("Enter position of first and second nodes:\n");

        scanf("%d %d", &a, &b);
        while (getchar() != '\n');

        if (a > 0 && a < *quantity + 1 && b > 0 && b < *quantity + 1) {
            run = 0;
        } else printf("Pozicia neexistuje\n");
    }
    if (a == b) {
        printf("Nodes can't have the same value\n");
        return;
    }

    while (m != a - 1) {                                        //get first node
        aPrev = aCurr;
        aCurr = aCurr->next;
        m++;
    }
//    printf("ID of %d-th element is %d\n", a, aCurr->ID);
    while (n != b - 1) {                                       //get second node
        bPrev = bCurr;
        bCurr = bCurr->next;
        n++;
    }
//    printf("ID of %d-th element is %d\n", b, bCurr->ID);

    if (aPrev == NULL) {                                        //if first node was head, make second node head
        *first = bCurr;
    } else aPrev->next = bCurr;

    if (bPrev == NULL) {                                        //if second node was head, make first node head
        *first = aCurr;
    } else bPrev->next = aCurr;

    buff = aCurr->next;                                         //swap pointers
    aCurr->next = bCurr->next;
    bCurr->next = buff;
}

void k(RECORD **first, FILE **f) {
    RECORD *tmp;
    while (*first != NULL) {        //free memory
        tmp = *first;
        *first = (*first)->next;
        free(tmp);
    }
    fclose(*f);
}

int main(int argc, char const *argv[]) {                                       //main
    char command;
    int true = 1, quantity = 0, count = 0;
    FILE *f = NULL;
    RECORD *first = NULL;
    while (true) {                                                            //while loop for main
        printf("Enter your command please:");
        scanf("%c", &command);
        while (getchar() != '\n');
        switch (command) {
            case 'n':
                n(&f, &quantity, &first);
                break;
            case 'v':
                v(first);
                break;
            case 'p':
                p(&quantity, &first);
                break;
            case 'z':
                z(&first, &quantity);
                break;
            case 'h':
                h(first);
                break;
            case 'a':
                a(&first);
                break;
            case 'r':
                r(&first, &quantity);
                break;
            case 'k':
                k(&first, &f);
                true = 0;
                break;
            default:
                printf("Can't recognise your command.\n");
                break;
        }
    }
    return 0;
}
