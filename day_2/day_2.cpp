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

enum class Result
{
    LOSE,
    TIE,
    WIN
};

using ResultGame = std::pair<Play, Result>;
using Game = std::pair<Play, Play>;

// Score gained from each play
constexpr static std::array<uint8_t, 3> SCORES{{1, 2, 3}};

// WINNER[x] is the play that beats X
constexpr static std::array<Play, 3> WINNER{{Play::PAPER, Play::SCISSORS, Play::ROCK}};

// LOSER[x] is the play that loses to X
constexpr static std::array<Play, 3> LOSER{{Play::SCISSORS, Play::ROCK, Play::PAPER}};

Play get_play(char c)
{
    switch (c)
    {
    case 'A':
        return Play::ROCK;
    case 'B':
        return Play::PAPER;
    case 'C':
        return Play::SCISSORS;
    }
    throw std::runtime_error("Unexpected play input: " + std::string(1, c));
}

Result get_result(char c)
{
    switch (c)
    {
    case 'X':
        return Result::LOSE;
    case 'Y':
        return Result::TIE;
    case 'Z':
        return Result::WIN;
    }
    throw std::runtime_error("Unexpected result input: " + std::string(1, c));
}

std::vector<ResultGame> read_all_input(std::istream& input)
{
    std::vector<ResultGame> result;

    std::string play, game_result;
    while (input >> play >> game_result)
    {
        if (play.size() != 1 || game_result.size() != 1)
        {
            throw std::runtime_error("Unexpected input: " + play + " " + game_result);
        }

        result.emplace_back(get_play(play[0]), get_result(game_result[0]));
    }

    return result;
}

Game convert_input_result_is_play(ResultGame game)
{
    return {game.first, static_cast<Play>(game.second)};
}

Game convert_input_calculate_play(ResultGame game)
{
    switch (game.second)
    {
    case Result::WIN:
        return {game.first, WINNER[static_cast<size_t>(game.first)]};
    case Result::TIE:
        return {game.first, game.first};
    case Result::LOSE:
        return {game.first, LOSER[static_cast<size_t>(game.first)]};
    }
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
    EXPECT_EQ(games[0], ResultGame(Play::ROCK, Result::LOSE));
    EXPECT_EQ(games[1], ResultGame(Play::PAPER, Result::TIE));
    EXPECT_EQ(games[2], ResultGame(Play::SCISSORS, Result::WIN));
    EXPECT_EQ(games[3], ResultGame(Play::ROCK, Result::WIN));
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
    const auto games_in = day_2_impl::read_all_input(input);

    std::vector<day_2_impl::Game> games(games_in.size());
    std::transform(
        games_in.begin(), games_in.end(), games.begin(), &day_2_impl::convert_input_result_is_play);

    output << std::accumulate(games.begin(), games.end(), 0, [](uint64_t sum, day_2_impl::Game g) {
        return sum + day_2_impl::get_score(g);
    });
}

void day_2_adv(std::istream& input, std::ostream& output)
{
    const auto games_in = day_2_impl::read_all_input(input);

    std::vector<day_2_impl::Game> games(games_in.size());
    std::transform(
        games_in.begin(), games_in.end(), games.begin(), &day_2_impl::convert_input_calculate_play);

    output << std::accumulate(games.begin(), games.end(), 0, [](uint64_t sum, day_2_impl::Game g) {
        return sum + day_2_impl::get_score(g);
    });
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

    day_2_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "12");
}
