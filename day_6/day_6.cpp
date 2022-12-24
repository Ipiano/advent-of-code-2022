#include <iostream>

#include <gtest/gtest.h>

namespace day_6_impl
{
template <class IteratorT> bool unique(IteratorT begin, IteratorT end)
{
    std::array<size_t, 256> counts = {{0}};
    while (begin != end)
    {
        if (++counts[*begin] > 1)
        {
            return false;
        }
        ++begin;
    }
    return true;
}

template <class IteratorT> size_t find_token(IteratorT begin, IteratorT end, size_t token_size)
{
    auto curr = begin;
    while (curr + token_size < end)
    {
        if (day_6_impl::unique(curr, curr + token_size))
        {
            return curr + token_size - begin;
        }
        ++curr;
    }
    return static_cast<size_t>(-1);
}
}

void day_6(std::istream& input, std::ostream& output)
{
    using namespace day_6_impl;

    std::string line;
    std::getline(input, line);
    output << find_token(line.begin(), line.end(), 4);
}

void day_6_adv(std::istream& input, std::ostream& output)
{
    using namespace day_6_impl;

    std::string line;
    std::getline(input, line);
    output << find_token(line.begin(), line.end(), 14);
}

TEST(Day6, Examples)
{
    using namespace day_6_impl;

    const std::vector<std::pair<std::string, size_t>> CASES {{"mjqjpqmgbljsphdztnvjfqwrcgsmlb", 7},
                                                             {"bvwbjplbgvbhsrlpgdmjqwftvncz", 5},
                                                             {"nppdvjthqldpwncqszvftbrmjlhg", 6},
                                                             {"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10},
                                                             {"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11}};

    for (const auto& test : CASES)
    {
        EXPECT_EQ(find_token(test.first.begin(), test.first.end(), 4), test.second);
    }
}

TEST(Day6, ExamplesAdvanced)
{
    using namespace day_6_impl;

    const std::vector<std::pair<std::string, size_t>> CASES {{"mjqjpqmgbljsphdztnvjfqwrcgsmlb", 19},
                                                             {"bvwbjplbgvbhsrlpgdmjqwftvncz", 23},
                                                             {"nppdvjthqldpwncqszvftbrmjlhg", 23},
                                                             {"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 29},
                                                             {"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 26}};

    for (const auto& test : CASES)
    {
        EXPECT_EQ(find_token(test.first.begin(), test.first.end(), 14), test.second);
    }
}
