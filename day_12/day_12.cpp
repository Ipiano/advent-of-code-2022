#include <iostream>
#include <queue>
#include <unordered_set>
#include <gtest/gtest.h>

namespace day_12_impl
{
using Mountain = std::vector<std::string>;
using Location = std::pair<size_t, size_t>; // y, x

struct LocationHash
{
    size_t operator()(Location l) const { return std::hash<size_t>()((l.first << (sizeof(size_t) / 2)) | l.second); }
};

Location operator+(Location l, Location r)
{
    return {l.first + r.first, l.second + r.second};
}

// Return map, start, end
std::tuple<Mountain, Location, Location> read_input(std::istream& in)
{
    Mountain result;
    Location start, end;

    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty())
        {
            auto start_ind = line.find('S');
            if (start_ind != std::string::npos)
            {
                start           = {result.size(), start_ind};
                line[start_ind] = 'a';
            }

            auto end_ind = line.find('E');
            if (end_ind != std::string::npos)
            {
                end           = {result.size(), end_ind};
                line[end_ind] = 'z';
            }

            result.push_back(line);
        };
    }
    return {result, start, end};
}

TEST(Day12, ReadInput)
{
    const static char* INPUT_DATA = R"in(
Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    Mountain m;
    Location s, e;
    std::tie(m, s, e) = read_input(ss_in);

    ASSERT_EQ(m.size(), 5);
    EXPECT_EQ(m[0][0], 'a');
    EXPECT_EQ(m[2][5], 'z');

    EXPECT_EQ(s, Location(0, 0));
    EXPECT_EQ(e, Location(2, 5));
}

std::pair<std::array<Location, 4>, size_t> adjacent_nodes(const Mountain& graph, Location l)
{
    std::array<Location, 4> result_nodes;
    size_t result_size = 0;

    auto exists = [&graph](Location l)
    { return l.first >= 0 && l.second >= 0 && l.first < graph.size() && l.second < graph[l.first].size(); };

    auto valid_step = [&graph, &exists](Location a, Location b)
    {
        if (!exists(a) || !exists(b))
            return false;

        auto height_a = graph[a.first][a.second];
        auto height_b = graph[b.first][b.second];

        return height_b <= height_a + 1;
    };

    for (auto step : {Location {0, 1}, Location {1, 0}, Location {0, -1}, Location {-1, 0}})
    {
        if (valid_step(l, l + step))
            result_nodes[result_size++] = l + step;
    }

    return {result_nodes, result_size};
}

size_t bfs_distance(const Mountain& graph, Location start, Location end)
{
    size_t result_distance = 0;

    std::unordered_set<Location, LocationHash> visited;
    std::queue<Location> visit_nodes;
    visit_nodes.push(start);
    visited.insert(start);

    while (!visit_nodes.empty())
    {
        size_t this_round_count = visit_nodes.size();

        for (; this_round_count > 0; --this_round_count)
        {
            auto next = visit_nodes.front();
            visit_nodes.pop();

            if (next == end)
            {
                return result_distance;
            }

            std::array<Location, 4> adjacent_list;
            size_t adjacent_count;
            std::tie(adjacent_list, adjacent_count) = adjacent_nodes(graph, next);

            for (auto it = adjacent_list.begin(); it < adjacent_list.begin() + adjacent_count; ++it)
            {
                auto adjacent = *it;
                if (visited.insert(adjacent).second)
                {
                    visit_nodes.push(adjacent);
                }
            }
        }

        result_distance++;
    }

    return result_distance;
}
}

void day_12(std::istream& in, std::ostream& out)
{
    using namespace day_12_impl;

    Mountain m;
    Location start, end;
    std::tie(m, start, end) = read_input(in);

    out << bfs_distance(m, start, end);
}

void day_12_adv(std::istream& in, std::ostream& out)
{
}

TEST(Day12, Example)
{
    const static char* INPUT_DATA = R"in(
Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_12(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "31");
}
