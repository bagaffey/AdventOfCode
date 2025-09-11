#include "app_platform.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "app_memory.h"
#include "app_intrinsics.h"
#include "app_math.h"
#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#endif

#ifdef _MSC_VER
typedef long ssize_t; // MSVC cl.exe doesn't provide ssize_t in C
#endif

// Trim trailing \r or \n
static char* TrimEOL(char* s) {
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r')) s[--n] = '\0';
    return s;
}

// Trim leading and trailing whitespace
static char* Trim(char* s) {
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

static int* GetMassesFromFile(const char* path, size_t* out_count) {
    FILE* f = xfopen_read(path);
    if (!f) return NULL;

    int* masses = NULL;
    size_t count = 0, cap = 0;

    char* line = NULL;
    size_t lcap = 0;

    while (getline_compat(&line, &lcap, f) != -1) {
        TrimEOL(line);
        char* s = Trim(line);
        if (*s == '\0') continue;

        char* endptr;
        long v = strtol(s, &endptr, 10);
        if (endptr == s || *endptr != '\0') {
            fprintf_s(stderr, "Invalid integer in input data file: '%s'\n", line);
            errno = EINVAL;
            free(masses);
            masses = NULL;
            break;
        }

        if (count == cap) {
            size_t newcap = cap ? cap * 2 : 16;
            int* newm = (int*)realloc(masses, newcap * sizeof(int));
            if (!newm) {
                free(masses);
                masses = NULL;
                break;
            }
            masses = newm;
            cap = newcap;
        }
        masses[count++] = (int)v;
    }

    free(line);
    fclose(f);

    if (masses) {
        *out_count = count;
    }
    return masses;
}

static int DownloadFromAOC(const char* cache_file) {
    char* session = NULL;
    size_t len = 0;
    errno_t dup_err = _dupenv_s(&session, &len, "AOC_SESSION");
    if (dup_err != 0 || session == NULL || *session == '\0') {
        fprintf_s(stderr, "AOC_SESSION environment variable is not set. Provide a file path or set AOC_SESSION.\n");
        free(session);
        return 1;
    }

    HINTERNET hInt = InternetOpenA("AOCFetcher", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInt) {
        fprintf_s(stderr, "InternetOpen failed: %lu\n", GetLastError());
        free(session);
        return 1;
    }

    HINTERNET hConn = InternetConnectA(hInt, "adventofcode.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConn) {
        fprintf_s(stderr, "InternetConnect failed: %lu\n", GetLastError());
        InternetCloseHandle(hInt);
        free(session);
        return 1;
    }

    HINTERNET hReq = HttpOpenRequestA(hConn, "GET", "/2019/day/1/input", NULL, NULL, NULL,
        INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hReq) {
        fprintf_s(stderr, "HttpOpenRequest failed: %lu\n", GetLastError());
        InternetCloseHandle(hConn);
        InternetCloseHandle(hInt);
        free(session);
        return 1;
    }

    size_t space_needed = strlen(session) + 32;
    char *header = (char*) malloc(space_needed);
    if (!header) { /* handle OOM */ }
    _snprintf_s(header, space_needed, _TRUNCATE, "Cookie: session=%s\r\n", session);
    if (!HttpAddRequestHeadersA(hReq, header, (DWORD) - 1L, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE)) {
        fprintf_s(stderr, "HttpAddRequestHeaders failed: %lu\n", GetLastError());
        InternetCloseHandle(hReq);
        InternetCloseHandle(hConn);
        InternetCloseHandle(hInt);
        free(session);
        free(header);
        return 1;
    }
    free(header);

    if (!HttpSendRequestA(hReq, NULL, 0, NULL, 0)) {
        fprintf_s(stderr, "HttpSendRequest failed: %lu\n", GetLastError());
        InternetCloseHandle(hReq);
        InternetCloseHandle(hConn);
        InternetCloseHandle(hInt);
        free(session);
        return 1;
    }

    DWORD status = 0;
    DWORD dlen = sizeof(status);
    if (!HttpQueryInfoA(hReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &status, &dlen, NULL)) {
        fprintf_s(stderr, "HttpQueryInfo failed: %lu\n", GetLastError());
        InternetCloseHandle(hReq);
        InternetCloseHandle(hConn);
        InternetCloseHandle(hInt);
        free(session);
        return 1;
    }

    if (status != 200) {
        fprintf_s(stderr, "Failed to fetch input (%lu). Body: ", status);
        char buf[4096];
        DWORD read;
        while (InternetReadFile(hReq, buf, sizeof(buf) - 1, &read) && read > 0) {
            buf[read] = '\0';
            fprintf_s(stderr, "%s", buf);
        }
        fprintf_s(stderr, "\n");
        InternetCloseHandle(hReq);
        InternetCloseHandle(hConn);
        InternetCloseHandle(hInt);
        free(session);
        return 1;
    }

    FILE* out = NULL;
    errno_t ec = fopen_s(&out, cache_file, "wb");
    if (ec != 0) {
        errno = ec;
        perror(cache_file);
        InternetCloseHandle(hReq);
        InternetCloseHandle(hConn);
        InternetCloseHandle(hInt);
        free(session);
        return 1;
    }

    char buf[4096];
    DWORD read;
    while (InternetReadFile(hReq, buf, sizeof(buf), &read) && read > 0) {
        fwrite(buf, 1, read, out);
    }

    fclose(out);
    InternetCloseHandle(hReq);
    InternetCloseHandle(hConn);
    InternetCloseHandle(hInt);
    free(session);
    return 0;
}

static void LoadEnvFile(const char* env_path) {
    FILE* f = xfopen_read(env_path);
    if (!f) return;

    char* line = NULL;
    size_t cap = 0;

    while (getline_compat(&line, &cap, f) != -1) {
        TrimEOL(line);
        char* s = Trim(line);
        if (*s == '\0' || *s == '#') continue;

        char* eq = strchr(s, '=');
        if (!eq) continue;

        *eq = '\0';
        char* key = Trim(s);
        char* val = Trim(eq + 1);

        // Strip surrounding quotes if present
        size_t vlen = strlen(val);
        if (vlen >= 2 && val[0] == '"' && val[vlen - 1] == '"') {
            val[vlen - 1] = '\0';
            val++;
        }

        _putenv_s(key, val);
    }

    free(line);
    fclose(f);
}

static int CalculateFuelRequirement(int module_mass) {
    return (module_mass / 3) - 2;
}

static int CalculateRealFuelRequirement(int module_mass) {
    int fuel = (module_mass / 3) - 2;
    if (fuel <= 0) {
        return (0);
    }
    else {
        return (fuel + CalculateRealFuelRequirement(fuel));
    }
}

int main(int argc, char** argv) {
    printf("__STDC_VERSION__ = %ld\n", (long)__STDC_VERSION__); // expect 202311 on C23

    const char* input_path = (argc >= 2) ? argv[1] : NULL;

    int* masses = NULL;
    size_t num_masses = 0;

    if (input_path && *input_path != '\0') {
        masses = GetMassesFromFile(input_path, &num_masses);
        if (!masses) {
            perror(input_path);
            return 1;
        }
    }
    else {
        LoadEnvFile("..\\..\\..\\..\\..\\..\\..\\.env");

        char temp_dir[MAX_PATH];
        GetTempPathA(MAX_PATH, temp_dir);

        char cache_file[MAX_PATH];
        _snprintf_s(cache_file, MAX_PATH, _TRUNCATE, "%s2019-12-01.dat", temp_dir);

        DWORD attr = GetFileAttributesA(cache_file);
        if (attr == INVALID_FILE_ATTRIBUTES) {
            if (DownloadFromAOC(cache_file) != 0) {
                return 1;
            }
        }

        masses = GetMassesFromFile(cache_file, &num_masses);
        if (!masses) {
            perror(cache_file);
            return 1;
        }
    }

    long long total_fuel = 0;
    long long total_real_fuel = 0;

    for (size_t i = 0; i < num_masses; i++) {
        total_fuel += CalculateFuelRequirement(masses[i]);
        total_real_fuel += CalculateRealFuelRequirement(masses[i]);
    }

    free(masses);

    printf("\n(Part 1) - Transport masses fuel required: %lld\n", total_fuel);
    printf("\n(Part 2) - Total Fuel for Trip Required: %lld\n", total_real_fuel);

    return 0;
}