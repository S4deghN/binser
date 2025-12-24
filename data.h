#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <endian.h>
#include <arpa/inet.h>
#include <byteswap.h>

// NOTE: because we use name concatination to construct functions, type of
// variables must be denoted as a single word (i.e., type names like `long int`
// cause errors)

#define STRUCTS \
    STRUCT(extra, \
        FIELD(int16_t, f) \
    ) \
    STRUCT(header, \
        FIELD(extra, x) \
        FIELD(int, type) \
        FIELD(int, size) \
    ) \
    STRUCT(message, \
        FIELD(header, h) \
        FIELD(int, x) \
        FIELD(float, y) \
        FIELD(uint8_t, z) \
    )

// struct defenitions
#define STRUCT(type, ...) \
    typedef struct type { \
        __VA_ARGS__ \
    } __attribute__((aligned(4))) type;
#define FIELD(type, name) type name;
STRUCTS
#undef STRUCT
#undef FIELD

#ifdef __cplusplus
extern "C" {
#endif

// pretty printer functions
#define STRUCT(type_name, ...) void print_##type_name(type_name, int); __VA_ARGS__
#define FIELD(type, field_name) void print_##type(type, int);
STRUCTS
#undef STRUCT
#undef FIELD
#define pretty_print(type, var) print_##type(var, 0)

// host to network converters
#define STRUCT(type_name, ...) void hton_##type_name(type_name*); __VA_ARGS__
#define FIELD(type, field_name) void hton_##type(type*);
STRUCTS
#undef STRUCT
#undef FIELD

#ifdef __cplusplus
} // extern "C"
#endif

#endif
