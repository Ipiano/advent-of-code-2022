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

using Command    = std::pair<Direction, size_t>;
using Coordinate = std::pair<int, int>;
using Rope       = std::vector<Coordinate>;

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

Coordinate step(Direction d, Coordinate start)
{
    // Move the head
    switch (d)
    {
    case Direction::UP:
        start.second++;
        break;
    case Direction::DOWN:
        start.second--;
        break;
    case Direction::LEFT:
        start.first--;
        break;
    case Direction::RIGHT:
        start.first++;
        break;
    }
    return start;
}

Coordinate update(Coordinate head, Coordinate tail)
{
    // If the tail is too far away, move it
    if (std::abs(head.first - tail.first) >= 2 || std::abs(head.second - tail.second) >= 2)
    {
        if (head.first != tail.first)
            tail.first += (head.first - tail.first) / (std::abs(head.first - tail.first));

        if (head.second != tail.second)
            tail.second += (head.second - tail.second) / std::abs(head.second - tail.second);
    }
    return tail;
}

size_t count_tail_locations(const std::vector<Command>& commands, size_t rope_length)
{
    Rope r(rope_length, {0, 0});

    std::set<Coordinate> tail_locations;
    tail_locations.insert(r.back());

    for (const auto& c : commands)
    {
        for (size_t i = 0; i < c.second; ++i)
        {
            r.front() = step(c.first, r.front());
            for (size_t j = 1; j < r.size(); ++j)
            {
                r[j] = update(r[j - 1], r[j]);
            }

            tail_locations.emplace(r.back());
        }
    }

    return tail_locations.size();
}
}

void day_9(std::istream& in, std::ostream& out)
{
    using namespace day_9_impl;

    const auto commands = read_input(in);

    out << count_tail_locations(commands, 2);
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
