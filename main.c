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
    tempObj tempData;
    // city* next;
    // city* prev;
} city;

typedef struct {
    city* cities;
    size_t size;
    city* endptr;
} citytable;

citytable* createTable() {
    city cities[TABLESIZE];
    size_t size = sizeof(cities) / sizeof(city);
    city* endptr = cities + size;

    citytable* table;
    table->cities = cities;
    table->endptr = endptr;
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

city* add(citytable* const table, const city* const cityadd) {
    // Check if already in table
    
    // probably memset some sizeof(city) at the endptr and initliaze to cityToAdd
    table->size += 1;
    city* newLoc = realloc(table->cities, sizeof(city));
    memcpy(table->endptr, cityadd, table->size * sizeof(city));

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

    return endptr;
    */
}

void destroy() {

}

int main(int argc, char *argv[]) {
    if (argc < 2) { fprintf(stderr, "Error: Too few arguements."); return -1; }
    if (argc > 2) { fprintf(stderr, "Error: Too many arguments."); return -1; }

    const char* fileName = argv[1];
    FILE* infile = fopen(fileName, "r");
    if (!infile) { fprintf(stderr, "Error: File could not open. Check name and extension."); return -1; }

    citytable* table = createTable();

    // Pointer math to parse the city name and temp by the semicolon
    char line[LINELENGTH];
    while (fgets(line, LINELENGTH, infile)) {
        char* delim = strpbrk(line, ";");
        if (delim) {
            *delim = '\0';
            char* strTemp = delim + 1;

            city adder = {
                line, 
                {
                    0.0,
                    0, // cnt
                    0.0, // avg
                    0.0, // min
                    0.0 // max
                } 
            };

            // Convert and store
            city* found;
            if (found = search(table, line)) {
                float temp = strtof(strTemp, NULL);
                tempObj data = found->tempData;
                
                data.sum += temp;
                data.cnt += 1;
                if (temp < data.min) data.min = temp;
                else if (temp > data.max) data.max = temp;
            }
            else {

            }
        }
        else {
            fprintf(stderr, "Error: No city found. Check input file formatting.");
            continue;
        }
    
    }

    fclose(infile);

    // Calculate and Output
    const char* ofileName = argv[2];
    FILE* ofile = fopen(ofileName, "w");
    fclose(ofile);
}
