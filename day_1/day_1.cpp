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

std::vector<uint64_t> sum_groups(const std::vector<std::vector<uint64_t>>& data)
{
    std::vector<uint64_t> result(data.size(), 0);

    std::transform(
        data.begin(), data.end(), result.begin(), [](const std::vector<uint64_t>& group) {
            return std::accumulate(group.begin(), group.end(), 0ULL);
        });

    return result;
}

uint64_t get_max_sum(const std::vector<std::vector<uint64_t>>& data)
{
    auto summed_groups = sum_groups(data);
    std::sort(summed_groups.rbegin(), summed_groups.rend());
    return summed_groups.front();
}
}  // namespace day_1_impl

void day_1(std::istream& input, std::ostream& output)
{
    auto input_groups = day_1_impl::read_all_input(input);
    output << day_1_impl::get_max_sum(input_groups);
}

void day_1_adv(std::istream& input, std::ostream& output)
{
    auto input_groups = day_1_impl::read_all_input(input);
    auto summed_groups = day_1_impl::sum_groups(input_groups);
    std::sort(summed_groups.rbegin(), summed_groups.rend());

    output << summed_groups[0] + summed_groups[1] + summed_groups[2];
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

TEST(Day1, SumGroups)
{
    ASSERT_EQ(day_1_impl::sum_groups({{100, 200}, {200, 300, 400}, {500}}),
              std::vector<uint64_t>({300, 900, 500}));
}

TEST(Day1, ExampleRegular)
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

TEST(Day1, ExampleAdvanced)
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

    day_1_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "45000");
}
