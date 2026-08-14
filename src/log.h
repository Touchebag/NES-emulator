#pragma once

#include <cstdio>
#include <string>

/*
 * LOG_LEVEL
 * Every level includes all previous levels
 *
 * 0 - Nothing
 * 1 - Error
 * 2 - Warning
 * 3 - Debug
 * 4 - Verbose
 */

// ---- Main log ----

#if LOG_LEVEL > 0
#define LOG(level, ...) fprintf(stderr, level " : " __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);
#else
#define LOG(...) ((void)0);
#endif

// ---- Levels ----

#if LOG_LEVEL > 0
#define LOGE(...) LOG("ERROR", __VA_ARGS__)
#else
#define LOGE(...) ((void)0);
#endif

#if LOG_LEVEL > 1
#define LOGW(...) LOG("WARNING", __VA_ARGS__)
#else
#define LOGW(...) ((void)0);
#endif

#if LOG_LEVEL > 2
#define LOGD(...) LOG("DEBUG", __VA_ARGS__)
#else
#define LOGD(...) ((void)0);
#endif

#if LOG_LEVEL > 3
#define LOGV(...) LOG("VERBOSE", __VA_ARGS__)
#else
#define LOGV(...) ((void)0);
#endif

// ---- Verbose instrcution output ----
#if LOG_LEVEL > 3
#define PRINT_INSTRUCTION(...) fprintf(stderr, "OP : " __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);
#else
#define PRINT_INSTRUCTION(...) ((void)0);
#endif

#ifdef NESTEST_OUTPUT
struct NestestData {
    uint8_t pc_lo, pc_hi;
    uint8_t opcode;
    std::string name = "";
    std::optional<uint8_t> arg1 = std::nullopt;
    std::optional<uint8_t> arg2 = std::nullopt;
};

inline void print_nestest_output(NestestData data) {
    fprintf(stderr, "%02X%02X  ", data.pc_hi, data.pc_lo);
    fprintf(stderr, "%02X ", data.opcode);

    // Optional arguments
    if (data.arg1) {
        fprintf(stderr, "%02X ", data.arg1.value());

        if (data.arg2) {
            fprintf(stderr, "%02X ", data.arg2.value());
        } else {
            fprintf(stderr, "  ");
        }
    } else {
        fprintf(stderr, "  ");
    }
    fprintf(stderr, "  ");

    fprintf(stderr, "%s", data.name.c_str());

    fprintf(stderr, "\n");
    fflush(stderr);
}
#endif
