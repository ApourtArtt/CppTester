#include "Tester.h"

int Bidule(int a, int b)
{
    return a + b;
}

TEST("BiduleTest", "Tests some additions",
    EXPECT("Is 6+7 equal to 12", Bidule(6, 7) == 12) // failed
    ASSERT("Is 6+6 equal to 12", Bidule(6, 6) == 12) // passed
    ASSERT("Is 6+7 equal to 12", Bidule(6, 7) == 12) // failed, will stop there
    EXPECT("Is 6+8 equal to 14", Bidule(6, 8) == 14) // ignored
)

BENCH("BiduleBench", "Bench additions",
    int a = 5; int b = 7; // Don't count the variable assignment in the benchmark
    START_BENCH
    Bidule(a, b); // Only count Bidule(a, b);
    STOP_BENCH
)

int main(int, char**)
{
    return Tester::Run();
}
