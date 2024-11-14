#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define TABLESIZE 10000
#define CITYNAMELENGTH 101

typedef struct {
    double sum;
    int cnt;
    float min;
    float max;
} tempstruct;

typedef struct {
    char key[CITYNAMELENGTH];
    tempstruct tempData;
} city;

typedef struct {
    city cities[TABLESIZE];
    int size;
} citytable;

#define FNVPRIME 16777619
#define FNVOFSSET 2166136261U

/// @brief FNV-1a hash implementation.
/// @param key String to hash.
/// @return The hashed value divided by table size.
int hash(const char* key) {
    unsigned long hash = FNVOFSSET;
    unsigned char letter;

    while((letter = *key++)) {
        hash ^= letter;
        hash *= FNVPRIME;
    }

    return hash % TABLESIZE;
}

city* search(citytable* const table, const char* const key) {
    int index = hash(key);
    int startindex = index;

    while (table->cities[index].key[0]) {
        if (strcmp(table->cities[index].key, key) == 0) {
            return &table->cities[index];
        }
        
        index = (index + 1) % TABLESIZE;
        if (index == startindex) { fprintf(stderr, "Err: Could not find key in search.\n"); return NULL; }
    }

    return NULL;
}

/// @brief Attempts to add a city to the table. If key is already in the table, it updates its values with new temp. Otherwise, it constructs a new city and appends.
/// @param table The table being added to.
/// @param key The city key to add.
/// @param temp The temp of the city to add.
/// @return Pointer to the existing or newly constructed city.
void add(citytable* const table, char* const key, const float temp) {
    // Add temp if already in table
    city* found = search(table, key);
    if (table->size > 0 && found) {
        found->tempData.sum += temp;
        found->tempData.cnt += 1;
        if (temp < found->tempData.min) found->tempData.min = temp;
        else if (temp > found->tempData.max) found->tempData.max = temp;
    }
    // Make new city and append if not in table
    else {
        // Collision handling
        int index = hash(key);
        int startindex = index;

        while (table->cities[index].key[0] != '\0') {
            index = (index + 1) % TABLESIZE;
            if (index == startindex) { fprintf(stderr, "Err: Could not add, table is full.\n"); return; }
        }

        city* cityadd = &table->cities[index];
        strcpy(cityadd->key, key);
        cityadd->tempData.sum = temp;
        cityadd->tempData.cnt = 1;
        cityadd->tempData.min = temp;
        cityadd->tempData.max = temp;

        table->size += 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) { fprintf(stderr, "Err: Too few arguements.\n"); return 1; }
    if (argc > 3) { fprintf(stderr, "Err: Too many arguments.\n"); return 1; }

    const char* fileName = argv[1];
    const char* ofileName = argv[2];

    int fd = open(fileName, O_RDONLY);
    if (fd == -1) { fprintf(stderr, "Err: Input file could not open. Check name and extension.\n"); return 1; }

    struct stat st;
    if (fstat(fd, &st) == -1) { fprintf(stderr, "Err: Could not retrieve input file metadata.\n"); close(fd); return 1; }
    
    char* input = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (input == (void*)-1) { fprintf(stderr, "Mmap failed.\n"); close(fd); return 1; }
    
    citytable table;
    memset(table.cities, 0, TABLESIZE * sizeof(city));
    table.size = 0;

    // Pointer math to slice the city name and temp by the semicolon
    int i = 0;
    char* currline = input;
    char* end = input + st.st_size;
    while (currline != end) {
        char* newline = strpbrk(currline, "\n");
        if (!newline) { currline = end; continue; }

        size_t len = newline - currline; // excludes newline char
        char addline[len + 1];
        memcpy(addline, currline, len);
        addline[len] = '\0';

        char* delim = strpbrk(addline, ";");
        if (!delim) { fprintf(stderr, "Err: No city found. Skipping line: %d.\n", i); currline = newline + 1; i++; continue; }
        
        *delim = '\0';
        float temp = strtof(delim + 1, NULL);
        add(&table, addline, temp);

        currline = newline + 1;
        i++;
    }

    munmap(input, st.st_size);
    close(fd);

    // Calculate and Output
    FILE* ofile = fopen(ofileName, "w");
    if (!ofile) { fprintf(stderr, "Err: Output file could not open. Check name and extension.\n"); return 1; }

    for (int k = 0; k < TABLESIZE; k++) {
        char* key = table.cities[k].key;
        if (!*key) continue;
        tempstruct data = table.cities[k].tempData;

        fprintf(ofile, "%s,%0.1f,%0.1lf,%0.1f\n", key, data.min, (double)data.sum / data.cnt, data.max);
    }

    fclose(ofile);

    // destroy(table);
}
