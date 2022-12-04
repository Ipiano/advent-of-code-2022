#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>

#include <gtest/gtest.h>

namespace day_1_impl {

std::vector<std::vector<uint64_t>> read_all_input(std::istream& input)
{
    std::vector<std::vector<uint64_t>> result;
    std::vector<uint64_t> this_group;

    std::string line;
    while (std::getline(input, line))
    {
        if (!line.empty())
        {
            this_group.emplace_back(std::stoull(line));
        } else
        {
            result.emplace_back(std::move(this_group));
            this_group.clear();
        }
    }

    if (!this_group.empty())
    {
        result.emplace_back(std::move(this_group));
    }

    return result;
}

uint64_t get_max_sum(const std::vector<std::vector<uint64_t>>& data)
{
    uint64_t max_sum = 0;
    std::for_each(data.begin(), data.end(), [&max_sum](const std::vector<uint64_t>& group) {
        max_sum = std::max<uint64_t>(max_sum, std::accumulate(group.begin(), group.end(), 0ULL));
    });
    return max_sum;
}
}  // namespace day_1_impl

void day_1(std::istream& input, std::ostream& output)
{
    std::vector<std::vector<uint64_t>> input_groups = day_1_impl::read_all_input(input);
    output << day_1_impl::get_max_sum(input_groups);
}

void day_1_adv(std::istream& input, std::ostream& output)
{
}

TEST(Day1, InputNumberGroups)
{
    std::stringstream ss;
    ss << "100\n200\n\n200\n300\n400\n\n500";

    auto number_groups = day_1_impl::read_all_input(ss);

    ASSERT_EQ(number_groups.size(), 3U);
    EXPECT_EQ(number_groups[0], std::vector<uint64_t>({100, 200}));
    EXPECT_EQ(number_groups[1], std::vector<uint64_t>({200, 300, 400}));
    EXPECT_EQ(number_groups[2], std::vector<uint64_t>({500}));
}

TEST(Day1, MaxGroupSum)
{
    ASSERT_EQ(day_1_impl::get_max_sum({{100, 200}, {200, 300, 400}, {500}}), 900);
}

TEST(Day1, Example)
{
    const static char* INPUT_DATA = R"in(1000
2000
3000

4000

5000
6000

7000
8000
9000

10000)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_1(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "24000");
}
