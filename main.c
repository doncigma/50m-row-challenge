#include <stdio.h>
#include <stddef.h>
#include <string.h>

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
    city* next;
} city;

city* search(const city* cities, const size_t size, const char* key) {
    for (size_t i=0; i<size; i++) {
        if (strcmp(cities[i].key, key) == 0) {
            return &cities[i];
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    const char const* fileName = argv[1];
    
    city cities[1000];
    size_t size = sizeof(cities) / sizeof(cities);
    
    
    
    char cityName[] = "";
    float temp;

    FILE* file = fopen(fileName, "r");
    char line[40];
    
    while (fgets(line, sizeof(line), file)) {
        char currChar = line[0];
        char str[] = currChar;
        str + currChar;
        
        // We want to go until we see a semicolon and add along the way for the city name, store
        for (int i = 0; currChar != ";"; i++) {
            currChar = line[i];
        }
        // Then we want to go until the null termiantor for the temp, convert then store
        for (int i = 0; currChar != "\n"; i++) {
            currChar = line[i];
        }

        city* this;
        if (this = search(&cities, size, &cityName)) {
            tempObj thisData = this->tempData;
            
            thisData.sum += temp;
            thisData.cnt++;
            temp < thisData.min ? thisData.min = temp : thisData.min;
            temp > thisData.max ? thisData.max = temp : thisData.max;
        }
        continue;
    }

    return 0;
}