#include <iostream>
#include <queue>
#include <unordered_set>
#include <cassert>

#include <gtest/gtest.h>

namespace day_12_impl
{
using Mountain = std::vector<std::string>;
using Location = std::pair<int, int>; // y, x

struct LocationHash
{
    size_t operator()(Location l) const { return std::hash<size_t>()((size_t(l.first) << (sizeof(size_t) / 2)) | l.second); }
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

std::vector<Location> bfs(const Mountain& graph, std::vector<Location> starts, Location end)
{
    std::unordered_map<Location, Location, LocationHash> steps_from;

    std::queue<Location> visit_nodes;

    for (const auto s : starts)
    {
        visit_nodes.push(s);
        steps_from.emplace(s, s);
    }

    while (!visit_nodes.empty())
    {
        size_t this_round_count = visit_nodes.size();

        for (; this_round_count > 0; --this_round_count)
        {
            auto current = visit_nodes.front();
            visit_nodes.pop();

            if (current == end)
            {
                visit_nodes = {};
                break;
            }

            std::array<Location, 4> adjacent_list;
            size_t adjacent_count;
            std::tie(adjacent_list, adjacent_count) = adjacent_nodes(graph, current);

            for (auto it = adjacent_list.begin(); it < adjacent_list.begin() + adjacent_count; ++it)
            {
                auto adjacent = *it;
                if (steps_from.emplace(adjacent, current).second)
                {
                    visit_nodes.push(adjacent);
                }
            }
        }
    }

    assert(steps_from.count(end) != 0);

    std::unordered_set<Location, LocationHash> start_set {starts.begin(), starts.end()};
    std::vector<Location> path;
    path.reserve(visit_nodes.size());

    path.push_back(end);
    while (start_set.count(path.back()) == 0)
    {
        path.push_back(steps_from[path.back()]);
    }

    std::reverse(path.begin(), path.end());

    return path;
}

void print_solution(Mountain m, const std::vector<Location>& path)
{
    std::unordered_set<Location, LocationHash> visited {path.begin(), path.end()};

    for (size_t i = 0; i < m.size(); ++i)
        for (size_t j = 0; j < m[i].size(); ++j)
            if (visited.count({i, j}) == 0)
                m[i][j] = '.';

    for (const auto& l : m)
        std::cout << l << std::endl;
}
}

void day_12(std::istream& in, std::ostream& out)
{
    using namespace day_12_impl;

    Mountain m;
    Location start, end;
    std::tie(m, start, end) = read_input(in);

    auto path = bfs(m, {start}, end);
    print_solution(m, path);

    out << path.size() - 1;
}

void day_12_adv(std::istream& in, std::ostream& out)
{
    using namespace day_12_impl;

    Mountain m;
    Location start, end;
    std::tie(m, start, end) = read_input(in);

    std::vector<Location> starts;
    for (size_t i = 0; i < m.size(); ++i)
        for (size_t j = 0; j < m[i].size(); ++j)
            if (m[i][j] == 'a')
                starts.push_back({i, j});

    auto path = bfs(m, starts, end);
    print_solution(m, path);

    out << path.size() - 1;
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

TEST(Day12, ExampleAdvanced)
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

    day_12_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "29");
}
