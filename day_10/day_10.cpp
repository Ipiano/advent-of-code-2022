#include <iostream>
#include <cmath>

#include <gtest/gtest.h>

namespace day_10_impl
{
enum class Op
{
    NOOP,
    ADD_X
};

struct Command
{
    Op op;
    int arg;
};

std::vector<Command> read_input(std::istream& in)
{
    std::vector<Command> result;

    std::string op_name;
    while (in >> op_name)
    {
        if (op_name == "noop")
        {
            result.push_back(Command {Op::NOOP, 0});
        }
        else if (op_name == "addx")
        {
            int arg;
            in >> arg;
            result.push_back(Command {Op::ADD_X, arg});
        }
    }

    return result;
}

std::vector<int> simulate(const std::vector<Command>& commands)
{
    // Cycles are 1-indexed; so push a 1.
    //
    // Each value represents the value at the end of a cycle, not the
    // start; so add another 1 representing the state at the end of the
    // first cycle (which will never) change the value.
    std::vector<int> result {1, 1};

    for (const auto& c : commands)
    {
        switch (c.op)
        {
        case Op::NOOP:
            result.push_back(result.back());
            break;
        case Op::ADD_X:
            result.push_back(result.back());
            result.push_back(result.back() + c.arg);
            break;
        }
    }

    return result;
}

std::vector<std::string> draw(const std::vector<int>& cycles)
{
    std::vector<std::string> result(6, std::string(40, '.'));

    size_t cycle = 1;
    for (std::string& row : result)
    {
        for (size_t col = 0; col < row.size(); ++col, ++cycle)
        {
            if (std::abs(cycles[cycle] - int(col)) <= 1)
            {
                row[col] = '#';
            }
        }
    }

    return result;
}

}

void day_10(std::istream& in, std::ostream& out)
{
    using namespace day_10_impl;

    const auto commands = read_input(in);
    const auto cycles   = simulate(commands);

    int64_t result = 0;
    for (const auto i : {20, 60, 100, 140, 180, 220})
    {
        result += cycles[i] * i;
    }

    out << result;
}

void day_10_adv(std::istream& in, std::ostream& out)
{
    using namespace day_10_impl;

    const auto commands = read_input(in);
    const auto cycles   = simulate(commands);
    const auto display  = draw(cycles);

    for (const auto& line : display)
    {
        out << line << "\n";
    }
}

TEST(Day10, Example)
{
    const static char* INPUT_DATA = R"in(addx 15
addx -11
addx 6
addx -3
addx 5
addx -1
addx -8
addx 13
addx 4
noop
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx -35
addx 1
addx 24
addx -19
addx 1
addx 16
addx -11
noop
noop
addx 21
addx -15
noop
noop
addx -3
addx 9
addx 1
addx -3
addx 8
addx 1
addx 5
noop
noop
noop
noop
noop
addx -36
noop
addx 1
addx 7
noop
noop
noop
addx 2
addx 6
noop
noop
noop
noop
noop
addx 1
noop
noop
addx 7
addx 1
noop
addx -13
addx 13
addx 7
noop
addx 1
addx -33
noop
noop
noop
addx 2
noop
noop
noop
addx 8
noop
addx -1
addx 2
addx 1
noop
addx 17
addx -9
addx 1
addx 1
addx -3
addx 11
noop
noop
addx 1
noop
addx 1
noop
noop
addx -13
addx -19
addx 1
addx 3
addx 26
addx -30
addx 12
addx -1
addx 3
addx 1
noop
noop
noop
addx -9
addx 18
addx 1
addx 2
noop
noop
addx 9
noop
noop
noop
addx -1
addx 2
addx -37
addx 1
addx 3
noop
addx 15
addx -21
addx 22
addx -6
addx 1
noop
addx 2
addx 1
noop
addx -10
noop
noop
addx 20
addx 1
addx 2
addx 2
addx -6
addx -11
noop
noop
noop)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_10(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "13140");
}

TEST(Day10, ExampleAdvanced)
{
    const static char* INPUT_DATA = R"in(addx 15
addx -11
addx 6
addx -3
addx 5
addx -1
addx -8
addx 13
addx 4
noop
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx -35
addx 1
addx 24
addx -19
addx 1
addx 16
addx -11
noop
noop
addx 21
addx -15
noop
noop
addx -3
addx 9
addx 1
addx -3
addx 8
addx 1
addx 5
noop
noop
noop
noop
noop
addx -36
noop
addx 1
addx 7
noop
noop
noop
addx 2
addx 6
noop
noop
noop
noop
noop
addx 1
noop
noop
addx 7
addx 1
noop
addx -13
addx 13
addx 7
noop
addx 1
addx -33
noop
noop
noop
addx 2
noop
noop
noop
addx 8
noop
addx -1
addx 2
addx 1
noop
addx 17
addx -9
addx 1
addx 1
addx -3
addx 11
noop
noop
addx 1
noop
addx 1
noop
noop
addx -13
addx -19
addx 1
addx 3
addx 26
addx -30
addx 12
addx -1
addx 3
addx 1
noop
noop
noop
addx -9
addx 18
addx 1
addx 2
noop
noop
addx 9
noop
noop
noop
addx -1
addx 2
addx -37
addx 1
addx 3
noop
addx 15
addx -21
addx 22
addx -6
addx 1
noop
addx 2
addx 1
noop
addx -10
noop
noop
addx 20
addx 1
addx 2
addx 2
addx -6
addx -11
noop
noop
noop)in";

    const static char* OUTPUT_DATA = R"in(##..##..##..##..##..##..##..##..##..##..
###...###...###...###...###...###...###.
####....####....####....####....####....
#####.....#####.....#####.....#####.....
######......######......######......####
#######.......#######.......#######.....
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_10_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), OUTPUT_DATA);
}
