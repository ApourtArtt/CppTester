#include <iostream>
#include <vector>
#include <functional>
#include <tuple>
#include <source_location>
#include <chrono>

struct testRet
{
    std::vector<std::tuple<std::string, bool, std::string>> statements;
    void AddStatement(std::string Title, bool Status, std::string location)
        { statements.push_back({std::move(Title), std::move(Status), std::move(location)}); }
};

struct benchRet
{
    int counter;
};

struct registrar {
    struct testEntity {
        std::string title, subTitle;
        std::function<testRet()> func;
        testEntity(std::string Title, std::string SubTitle, std::function<testRet()> Func)
            : title(std::move(Title)), subTitle(std::move(SubTitle)), func(Func)
        {}
    };
    struct benchEntity {
        std::string title, subTitle;
        std::function<benchRet()> func;
        benchEntity(std::string Title, std::string SubTitle, std::function<benchRet()> Func)
            : title(std::move(Title)), subTitle(std::move(SubTitle)), func(Func)
        {}
    };

    inline static std::vector<testEntity> testers;
    registrar(std::string Title, std::string SubTitle, std::function<testRet()> Func) {
        testers.push_back(testEntity(std::move(Title), std::move(SubTitle), Func));
    }
    inline static std::vector<benchEntity> benchers;
    registrar(std::string Title, std::string SubTitle, std::function<benchRet()> Func) {
        benchers.push_back(benchEntity(std::move(Title), std::move(SubTitle), Func));
    }
};

#define TESTER(name,message,func) registrar testR(name, message, []()->testRet{testRet r; func return r;});
#define TEST(...) TESTER(__VA_ARGS__)

#define BENCHER(name, message, func) registrar benchR(name, message, []()->benchRet{benchRet r; func return r;});
#define BENCH(...) BENCHER(__VA_ARGS__)

#define START_BENCH \
    auto t1 = std::chrono::high_resolution_clock::now(); \
    auto t2 = std::chrono::high_resolution_clock::now(); \
    while (duration_cast<std::chrono::milliseconds>(t2-t1).count() < 1000) {

#define STOP_BENCH \
        t2 = std::chrono::high_resolution_clock::now(); \
        r.counter++; \
    }

#define GET_LOCATION __FILE__ + std::string(":") + std::to_string(__LINE__)

#define EXPECTER(title, statement) r.AddStatement(std::string("[EXPECT] ")+title, statement, GET_LOCATION);
#define EXPECT(...) EXPECTER(__VA_ARGS__)

#define ASSERTER(title, statement) r.AddStatement(std::string("[ASSERT] ")+title, statement, GET_LOCATION); if (statement == false) return r;
#define ASSERT(...) ASSERTER(__VA_ARGS__)

namespace Tester
{
    static void Run()
    {
        static bool hasRan = false;
        if (hasRan) return;
        hasRan = true;

        int nbTestFailed = 0, nbTestPassed = 0;

        std::cout << "Starting tests..." << std::endl;

        for (auto& fn: registrar::testers)
        {
            std::cout << "Starting to test: " << fn.title << " - " << fn.subTitle << std::endl;
            testRet r = fn.func();
            for (auto& test: r.statements)
            {
                auto [subTitle, status, location] = test;
                if (status)
                {
                    std::cout << "\t" << subTitle << ": Passed" << std::endl;
                    nbTestPassed++;
                }
                else
                {
                    std::cout << "\t" << subTitle << ": Failed (" << location << ")" << std::endl;
                    nbTestFailed++;
                }
            }
        }

        std::cout << "Tests executed : " << nbTestFailed + nbTestPassed
        << ", Passed: " << nbTestPassed << ", Failed: " << nbTestFailed
        << ", Ignored: " << registrar::testers.size() << std::endl; // TODO: it works (that's strange) on GCC, check for MSVC
        
        std::cout << "Starting benchs..." << std::endl;

        for (auto& fn: registrar::benchers)
        {
            std::cout << "Starting to benchmark: " << fn.title << " - " << fn.subTitle << "...\t";
            benchRet r = fn.func();
            std::cout << r.counter << " iteration/s" << std::endl;
        }

        std::cout << "Benchs executed\nEvery tasks are finished" << std::endl;
    }
}

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
    int a = 5; int b = 7;
    START_BENCH
    Bidule(a, b);
    STOP_BENCH
)

int main(int, char**)
{
    Tester::Run();
}
