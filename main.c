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
city* search(city* const cities, const size_t size, const char* const searchKey) {
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

// PSD: potential slowdown

int main(int argc, char *argv[]) {
    int nameLength = 101;
    int lineLength = 106;
    int citiesLength = 1001;
    int strTempLength = 6;
    
    city cities[citiesLength];
    size_t size = sizeof(cities) / sizeof(city);
    city* endptr = cities + size;

    // char cityName[nameLength];
    // char strTemp[strTempLength];
    char line[lineLength];

    const char const* fileName = argv[1];
    FILE* file = fopen(fileName, "r");

    while (fgets(line, lineLength, file)) {
        // Pointer math to parse the city name and temp by the semicolon
        char* delim = strpbrk(line, ";");
        if (delim) {
            char* cityName = line;
            *delim = '\0';
            char* strTemp = delim + 1;
            printf("City: %s\n", cityName);
            printf("Temp: %s\n", strTemp);
        }
        else {
            fprintf(stderr, "Error: No city found");
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
}
