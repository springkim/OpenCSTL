#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"

int main() {
    FILE *fp = fstream.open("sample.json", "r");

    char *data = fstream.read(fp);

    fstream.close(fp);

    JSON *root = json.parse(data);

    printf("title         = %s\n", json.get(root, "title")->as_string());
    printf("2             = %d\n", json.get(root, "2")->as_int());
    printf("arrays.2      = %d\n", json.get(root, "arrays.2")->as_int());
    printf("nested.3      = %s\n", json.get(root, "nested.3")->as_bool() ? "true" : "false");
    printf("nested.9      = %f\n", json.get(root, "nested.9")->as_double());
    printf("nested.10     = %s\n", json.get(root, "nested.10")->is_null() ? "null" : "not null");
    json.delete(root);

    free(data);
    return 0;
}
