#include "data.h"

int
main()
{
    message m;
    m.h.x.f = 1234;
    m.h.type = 0;
    m.h.size = 10;
    m.x = 50;
    m.y = 3.14;
    m.z = 23;

    pretty_print(message, m);
    printf("-------------------------\n");
    hton_message(&m);
    pretty_print(message, m);
    printf("-------------------------\n");
    hton_message(&m);
    pretty_print(message, m);
}
