#include <iostream>
#include <cassert>
#include <set>

#include <gtest/gtest.h>

namespace day_9_impl
{
enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

using Command = std::pair<Direction, size_t>;

std::vector<Command> read_input(std::istream& in)
{
    std::vector<Command> result;

    char c;
    size_t dist;
    while (in >> c >> dist)
    {
        Direction d = [](char c)
        {
            switch (c)
            {
            case 'R':
                return Direction::RIGHT;
            case 'L':
                return Direction::LEFT;
            case 'U':
                return Direction::UP;
            case 'D':
                return Direction::DOWN;
            default:
                assert(false);
            }
        }(c);

        result.emplace_back(d, dist);
    }

    return result;
}

void step(Direction d, int& head_x, int& head_y)
{
    // Move the head
    switch (d)
    {
    case Direction::UP:
        head_y++;
        break;
    case Direction::DOWN:
        head_y--;
        break;
    case Direction::LEFT:
        head_x--;
        break;
    case Direction::RIGHT:
        head_x++;
        break;
    }
}

void update(int head_x, int head_y, int& tail_x, int& tail_y)
{
    // If the tail is too far away, move it
    if (std::abs(head_x - tail_x) >= 2 || std::abs(head_y - tail_y) >= 2)
    {
        if (head_x != tail_x)
            tail_x += (head_x - tail_x) / (std::abs(head_x - tail_x));

        if (head_y != tail_y)
            tail_y += (head_y - tail_y) / std::abs(head_y - tail_y);
    }
}

size_t count_tail_locations(const std::vector<Command>& commands)
{
    std::set<std::pair<int, int>> tail_locations;

    int head_x = 0, head_y = 0;
    int tail_x = 0, tail_y = 0;

    tail_locations.emplace(tail_x, tail_y);

    for (const auto& c : commands)
    {
        for (size_t i = 0; i < c.second; ++i)
        {
            step(c.first, head_x, head_y);
            update(head_x, head_y, tail_x, tail_y);

            tail_locations.emplace(tail_x, tail_y);
        }
    }

    return tail_locations.size();
}
}

void day_9(std::istream& in, std::ostream& out)
{
    using namespace day_9_impl;

    const auto commands = read_input(in);

    out << count_tail_locations(commands);
}

void day_9_adv(std::istream& in, std::ostream& out)
{
    using namespace day_9_impl;
}

TEST(Day9, Example)
{
    const static char* INPUT_DATA = R"in(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_9(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "13");
}

TEST(DISABLED_Day9, ExampleAdvanced1)
{
    const static char* INPUT_DATA = R"in(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_9_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "1");
}

TEST(DISABLED_Day9, ExampleAdvanced2)
{
    const static char* INPUT_DATA = R"in(R 5
U 8
L 8
D 3
R 17
D 10
L 25
U 20
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_9_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "36");
}
