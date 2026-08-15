#pragma once

#include <cstdio>
#include <string>
#include <optional>
#include <cstdint>

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
// Forward declaration
enum class AddressingMode;

struct NestestData {
    uint8_t pc_lo, pc_hi;
    uint8_t opcode;
    std::string name = "";
    std::optional<uint8_t> arg1 = std::nullopt;
    std::optional<uint8_t> arg2 = std::nullopt;
    AddressingMode mode;
    uint8_t reg_a, reg_x, reg_y, reg_p;
    uint8_t sp;
    int cycles;
    // Only used during special addressing modes
    uint8_t address_lo;
    uint8_t address_hi;
    uint8_t address_val;
};

void print_nestest_output(NestestData);

#endif
