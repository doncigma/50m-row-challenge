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
    tempObj temps;
} city;

void addTemp(float temp, string cityName) {
    this = table[cityName]
    this.sum += temp
    this.cnt += 1
    temp < this.min ? this.min = temp : nothing
    temp > this.max ? this.max = temp : nothing
}

int main(int argc, char *argv[]) {
    city cities[] = {
        {"foo", 10}, {"bar", 42}, {"bazz", 36}, {"buzz", 7},
        {"bob", 11}, {"jane", 100}, {"x", 200}};
    
    size_t length = sizeof(cities) / sizeof(cities);

    
    
    return 0;
}