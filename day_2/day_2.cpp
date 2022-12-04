#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>

#include <gtest/gtest.h>

namespace day_2_impl {

// Moves you can make
enum class Play
{
    ROCK = 0,
    PAPER = 1,
    SCISSORS = 2,
};

using Game = std::pair<Play, Play>;

// Score gained from each play
constexpr static std::array<uint8_t, 3> SCORES{{1, 2, 3}};

// WINNER[x] is the play that beats X
constexpr static std::array<Play, 3> WINNER{{Play::PAPER, Play::SCISSORS, Play::ROCK}};

Play get_play(char c)
{
    switch (c)
    {
    case 'A':
    case 'X':
        return Play::ROCK;
    case 'B':
    case 'Y':
        return Play::PAPER;
    case 'C':
    case 'Z':
        return Play::SCISSORS;
    }
    throw std::runtime_error("Unexpected input: " + std::string(1, c));
}

std::vector<Game> read_all_input(std::istream& input)
{
    std::vector<Game> result;

    std::string them, us;
    while (input >> them >> us)
    {
        if (them.size() != 1 || us.size() != 1)
        {
            throw std::runtime_error("Unexpected input: " + them + " " + us);
        }

        result.emplace_back(get_play(them[0]), get_play(us[0]));
    }

    return result;
}

uint8_t get_score(Game g)
{
    const auto play_score = SCORES[static_cast<size_t>(g.second)];

    if (g.first == g.second)
    {
        return play_score + 3;
    }

    if (g.second == WINNER[static_cast<size_t>(g.first)])
    {
        return play_score + 6;
    }

    return play_score;
}

TEST(Day2, InputGames)
{
    std::stringstream ss;
    ss << "A X\nB Y\nC Z\nA Z";

    auto games = day_2_impl::read_all_input(ss);

    ASSERT_EQ(games.size(), 4U);
    EXPECT_EQ(games[0], Game(Play::ROCK, Play::ROCK));
    EXPECT_EQ(games[1], Game(Play::PAPER, Play::PAPER));
    EXPECT_EQ(games[2], Game(Play::SCISSORS, Play::SCISSORS));
    EXPECT_EQ(games[3], Game(Play::ROCK, Play::SCISSORS));
}

TEST(Day2, Scoring)
{
    EXPECT_EQ(get_score({Play::ROCK, Play::ROCK}), 4U);
    EXPECT_EQ(get_score({Play::PAPER, Play::PAPER}), 5U);
    EXPECT_EQ(get_score({Play::SCISSORS, Play::SCISSORS}), 6U);
    EXPECT_EQ(get_score({Play::ROCK, Play::PAPER}), 8U);
    EXPECT_EQ(get_score({Play::PAPER, Play::SCISSORS}), 9U);
    EXPECT_EQ(get_score({Play::SCISSORS, Play::ROCK}), 7U);
    EXPECT_EQ(get_score({Play::PAPER, Play::ROCK}), 1U);
    EXPECT_EQ(get_score({Play::SCISSORS, Play::PAPER}), 2U);
    EXPECT_EQ(get_score({Play::ROCK, Play::SCISSORS}), 3U);
}
}  // namespace day_2_impl

void day_2(std::istream& input, std::ostream& output)
{
    const auto games = day_2_impl::read_all_input(input);

    output << std::accumulate(games.begin(), games.end(), 0, [](uint64_t sum, day_2_impl::Game g) {
        return sum + day_2_impl::get_score(g);
    });
}

void day_2_adv(std::istream& input, std::ostream& output)
{
}

TEST(Day2, Example)
{
    const static char* INPUT_DATA = R"in(A Y
B X
C Z)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_2(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "15");
}

TEST(Day2, ExampleAdvanced)
{
    const static char* INPUT_DATA = R"in(A Y
B X
C Z)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    // day_2_adv(ss_in, ss_out);
}
