typedef struct {
    char* key;
    float val;
} pair;

typedef struct {
    char* key;
    pair* val;
} element;

int main(int argc, char *argv[]) {
    element table[] = {
        {"foo", 10}, {"bar", 42}, {"bazz", 36}, {"buzz", 7},
        {"bob", 11}, {"jane", 100}, {"x", 200}};
    size_t num_items = sizeof(table) / sizeof(table);
    
    return 0;
}