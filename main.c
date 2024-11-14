#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define TABLESIZE 1000
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
    city* endptr;
    int size;
    size_t citysizeof;
    size_t tempsizeof;
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
    city* found = &table->cities[hash(key)];
    if (found && (strcmp(found->key, key) != 0) ) return found;
    return NULL;
}

/// @brief Attempts to add a city to the table. If key is already in the table, it updates its values with new temp. Otherwise, it constructs a new city and appends.
/// @param table The table being added to.
/// @param key The city key to add.
/// @param temp The temp of the city to add.
/// @return Pointer to the existing or newly constructed city.
void add(citytable* const table, char* const key, const float temp) {
    // Add temp if already in table
    city* found;
    if (table->size > 0 && (found = search(table, key))) {
        // COLLISIONS

        found->tempData.sum += temp;
        found->tempData.cnt += 1;
        if (temp < found->tempData.min) found->tempData.min = temp;
        else if (temp > found->tempData.max) found->tempData.max = temp;
    }
    // Make new city and append if not in table
    else {
        // COLLISIONS

        int index = hash(key);

        city* cityadd = &table->cities[index];
        strcpy(cityadd->key, key);
        cityadd->tempData.sum = temp;
        cityadd->tempData.cnt = 1;
        cityadd->tempData.min = temp;
        cityadd->tempData.max = temp;

        table->size += 1;
        table->endptr = &table->cities[table->size - 1];
    }
}

int main(int argc, char *argv[]) {
    // if (argc < 2) { fprintf(stderr, "Err: Too few arguements.\n"); return 1; }
    // if (argc > 3) { fprintf(stderr, "Err: Too many arguments.\n"); return 1; }

    // const char* fileName = argv[1];
    // const char* ofileName = argv[2];
    
    // const char* fileName = "test.txt";
    const char* fileName = "measurements.txt";
    const char* ofileName = "output.txt";

    int fd = open(fileName, O_RDONLY);
    if (fd == -1) { fprintf(stderr, "Err: Input file could not open. Check name and extension.\n"); return 1; }

    struct stat st;
    if (fstat(fd, &st) == -1) { fprintf(stderr, "Err: Could not retrieve input file metadata.\n"); close(fd); return 1; }
    
    char* input = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (input == (void*)-1) { fprintf(stderr, "Mmap failed.\n"); close(fd); return 1; }
    
    citytable table;
    table.size = 0;
    table.citysizeof = sizeof(city);
    table.tempsizeof = sizeof(tempstruct);
    table.endptr = table.cities + TABLESIZE;

    // Pointer math to parse the city name and temp by the semicolon
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

    // int fd = open(ofileName, O_WRONLY);
    // if (fd == -1) { fprintf(stderr, "Err: Output file could not open. Check name and extension.\n"); return 1; }

    for (int i = 0; i < table.size; i++) {
        char* key = table.cities[i].key;
        tempstruct data = table.cities[i].tempData;
        // figure out averages because float math
        // check if float is infinity and if is then do something about the calculation
        double avg = data.sum / data.cnt;

        fprintf(ofile, "%s,%0.1f,%0.1lf,%0.1f\n", key, data.min, avg, data.max);
        // char line[] = ""
        // write(fd, );
    }

    fclose(ofile);
}
