#include <cstdio>
#include "ComputerState.hpp"

int main() {
    ComputerState comp(200);
    comp.step(10);
    printf("Pass!\n");
    return 0;
}
