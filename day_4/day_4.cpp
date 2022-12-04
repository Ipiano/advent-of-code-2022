#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>

#include <gtest/gtest.h>

namespace day_4_impl {

using Range = std::pair<size_t, size_t>;
using Team = std::pair<Range, Range>;

std::vector<Team> read_all_input(std::istream& input)
{
    std::vector<Team> result;

    std::string team_line;
    while (std::getline(input, team_line))
    {
        Team t;

        std::sscanf(team_line.c_str(),
                    "%zu-%zu,%zu-%zu",
                    &t.first.first,
                    &t.first.second,
                    &t.second.first,
                    &t.second.second);

        result.push_back(t);
    }

    return result;
}

bool does_contain(Range l, Range r)
{
    return l.first >= r.first && l.second <= r.second;
}

TEST(Day4, ReadInput)
{
    std::stringstream ss;
    ss << "1-2,2-3\n10-11,99-103";

    const auto teams = day_4_impl::read_all_input(ss);

    ASSERT_EQ(teams.size(), 2U);
    EXPECT_EQ(teams[0], (Team{{1, 2}, {2, 3}}));
    EXPECT_EQ(teams[1], (Team{{10, 11}, {99, 103}}));
}

}  // namespace day_4_impl

void day_4(std::istream& input, std::ostream& output)
{
    using namespace day_4_impl;

    const auto teams = read_all_input(input);

    output << std::accumulate(teams.begin(), teams.end(), 0ULL, [](uint64_t sum, Team t) {
        return sum + ((does_contain(t.first, t.second) || does_contain(t.second, t.first)) ? 1 : 0);
    });
}

void day_4_adv(std::istream& input, std::ostream& output)
{
}

TEST(Day4, Example)
{
    const static char* INPUT_DATA = R"in(2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_4(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "2");
}

TEST(Day4, ExampleAdvanced)
{
    const static char* INPUT_DATA = R"in(2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    // day_4_adv(ss_in, ss_out);

    // EXPECT_EQ(ss_out.str(), "70");
}
