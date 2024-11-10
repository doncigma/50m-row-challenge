#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* key;
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

// SLOWDOWN: Need to improve this to O(1) lookup with a custom hash function
city* search(city* const cities, const size_t size, const char* searchKey) {
    for (size_t i = 0; i < size; i++) {
        if (strcmp(cities[i].key, searchKey) == 0) {
            return &cities[i];
        }
    }
    return NULL;
}

city* add(city* const cities, size_t* const size, city* const endptr, const city* const cityToAdd, const size_t citySize) {
    // probably memset some sizeof(city) at the endptr and initliaze to cityToAdd
    *size += 1;

    city* newLoc = realloc(cities, citySize);
    memcpy(endptr, cityToAdd, (*size) * citySize);

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

int main(int argc, char *argv[]) {
    city cities[1000];
    size_t size = sizeof(cities) / sizeof(city);
    city* endptr = cities + size;

    int lineLength = 106; // max chars in name + temp
    char line[lineLength];

    const char const* fileName = argv[1];
    FILE* infile = fopen(fileName, "r");
    const char* ofileName = argv[2];
    FILE* ofile = fopen(ofileName, "w");
    if (!infile && !ofile) {
        fprintf(stderr, "Error: File could not open. Check name and extension.");
        return -1;
    }

    // Pointer math to parse the city name and temp by the semicolon
    while (fgets(line, lineLength, infile)) {
        char* delim = strpbrk(line, ";");
        if (delim) {
            char* cityName = line;
            *delim = '\0';
            char* strTemp = delim + 1;

            // Convert and store
            fprintf(ofile, "%s;%s", cityName, strTemp);
        }
        else {
            fprintf(stderr, "Error: No city found. Check input file formatting.");
            continue;
        }
    
        // // Convert and store
        // float temp = strtof(strTemp, NULL);
        // city* this = search(cities, size, cityName);
        // if (this) {
        //     tempObj data = this->tempData;
            
        //     data.sum += temp;
        //     data.cnt += 1;
        //     temp < data.min ? data.min = temp : data.min;
        //     temp > data.max ? data.max = temp : data.max;
        // }
    }

    fclose(infile);

    // Calculate and Output
    
    
    fclose(ofile);
}
