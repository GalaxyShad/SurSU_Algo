#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


#define HASH_TABLE_SIZE     511             // M
#define N                   2000            // K

#define BUFFER_SIZE         128

typedef struct _Person { 
    char Surname[BUFFER_SIZE]; 
} Person;

//////////////////////// UTILS /////////////////////////////////////////////////

int Exists(Person* personArr, Person* person) {
    for (int i = 0; i < N; i++) {
        if ((strcmp(personArr[i].Surname, person->Surname) == 0) && 
            (strcmp(personArr[i].Surname, "none") != 0))

            return 1;
    }

    return 0;
}

int ReadPersons(Person* personArr) {
    FILE* inputFile = fopen("l7data.txt", "r");
    if (!inputFile) return 0;

    char buff[BUFFER_SIZE];
    int i = 0;
    while (fscanf(inputFile, "%s", buff) == 1) {
        Person temp;
        strcpy(temp.Surname, buff);

        if (Exists(personArr, &temp)) {
            sprintf(personArr[i].Surname, "%s%d", buff, i);
        } else {
            strcpy(personArr[i].Surname, buff);
        }
        i++;
    }

    fclose(inputFile);

    return 1;
}


////////////////////////////////////////////////////////////////////////////////

uint32_t GetHash(const Person* keyPerson) {
    uint32_t hash = 0;

    for (int i = 0; i < strlen(keyPerson->Surname); i++) {
        //hash = (hash << 5) - hash + keyPerson->Surname[i];
        hash = (hash << 2) - hash + keyPerson->Surname[i];      // Забрутфорсил число :D
    }

    return hash % HASH_TABLE_SIZE;
}

void PrintPersonSurnames(Person* personArr) {
    for (int i = 0; i < N; i++) {
        printf("0x%03x %s\n", GetHash(&personArr[i]), personArr[i].Surname);
    }
}

int main() {
    Person personArr[N];
    for (int i = 0; i < N; strcpy(personArr[i].Surname, "none"), i++);
    
    if (!ReadPersons(personArr)) printf("[ERR] Open a file");

    int personHashMap[HASH_TABLE_SIZE] = {0};

    // Collisions calculations
    for (int i = 0; i < N; i++) {
        personHashMap[GetHash(&personArr[i])]++;
    }

    // Print Collisions
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        printf("%d;%d\n", i+1, personHashMap[i]);
    }

    double k    = HASH_TABLE_SIZE / (double) N;
    double k_   = N / (double) HASH_TABLE_SIZE;

    double stats = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        stats += (personHashMap[i] - k_) * (personHashMap[i] - k_);
    }

    printf("M=%d N=%d\n", HASH_TABLE_SIZE, N);

    printf(
        "%lf [%lf %lf]", 
        k * stats, 
        HASH_TABLE_SIZE - sqrt(HASH_TABLE_SIZE), 
        HASH_TABLE_SIZE + sqrt(HASH_TABLE_SIZE)
    );

    return 0;
}