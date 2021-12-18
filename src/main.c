#include "Enigma.h"

#include <stdio.h>
#include <string.h>

int main(int ac, const char *av[])
{
    (void) ac;
    (void) av;
    const char data[] = "ceci est un long message chiffre avec la technique de la machine enigma";
    char buffer[sizeof (data)] = {0};
    char buffer2[sizeof (data)] = {0};

    const char data1[] = "aaaa aaa aa aaaa aaaaaaa aaaaaaa aaaa aa aaaaaaaaa aa aa aaaaaaa aaaaaa";
    char buffer1[sizeof (data1)] = {0};
    char buffer12[sizeof (data1)] = {0};
    t_enigmaParameters engine = {};


    default_params(&engine);
    encode_data(&engine, data, buffer);
    default_params(&engine);
    encode_data(&engine, buffer, buffer2);

    printf("DATA = '%s'\nBUF1 = '%s'\nBUF2 = '%s'\n", data, buffer, buffer2);

    printf("RESULT : %s\n", (strcmp(data, buffer2) == 0) ? "OK" : "ERROR !");

    default_params(&engine);
    encode_data(&engine, data1, buffer1);
    default_params(&engine);
    encode_data(&engine, buffer1, buffer12);

    printf("DATA1 = '%s'\nBUF11 = '%s'\nBUF12 = '%s'\n", data1, buffer1, buffer12);

    printf("RESULT : %s\n", (strcmp(data1, buffer12) == 0) ? "OK" : "ERROR !");

    return 0;
}