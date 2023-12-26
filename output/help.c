#include <stdio.h>

void print_help() {
    printf(_(
        "pako",
        VERSION, "\n",
        "Usage: pako <action> [packages]\n",
        "  -i           install package\n",
        "  -l           list packages or specified packages in arguments\n",
        "  -r           remove packages\n",
        "  -h           print help message\n",
        "  -v           print version\n")
    );
}