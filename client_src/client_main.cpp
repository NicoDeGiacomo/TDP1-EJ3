#include <cstdlib>
#include "client.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return EXIT_FAILURE;
    }

    return run(argv[1], argv[2]);
}
