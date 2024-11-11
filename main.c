#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define TABLESIZE 1000
#define LINELENGTH 106

typedef struct {
    float sum;
    int cnt;
    float avg;
    float min;
    float max;
} tempObj;

typedef struct {
    char* key;
    tempObj* tempData;
} city;

typedef struct {
    city* cities;
    int size;
    city* endptr;
    size_t citySize;
    size_t tempObjSize;
} citytable;

citytable* createTable() {
    city* cities = malloc(sizeof(city) * TABLESIZE);
    if (!cities) { return NULL; }
    
    citytable* table = malloc(sizeof(citytable));
    if (!table) { free(cities); return NULL; }

    table->cities = cities;
    table->size = TABLESIZE;
    table->citySize = sizeof(city);
    table->tempObjSize = sizeof(tempObj);
    table->endptr = cities + TABLESIZE;
    return table;
}

// SLOWDOWN: Need to improve this to O(1) lookup with a custom hash function
city* search(const citytable* const table, const char* const searchKey) {
    city* cities = table->cities;
    for (size_t i = 0; i < table->size; i++) {
        if (strcmp(cities[i].key, searchKey) == 0) {
            return &cities[i];
        }
    }
    return NULL;
}

city* add(citytable* const table, const char* const key, const float* const temp) {
    // Add temp if already in table
    city* found;
    if (found = search(table, key)) {
        tempObj* data = found->tempData;
        data->sum += *temp;
        data->cnt += 1;
        if (*temp < data->min) data->min = *temp;
        else if (*temp > data->max) data->max = *temp;
        return found;
    }
    
    table->size += 1;
    city* newend = realloc(table->cities, sizeof(city));
    if (!newend) return NULL;
    table->endptr = newend;

    tempObj* tempData = malloc(sizeof(tempObj));
    if (!tempData) { realloc(table->cities, sizeof(city) * table->size); return NULL; }
    tempData->sum = *temp;
    tempData->cnt = 1;
    tempData->min = *temp;
    tempData->max = *temp;

    city* cityadd = malloc(sizeof(city));
    if (!cityadd) { free(tempData); return NULL; }
    cityadd->key = key;
    cityadd->tempData = tempData;

    memcpy(table->endptr, cityadd, sizeof(city));
    memset(newend, cityadd, sizeof(city));

    /*
    // Reallocate memory for the new element
    city* temp = realloc(*cities, (*size) * sizeof(city));
    if (temp == NULL) {
        // Handle allocation failure
        perror("Failed to allocate memory");
        return NULL;
    }
    *cities = temp;

    // Set endptr to the correct new end position
    city* endptr = &(*cities)[*size - 1]; // This points to the new last element, which is valid and contiguous
    memcpy(endptr, &cityToAdd, sizeof(city)); // Copy data into the newly allocated space
    */
}

void destroy(citytable* table) {
    free(table->cities);
    free(table);
}

int main(int argc, char *argv[]) {
    if (argc < 2) { fprintf(stderr, "Error: Too few arguements.\n"); return -1; }
    if (argc > 2) { fprintf(stderr, "Error: Too many arguments.\n"); return -1; }

    const char* fileName = argv[1];
    FILE* infile = fopen(fileName, "r");
    if (!infile) { fprintf(stderr, "Error: File could not open. Check name and extension.\n"); return -1; }

    citytable* table = createTable();

    // Pointer math to parse the city name and temp by the semicolon
    int i = 0;
    char line[LINELENGTH];
    while (fgets(line, LINELENGTH, infile)) {
        char* delim = strpbrk(line, ";");
        if (!delim) { fprintf(stderr, "Error: No city found. Line: %d.\n", i); continue; }
        
        *delim = '\0';
        char* strTemp = delim + 1;

        // Convert and store
        city* found;
        float temp = strtof(strTemp, NULL);
        add(table, line, &temp);
        i++;
    }

    fclose(infile);

    // Calculate and Output
    const char* ofileName = argv[2];
    FILE* ofile = fopen(ofileName, "w");
    fclose(ofile);
}
