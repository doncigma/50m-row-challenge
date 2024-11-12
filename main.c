#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define LINELENGTH 106
#define TABLESIZE 1000
#define CITYNAMELENGTH 101

typedef struct {
    float sum;
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

#define FNVPRIME 
#define FNVOFSSET 2166136261U

/// @brief FNV-1a hash implementation.
/// @param key String to hash.
/// @return The hashed value divided by table size.
unsigned long hash(const char* key) {
    unsigned long hash = 2166136261U; // fnv offset basis
    unsigned char letter;

    while(letter = *key++) {
        hash ^= letter;
        hash *= 16777619; // fnv prime
    }
    
    return hash % TABLESIZE;
}

city* search(citytable* const table, const char* const key) {
    // for (city* iter = table->cities; iter != table->endptr; iter++)
    //     if (strcmp(iter->key, key) == 0) return iter;
    // return NULL;
    city* found = &table->cities[hash(key)];
    if (strcmp(found->key, key) == 0) return found;
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
        found->tempData.sum += temp;
        found->tempData.cnt += 1;
        if (temp < found->tempData.min) found->tempData.min = temp;
        else if (temp > found->tempData.max) found->tempData.max = temp;
        // return found;
    }
    // Make new city and append if not in table
    else {
        table->size += 1;
        table->endptr = &table->cities[table->size - 1];
       
        city* cityadd = &table->cities[table->size - 1];
        // city* cityadd = &table->cities[hash(key)];
        strncpy(cityadd->key, key, CITYNAMELENGTH);
        cityadd->tempData.sum = temp;
        cityadd->tempData.cnt = 1;
        cityadd->tempData.min = temp;
        cityadd->tempData.max = temp;

        // return cityadd;
    }
}

int main(int argc, char *argv[]) {
    // if (argc < 2) { fprintf(stderr, "Err: Too few arguements.\n"); return -1; }
    // if (argc > 3) { fprintf(stderr, "Err: Too many arguments.\n"); return -1; }

    // const char* fileName = argv[1];
    // const char* ofileName = argv[2];

    // FILE* infile = fopen(fileName, "r");
    // if (!infile) { fprintf(stderr, "Err: File could not open. Check name and extension.\n"); return -1; }
    
    // const char* fileName = "test.txt";
    const char* fileName = "measurements.txt";
    const char* ofileName = "output.txt";

    FILE* infile = fopen(fileName, "r");
    FILE* ofile = fopen(ofileName, "w");
    if (!infile || !ofile) { fprintf(stderr, "Err: File could not open. Check name and extension.\n"); return -1; }

    citytable table;
    table.size = 0;
    table.citysizeof = sizeof(city);
    table.tempsizeof = sizeof(tempstruct);
    table.endptr = table.cities + TABLESIZE;

    // Pointer math to parse the city name and temp by the semicolon
    int i = 0;
    char line[LINELENGTH];
    while (fgets(line, LINELENGTH, infile)) {
        char* delim = strpbrk(line, ";");
        if (!delim) { fprintf(stderr, "Err: No city found. Line: %d.\n", i); continue; }
        
        *delim = '\0';
        float temp = strtof(delim + 1, NULL);
        add(&table, line, temp);
        // city* tmp = add(&table, line, strtof(delim + 1, NULL));
        // if (!tmp) { fprintf(stderr, "Err: Problem allocating memory while adding. Line: %d.\n", i); continue; }

        // fprintf(ofile, "City: %s, Temp: %0.1f, Sum: %0.1f, Cnt: %d, Min: %0.1f, Max: %0.1f\n", tmp->key, temp, tmp->tempData.sum, tmp->tempData.cnt, tmp->tempData.min, tmp->tempData.max);
        i++;
    }

    fclose(infile);

    // Calculate and Output
    for (int i = 0; i < table.size; i++) {
        char* key = table.cities[i].key;
        tempstruct data = table.cities[i].tempData;

        fprintf(ofile, "%s,%0.1f,%0.1f,%0.1f\n", key, data.min, data.sum / data.cnt, data.max);
    }

    fclose(ofile);
}
