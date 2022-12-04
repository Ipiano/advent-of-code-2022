#include <fstream>
#include <iostream>
#include <memory>
#include <type_traits>

using namespace std;

#ifndef PROCESS_FN
static_assert(false,
              "PROCESS_FN must be defined as an externed void(*)(std::istream&, std::ostream&)");
#endif

void PROCESS_FN(std::istream& input, std::ostream& output);

int main(int argc, char** argv)
{
    std::unique_ptr<std::ifstream> fin;
    std::unique_ptr<std::ofstream> fout;

    std::istream* in = &std::cin;
    std::ostream* out = &std::cout;

    for (int i = 0; i < argc; ++i)
    {
        std::string argi(argv[i]);
        if (argi == "-i")
        {
            ++i;
            fin = std::make_unique<std::ifstream>(argv[i]);
            in = fin.get();
        } else if (argi == "-o")
        {
            ++i;
            fout = std::make_unique<std::ofstream>(argv[i]);
            out = fout.get();
        }
    }

    PROCESS_FN(*in, *out);

    return 0;
}
