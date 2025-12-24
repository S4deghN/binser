#include "data.h"

// pretty printer functions
#define STRUCT(type_name, ...) \
    void print_##type_name(type_name s, int space) { \
        printf("%*c:%*c"#type_name" {\n", 2, ' ', space, ' '); \
        __VA_ARGS__ \
        printf("%-2ld:%*c}\n", sizeof(type_name), space, ' '); \
    }
#define FIELD(type, field_name) _Generic(s.field_name, \
    int8_t:  printf("%-2ld:%*c"#type" "#field_name" = %d\n", offsetof(typeof(s), field_name), space+4, ' ', s.field_name), \
    uint8_t: printf("%-2ld:%*c"#type" "#field_name" = %d\n", offsetof(typeof(s), field_name), space+4, ' ', s.field_name), \
    int16_t: printf("%-2ld:%*c"#type" "#field_name" = %d\n", offsetof(typeof(s), field_name), space+4, ' ', s.field_name), \
    int32_t: printf("%-2ld:%*c"#type" "#field_name" = %d\n", offsetof(typeof(s), field_name), space+4, ' ', s.field_name), \
    int64_t: printf("%-2ld:%*c"#type" "#field_name" = %ld\n", offsetof(typeof(s), field_name), space+4, ' ', s.field_name), \
    float:   printf("%-2ld:%*c"#type" "#field_name" = %f\n", offsetof(typeof(s), field_name), space+4, ' ', s.field_name), \
    default: print_##type(s.field_name, space+4) );
STRUCTS
#undef STRUCT
#undef FIELD

// host to network converters
static inline void hton_8(void *a) {
    (void)a;
}
static inline void hton_16(void *a) {
    uint32_t *d = a;
    *d = bswap_16(*d);
}
static inline void hton_32(void *a) {
    uint32_t *d = a;
    *d = bswap_32(*d);
}
static inline void hton_64(void *a) {
    uint32_t *d = a;
    *d = bswap_64(*d);
}
#define STRUCT(type_name, ...) \
    void hton_##type_name(type_name* s) { \
        __VA_ARGS__ \
    }
#define FIELD(type, field_name) _Generic((s->field_name), \
    int8_t:   hton_8, \
    uint8_t:  hton_8, \
    int16_t:  hton_16, \
    uint16_t: hton_16, \
    int32_t:  hton_32, \
    uint32_t: hton_32, \
    int64_t:  hton_64, \
    uint64_t: hton_64, \
    float:    hton_32, \
    default:  hton_##type \
    )(&(s->field_name));
STRUCTS
#undef STRUCT
#undef FIELD
