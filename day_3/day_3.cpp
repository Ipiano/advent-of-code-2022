#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include <gtest/gtest.h>

namespace day_3_impl {

using Rucksack = std::pair<std::unordered_set<char>, std::unordered_set<char>>;

std::vector<Rucksack> read_all_input(std::istream& input)
{
    std::vector<Rucksack> result;

    std::string sack_items;
    while (std::getline(input, sack_items))
    {
        Rucksack sack;

        auto left = sack_items.begin();
        auto right = left + sack_items.size() / 2;
        for (; right != sack_items.end(); ++left, ++right)
        {
            sack.first.insert(*left);
            sack.second.insert(*right);
        }

        result.emplace_back(std::move(sack));
    }

    return result;
}

size_t get_priority(char c)
{
    return c <= 'Z' ? c - 'A' + 27 : c - 'a' + 1;
}

char get_mismatch(const Rucksack& sack)
{
    for (const auto& c : sack.first)
    {
        if (sack.second.count(c) > 0)
        {
            return c;
        }
    }
    throw std::runtime_error("input has no discprepancy");
}

char get_badge(const Rucksack& r1, const Rucksack& r2, const Rucksack& r3)
{
    std::unordered_map<char, uint8_t> item_counts;

    auto count_pocket = [&](const std::unordered_set<char>& p) {
        for (const auto c : p)
        {
            if (item_counts.count(c) == 0)
            {
                item_counts[c] = 0;
            }

            item_counts[c]++;
        }
    };

    auto count_items = [&](const Rucksack& r) {
        count_pocket(r.first);
        count_pocket(r.second);
    };

    count_items(r1);
    count_items(r2);
    count_items(r3);

    for (const auto& item_and_count : item_counts)
    {
        if (item_and_count.second == 3)
        {
            return item_and_count.first;
        }
    }
    throw std::runtime_error("no badge found");
}

TEST(Day3, ReadInput)
{
    std::stringstream ss;
    ss << "abcdefgh\nabcdef";

    const auto sack = read_all_input(ss);

    ASSERT_EQ(sack.size(), 2U);

    ASSERT_EQ(sack[0].first.size(), 4U);
    ASSERT_EQ(sack[0].second.size(), 4U);

    ASSERT_EQ(sack[1].first.size(), 3U);
    ASSERT_EQ(sack[1].second.size(), 3U);

    for (auto c : std::string("abcd"))
    {
        EXPECT_TRUE(sack[0].first.count(c) > 0);
    }

    for (auto c : std::string("efgh"))
    {
        EXPECT_TRUE(sack[0].second.count(c) > 0);
    }

    for (auto c : std::string("abc"))
    {
        EXPECT_TRUE(sack[1].first.count(c) > 0);
    }

    for (auto c : std::string("def"))
    {
        EXPECT_TRUE(sack[1].second.count(c) > 0);
    }
}

}  // namespace day_3_impl

void day_3(std::istream& input, std::ostream& output)
{
    auto sacks = day_3_impl::read_all_input(input);

    const auto mismatch_sum = std::accumulate(
        sacks.begin(), sacks.end(), 0ULL, [](uint64_t sum, const day_3_impl::Rucksack& sack) {
            return sum + day_3_impl::get_priority(day_3_impl::get_mismatch(sack));
        });

    output << mismatch_sum;
}

void day_3_adv(std::istream& input, std::ostream& output)
{
    auto sacks = day_3_impl::read_all_input(input);

    uint64_t badge_sum = 0;
    for (auto it = sacks.begin(); it != sacks.end(); it += 3)
    {
        badge_sum += day_3_impl::get_priority(day_3_impl::get_badge(*it, *(it + 1), *(it + 2)));
    }

    output << badge_sum;
}

TEST(Day3, Example)
{
    const static char* INPUT_DATA = R"in(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_3(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "157");
}

TEST(Day3, ExampleAdvanced)
{
    const static char* INPUT_DATA = R"in(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_3_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "70");
}
