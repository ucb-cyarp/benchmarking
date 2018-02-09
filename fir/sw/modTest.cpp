#include <cstdint>
#include <cstdlib>
#include <cstdio>

int main(int argc, char* argv[])
{
    uint64_t ucounter = 5;
    for(int i = 0; i<20; i++)
    {
        printf("Counter Unsigned: %3d\n", ucounter);

        ucounter = (ucounter-1)%10;
    }

    int64_t scounter = 5;
    for(int i = 0; i<20; i++)
    {
        printf("Counter: %3d\n", scounter);

        scounter = (scounter-1)%10;
    }

}
