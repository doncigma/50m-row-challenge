#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define TABLESIZE 1000
#define LINELENGTH 106

typedef struct {
    float sum;
    int cnt;
    float min;
    float max;
} tempstruct;

typedef struct {
    char* key;
    tempstruct* tempData;
} city;

typedef struct {
    city* cities;
    int size;
    city* endptr;
    size_t citysizeof;
    size_t tempsizeof;
} citytable;

citytable* createTable() {
    city* cities = malloc(sizeof(city) * TABLESIZE);
    citytable* table = malloc(sizeof(citytable));
    if (!cities || !table) { free(cities); free(table); return NULL; }

    table->cities = cities;
    table->size = TABLESIZE;
    table->citysizeof = sizeof(city);
    table->tempsizeof = sizeof(tempstruct);
    table->endptr = cities + TABLESIZE;
    return table;
}

// SLOWDOWN: Need to improve this to O(1) lookup with a custom hash function
city* search(const citytable* const table, const char* const key) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->cities[i].key, key) == 0)
            return &table->cities[i];
    }
    return NULL;
}

/// @brief Attempts to add a city to the table. If key is already in the table, it updates its values with new temp. Otherwise, it constructs a new city and appends.
/// @param table The table being added to.
/// @param key The city key to add.
/// @param temp The temp of the city to add.
/// @return Pointer to the existing or newly constructed city.
city* add(citytable* const table, char* const key, const float* const temp) {
    // Add temp if already in table
    city* found;
    if (found = search(table, key)) {
        tempstruct* data = found->tempData;
        data->sum += *temp;
        data->cnt += 1;
        if (*temp < data->min) data->min = *temp;
        else if (*temp > data->max) data->max = *temp;
        return found;
    }
    // Make new city and append if not in table
    else {
        city* newstart = realloc(table->cities, (table->size + 1) * table->citysizeof);
        tempstruct* tempData = malloc(table->tempsizeof);
        if (!newstart || !tempData) { free(newstart); free(tempData); return NULL; }

        table->size += 1;
        table->cities = newstart;
        table->endptr = newstart + table->size * table->citysizeof;

        tempData->sum = *temp;
        tempData->cnt = 1;
        tempData->min = *temp;
        tempData->max = *temp;

        city cityadd;
        cityadd.key = key;
        cityadd.tempData = tempData;

        *(table->endptr - 1) = cityadd;
        city* newadd = table->endptr - 1;

        // table->cities[table->size - 1] = cityadd;
        // city* newadd = &table->cities[table->size - 1];

        return newadd;
    }
}

void destroy(citytable* table) {
    free(table->cities);
    free(table);
}

int main(int argc, char *argv[]) {
    // if (argc < 2) { fprintf(stderr, "Err: Too few arguements.\n"); return -1; }
    // if (argc > 3) { fprintf(stderr, "Err: Too many arguments.\n"); return -1; }

    // const char* fileName = argv[1];
    // const char* ofileName = argv[2];

    const char* fileName = "test.txt";
    const char* ofileName = "output.txt";

    FILE* infile = fopen(fileName, "r");
    FILE* ofile = fopen(ofileName, "w");
    if (!infile || !ofile) { fprintf(stderr, "Err: File could not open. Check name and extension.\n"); return -1; }

    // FILE* infile = fopen(fileName, "r");
    // if (!infile) { fprintf(stderr, "Err: File could not open. Check name and extension.\n"); return -1; }

    citytable* table = createTable();
    if (!table) { fprintf(stderr, "Err: Problem allocating.\n"); return -1; }

    // Pointer math to parse the city name and temp by the semicolon
    int i = 0;
    char line[LINELENGTH];
    while (fgets(line, LINELENGTH, infile)) {
        char* delim = strpbrk(line, ";");
        if (!delim) { fprintf(stderr, "Err: No city found. Line: %d.\n", i); continue; }
        
        *delim = '\0';
        char* strTemp = delim + 1;

        // Convert and store
        float temp = strtof(strTemp, NULL);
        city* tmp = add(table, line, &temp);
        if (!tmp) { fprintf(stderr, "Err: Problem allocating memory while adding. Line: %d.\n", i); continue; }
        
        fprintf(ofile, "City: %s, Temp: %f, Sum: %f, Cnt: %d, Min: %f, Max: %f\n", tmp->key, temp, tmp->tempData->sum, tmp->tempData->cnt, tmp->tempData->min, tmp->tempData->max);

        i++;
    }

    fclose(infile);

    // Calculate and Output
    fclose(ofile);

    destroy(table);
}
