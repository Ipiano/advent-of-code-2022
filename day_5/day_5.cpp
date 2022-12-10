#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stack>
#include <stdexcept>

#include <gtest/gtest.h>

namespace std {
template<class T>
std::ostream& operator<<(std::ostream& o, const std::stack<T>& s)
{
    auto copy = s;

    o << "<";
    while (!copy.empty())
    {
        o << copy.top();
        copy.pop();
        if (!copy.empty())
            o << ", ";
    }
    return o << "<";
}
}  // namespace std

namespace day_5_impl {

using Stack = std::stack<char>;
using Stacks = std::array<Stack, 10>;  // Stacks[0] is empty so indicies align

struct Command
{
    size_t count;
    size_t index_from;
    size_t index_to;
};

bool operator==(const Command& l, const Command& r)
{
    return l.index_from == r.index_from && l.index_to == r.index_to && l.count == r.count;
}

// List of stack data is ordered bottom -> up
Stacks parse_stacks(const std::vector<std::string>& stack_data, const std::string& ids)
{
    Stacks result;

    for (size_t char_idx = 0; char_idx < ids.size(); ++char_idx)
    {
        if (ids[char_idx] != ' ')
        {
            const size_t stack_idx = ids[char_idx] - '0';

            for (const auto& row : stack_data)
            {
                if (row.size() >= char_idx && row[char_idx] != ' ')
                {
                    result[stack_idx].push(row[char_idx]);
                } else
                {
                    break;
                }
            }
        }
    }

    return result;
}

Command parse_command(const std::string& cmd_line)
{
    Command cmd;

    std::sscanf(
        cmd_line.c_str(), "move %zu from %zu to %zu", &cmd.count, &cmd.index_from, &cmd.index_to);

    return cmd;
}

// List of stacks will have an empty stack in index 0 so all numbers line up nice
std::pair<Stacks, std::vector<Command>> read_all_input(std::istream& input)
{
    Stacks stacks;
    std::vector<Command> commands;

    // Read lines until the empty line is reached
    std::string stack_line;
    std::vector<std::string> stack_data;
    while (std::getline(input, stack_line))
    {
        if (!stack_line.empty())
        {
            if (stack_line.find('1') != std::string::npos)
            {
                stacks = parse_stacks(stack_data, stack_line);
                std::getline(input, stack_line);
                break;
            } else
            {
                stack_data.insert(stack_data.begin(), stack_line);
            }
        }
    }

    // Read the commands
    std::string cmd_line;
    while (std::getline(input, cmd_line))
    {
        commands.push_back(parse_command(cmd_line));
    }

    return {std::move(stacks), std::move(commands)};
}

Stacks execute_plan(Stacks input, const std::vector<Command>& commands)
{
    for (const auto& cmd : commands)
    {
        for (size_t i = 0; i < cmd.count; ++i)
        {
            input[cmd.index_to].push(input[cmd.index_from].top());
            input[cmd.index_from].pop();
        }
    }
    return input;
}

TEST(Day5, ReadInput)
{
    const static char* INPUT_DATA = R"in(    [D]
[N] [C]
[Z] [M] [P]
 1   2   3

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    Stacks stacks;
    std::vector<Command> commands;
    std::tie(stacks, commands) = day_5_impl::read_all_input(ss_in);

    EXPECT_EQ(stacks[1], Stack({'Z', 'N'}));
    EXPECT_EQ(stacks[2], Stack({'M', 'C', 'D'}));
    EXPECT_EQ(stacks[3], Stack({'P'}));

    ASSERT_EQ(commands.size(), 4U);
    EXPECT_EQ(commands[0], Command({1, 2, 1}));
    EXPECT_EQ(commands[1], Command({3, 1, 3}));
    EXPECT_EQ(commands[2], Command({2, 2, 1}));
    EXPECT_EQ(commands[3], Command({1, 1, 2}));
}

}  // namespace day_5_impl

void day_5(std::istream& input, std::ostream& output)
{
    using namespace day_5_impl;

    const auto instructions = read_all_input(input);
    const auto result = execute_plan(instructions.first, instructions.second);

    for (const auto& s : result)
    {
        if (!s.empty())
        {
            output << s.top();
        }
    }
}

void day_5_adv(std::istream& input, std::ostream& output)
{
    using namespace day_5_impl;
}

TEST(Day5, Example)
{
    const static char* INPUT_DATA = R"in(    [D]
[N] [C]
[Z] [M] [P]
 1   2   3

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_5(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "CMZ");
}

TEST(Day5, ExampleAdvanced)
{
    const static char* INPUT_DATA = R"in(    [D]
[N] [C]
[Z] [M] [P]
 1   2   3

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    // day_5_adv(ss_in, ss_out);

    // EXPECT_EQ(ss_out.str(), "CMZ");
}
