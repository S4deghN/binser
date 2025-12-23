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
        FIELD(int32_t, f) \
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
    } __attribute__((packed)) type;
#define FIELD(type, name) type name;
STRUCTS
#undef STRUCT
#undef FIELD

// pretty printer functions
#define STRUCT(type_name, ...) __VA_ARGS__
#define FIELD(type, field_name) void print_##type(type, int);
STRUCTS
#undef STRUCT
#undef FIELD
#define STRUCT(type_name, ...) \
    void print_##type_name(type_name s, int space) { \
        printf("%*c"#type_name" {\n", space, ' '); \
        __VA_ARGS__ \
        printf("%*c}\n", space, ' '); \
    }
#define FIELD(type, field_name) _Generic(s.field_name, \
    uint8_t: printf("%*c"#field_name" = %d\n", space+4, ' ', s.field_name), \
    int32_t: printf("%*c"#field_name" = %d\n", space+4, ' ', s.field_name), \
    float: printf("%*c"#field_name" = %f\n", space+4, ' ', s.field_name), \
    default: print_##type(s.field_name, space+4) );
STRUCTS
#undef STRUCT
#undef FIELD
#define pretty_print(type, var) print_##type(var, 0)

// host to network converters
void hton_8(void *a) {
    return;
}
void hton_16(void *a) {
    uint32_t *d = a;
    *d = bswap_16(*d);
}
void hton_32(void *a) {
    uint32_t *d = a;
    *d = bswap_32(*d);
}
void hton_64(void *a) {
    uint32_t *d = a;
    *d = bswap_64(*d);
}
#define STRUCT(type_name, ...) __VA_ARGS__
#define FIELD(type, field_name) void hton_##type(type*);
STRUCTS
#undef STRUCT
#undef FIELD
#define STRUCT(type_name, ...) \
    void hton_##type_name(type_name* s) { \
        __VA_ARGS__ \
    }
#define FIELD(type, field_name) _Generic((s->field_name), \
    uint8_t:  hton_8, \
    int8_t:   hton_8, \
    uint16_t: hton_16, \
    int32_t:  hton_32, \
    uint32_t: hton_32, \
    float:    hton_32, \
    default:  hton_##type \
    )(&(s->field_name));
STRUCTS
#undef STRUCT
#undef FIELD

int
main()
{
    int x = sizeof(message);
    message m;
    m.h.x.f = 1234;
    m.h.type = 0;
    m.h.size = 10;
    m.x = 50;
    m.y = 3.14;
    m.z = 23;

    pretty_print(message, m);

    hton_message(&m);
    pretty_print(message, m);

    hton_message(&m);
    pretty_print(message, m);
}
