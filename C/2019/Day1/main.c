#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <wininet.h>

#ifdef _MSC_VER
typedef long ssize_t; // MSVC cl.exe doesn't provide ssize_t in C
#endif

// Trim trailing \r or \n
static char* trim_eol(char* s) {
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r')) s[--n] = '\0';
    return s;
}

// Trim leading and trailing whitespace
static char* trim(char* s) {
    while (isspace((unsigned char)*s)) s++;
    char* e = s + strlen(s) - 1;
    while (e >= s && isspace((unsigned char)*e)) *e-- = '\0';
    return s;
}

// getline replacement that works on MSVCRT
static ssize_t getline_compat(char** lineptr, size_t* n, FILE* stream) {
    if (!lineptr || !n || !stream) { errno = EINVAL; return -1; }
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = (char*)malloc(*n);
        if (!*lineptr) { errno = ENOMEM; return -1; }
    }
    size_t len = 0;
    int c;
    while ((c = fgetc(stream)) != EOF) {
        if (len + 1 >= *n) {
            size_t newn = (*n < 64 ? 128 : *n * 2);
            char* p = (char*)realloc(*lineptr, newn);
            if (!p) { errno = ENOMEM; return -1; }
            *lineptr = p; *n = newn;
        }
        (*lineptr)[len++] = (char)c;
        if (c == '\n') break;
    }
    if (len == 0 && c == EOF) return -1;
    (*lineptr)[len] = '\0';
    return (ssize_t)len;
}

// "open for read" fopen_s on Windows, fopen elsewhere
static FILE* xfopen_read(const char* path) {
#ifdef _WIN32
    FILE* f = NULL;
    errno_t ec = fopen_s(&f, path, "rb");
    if (ec != 0) { errno = ec; return NULL; }
    return f;
#else
    return fopen(path, "rb");
#endif
}

int main(int argc, char** argv) {
    
    printf("__STDC_VERSION__ = %ld\n", (long)__STDC_VERSION__);  // expect 202311 on C23

    const char* path = (argc >= 2) ? argv[1] : "input.txt";
    FILE* f = xfopen_read(path);
    if (!f) { perror(path); return 1; }

    char* line = NULL; size_t cap = 0;
    long long part1 = 0, part2 = 0;

    while (getline_compat(&line, &cap, f) != -1) {
        trim_eol(line);
        // TODO: parse & solve
        (void)line;
    }

    free(line);
    fclose(f);

    printf("Part 1: %lld\n", part1);
    printf("Part 2: %lld\n", part2);
    return 0;
}
