#include <cstdlib>
#include "server.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    return run(argv[1]);
}
