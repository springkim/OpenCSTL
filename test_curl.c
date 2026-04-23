// fetch_popen.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define popen  _popen
#define pclose _pclose
#endif


char *http_get(const char *url) {
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "curl -sL --max-time 30 \"%s\"", url);
    FILE *fp = popen(cmd, "r");
    if (!fp) return NULL;

    size_t cap = 65536, len = 0;
    char *buf = (char *) malloc(cap);
    if (!buf) {
        pclose(fp);
        return NULL;
    }

    char tmp[4096];
    size_t n;
    while ((n = fread(tmp, 1, sizeof(tmp), fp)) > 0) {
        if (len + n + 1 > cap) {
            cap = (len + n + 1) * 2;
            char *p = (char *) realloc(buf, cap);
            if (!p) {
                free(buf);
                pclose(fp);
                return NULL;
            }
            buf = p;
        }
        memcpy(buf + len, tmp, n);
        len += n;
    }
    buf[len] = '\0';
    pclose(fp);
    return buf;
}

int main() {
    char *url = "https://github.com/springkim/OpenCSTL/wiki";

    char *html = http_get(url);

    for (char *p = html; *p; p++) {
        putchar(*p);
    }

    printf("%d\n", strlen(html));

    free(html);
    return 0;
}
