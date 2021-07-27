# CppTester

Header-only simple unit tester for C++.

## Why ?
I don't know if I am too stupid or what, but I found making unit testing in C++ was a pain, so I wrote this.

## How to use ?
There are two kinds of tests :
1) `TEST("What the test is related to", "What the test does", the_code)` : it tests the function based on what `the_code` is. There are two types of tests :
      - ASSERT("What the sub-test is about", the_statement) : if it passes, cool. If it failes, the test is stopped.
      - EXPECT("What the sub-test is about", the_statement) : if it passes, cool. If it failes, not cool. But it continues.
2) `BENCH("What the bench is related to", "What the bench... marks", the_code)` : it gives the number of iteration the code between `START_BENCH` and `STOP_BENCH ` does in a second.

## Example
Imagine we have an ultra complicated function named Bidule, such as it takes two inputs, `a` and `b`, adds them and return the value :
```cpp
int Bidule(int a, int b)
{
    return a + b;
}
```

### Test
It is for sure a function that we are going to refactor, etc, so we need to be future-proof. A good way of making sure we don't break thing is to implement some interesting unit test.

The syntax is pretty straightforward :
```cpp
TEST("BiduleTest", "Tests some additions",
    EXPECT("Is 6+7 equal to 12", Bidule(6, 7) == 12) // failed
    ASSERT("Is 6+6 equal to 12", Bidule(6, 6) == 12) // passed
    ASSERT("Is 6+7 equal to 12", Bidule(6, 7) == 12) // failed, will stop there
    EXPECT("Is 6+8 equal to 14", Bidule(6, 8) == 14) // ignored
)
```
The output we'll get is :
```
Starting tests...
Starting to test: BiduleTest - Tests some additions
        [EXPECT] Is 6+7 equal to 12: Failed (C:\Users\Moi\source\repos\Project2\Project2\main.cpp:13)
        [ASSERT] Is 6+6 equal to 12: Passed
        [ASSERT] Is 6+7 equal to 12: Failed (C:\Users\Moi\source\repos\Project2\Project2\main.cpp:13)
Tests executed : 3, Passed: 1, Failed: 2, Ignored: 1
Starting benchs...
Benchs executed
Every tasks are finished
```

### Benchmark
It can also be interesting to have an idea of the time a function takes, since we're talking about unit test and not integration tests, I chose to re-iterate again and again while one second did not passed.

Here is an example, still with Bidule :
```cpp
BENCH("BiduleBench", "Bench additions",
    int a = 5; int b = 7; // Don't count variable assignment in the bench
    START_BENCH // Chrono (and loop) starts right now
    Bidule(a, b); // Execute Bidule with `a` and `b`
    STOP_BENCH // loop stops if one second is passed
)
```

Here is the output :
```cpp
Starting tests...
Tests executed : 0, Passed: 0, Failed: 0, Ignored: 0
Starting benchs...
Starting to benchmark: BiduleBench - Bench additions... 4780033 iteration/s
Benchs executed
Every tasks are finished
```

## Final note
You will need to add `Tester::Run();` somewhere, to run the tests. I recommand doing a second project that refers to the first one, so you'll basically have two mains : one calling the project, one calling the tester.
