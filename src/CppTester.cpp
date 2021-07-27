#include <iostream>
#include <vector>
#include <functional>
#include <stdio.h>
#include <tuple>
#include <source_location>

struct ret
{
    std::vector<std::tuple<std::string, bool, std::string>> statements; // subtitle, status, location
    void AddStatement(std::string Title, bool Status, std::string location)
        { statements.push_back({Title, Status, location}); }
};

struct registrar {
    struct entity {
        std::string title, subTitle;
        std::function<ret()> func;
        entity(std::string Title, std::string SubTitle, std::function<ret()> Func)
            : title(std::move(Title)), subTitle(std::move(SubTitle)), func(Func)
        {}
    };

    inline static std::vector<entity> testers;
    registrar(std::string Title, std::string SubTitle, std::function<ret()> Func, bool IsTest) {
        testers.push_back(entity(std::move(Title), std::move(SubTitle), Func));
    }
};

#define TESTER(name,message,func) registrar r(name, message, []()->ret{ret r; func return r;}, true);
#define TEST(...) TESTER(__VA_ARGS__)

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

        for (auto& fn: registrar::testers)
        {
            std::cout << "Starting to test: " << fn.title << " - " << fn.subTitle << std::endl;
            ret r = fn.func();
            for (auto& test: r.statements)
            {
                auto [ subTitle, status, location ] = test;
                if (status)
                    std::cout << "\t" << subTitle << " : passed" << std::endl;
                else
                    std::cout << "\t" << subTitle << " : failed (" << location << ")" << std::endl;
            }
        }
    }
}

int Bidule(int a, int b)
{
    return a + b;
}

TEST("BiduleTest", "Tests some additions",
    EXPECT("Is 6+7 equal to 12", Bidule(6, 7)==12) // false
    ASSERT("Is 6+6 equal to 12", Bidule(6, 6)==12) // true
    ASSERT("Is 6+7 equal to 12", Bidule(6, 7)==12) // false, will stop there
    EXPECT("Is 6+8 equal to 14", Bidule(6, 8)==14) // true
)

int main(int, char**)
{
    Tester::Run();
}
