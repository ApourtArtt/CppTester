#include <iostream>
#include <vector>
#include <functional>
#include <stdio.h>

struct registrar {
    inline static std::vector<std::function<void()>> funcs;
    registrar(std::function<void()> func) {
        funcs.push_back(func);
    }
};

#define TEST(func) \
    registrar r(func);

namespace Tester
{
    static void Run()
    {
        for (auto& fn: registrar::funcs)
            fn();
    }
}

int Bidule(int a, int b)
{
    return a + b;
}

TEST([]()
{
    if (Bidule(6, 7) != 12)
    printf("fail");
})

int main(int, char**)
{
    Tester::Run();
}
