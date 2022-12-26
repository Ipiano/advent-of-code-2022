#include <iostream>
#include <numeric>

#include <gtest/gtest.h>

namespace day_8_impl
{
template <class T> class ColumnIterator
{
  public:
    ColumnIterator();
    ColumnIterator(std::vector<std::vector<T>>& grid, size_t column, size_t index) : m_column(column), m_grid(&grid), m_curr(index) {};

    T& operator*() { return (*m_grid)[m_curr][m_column]; }
    T* operator->() { return &(operator*()); }

    ColumnIterator& operator++()
    {
        m_curr++;
        return *this;
    }

    ColumnIterator& operator--()
    {
        m_curr--;
        return *this;
    }

    bool operator==(const ColumnIterator& other) const
    {
        if (!valid() && !other.valid())
            return true;

        if (valid() != other.valid())
            return false;

        return m_grid == other.m_grid && m_column == other.m_column && m_curr == other.m_curr;
    }

    bool operator!=(const ColumnIterator& other) const { return !(*this == other); }

  private:
    bool valid() const { return m_grid && m_curr < m_grid->size() && m_curr >= 0; }

    size_t m_column                     = 0;
    std::vector<std::vector<T>>* m_grid = nullptr;

    size_t m_curr = 0;
};

template <class T> ColumnIterator<T> begin_column(std::vector<std::vector<T>>& grid, size_t column)
{
    return {grid, column, 0};
}

template <class T> ColumnIterator<T> end_column(std::vector<std::vector<T>>& grid, size_t column)
{
    return {grid, column, grid.size()};
}

// Height, is visible
using Tree   = std::pair<size_t, bool>;
using Forest = std::vector<std::vector<Tree>>;

Forest read_input(std::istream& in)
{
    Forest result;

    std::string line;
    while (std::getline(in, line) && !line.empty())
    {
        std::vector<Tree> tree_line;
        tree_line.reserve(line.size());

        for (const auto& c : line)
        {
            tree_line.emplace_back(c - '0', false);
        }
        result.push_back(std::move(tree_line));
    }

    assert(
        [&]
        {
            for (const auto& row : result)
            {
                if (row.size() != result.size())
                    return false;
            }
            return true;
        }());

    return result;
}

template <class IteratorT> void mark_visible_trees(IteratorT begin, IteratorT end)
{
    auto forward  = begin;
    auto backward = end;
    --backward;

    size_t forward_height  = forward->first;
    size_t backward_height = backward->first;

    forward->second  = true;
    backward->second = true;

    for (; forward != end; ++forward, --backward)
    {
        if (forward->first > forward_height)
        {
            forward_height  = forward->first;
            forward->second = true;
        }

        if (backward->first > backward_height)
        {
            backward_height  = backward->first;
            backward->second = true;
        }
    }
}

void mark_visible_trees(Forest& f)
{
    for (auto& row : f)
    {
        mark_visible_trees(row.begin(), row.end());
    }

    for (size_t i = 0; i < f.size(); ++i)
    {
        mark_visible_trees(begin_column(f, i), end_column(f, i));
    }
}

size_t count_visible_trees(const Forest& f)
{
    return std::accumulate(f.begin(), f.end(), 0,
                           [](size_t x, const std::vector<Tree>& row)
                           { return x + std::count_if(row.begin(), row.end(), [](const Tree& t) { return t.second; }); });
}

size_t scenic_score(const Forest& f, size_t y, size_t x)
{
    // score 0 on the border - important to shortcut so checks below don't
    // go out of bounds
    if (x == 0 || y == 0 || x == f.size() - 1 || y == f.size() - 1)
    {
        return 0;
    }

    size_t score_up    = 1;
    size_t score_down  = 1;
    size_t score_left  = 1;
    size_t score_right = 1;

    size_t this_height = f[y][x].first;

    for (size_t i = x + 1; i < f.size() - 1 && f[y][i].first < this_height; ++i, ++score_right)
        ;

    for (size_t i = 1; i < x && f[y][x - i].first < this_height; ++i, ++score_left)
        ;

    for (size_t i = y + 1; i < f.size() - 1 && f[i][x].first < this_height; ++i, ++score_down)
        ;

    for (size_t i = 1; i < y && f[y - i][x].first < this_height; ++i, ++score_up)
        ;
    return score_left * score_right * score_up * score_down;
}
}

void day_8(std::istream& in, std::ostream& out)
{
    using namespace day_8_impl;

    auto forest = read_input(in);
    mark_visible_trees(forest);

    out << count_visible_trees(forest);
}

void day_8_adv(std::istream& in, std::ostream& out)
{
    using namespace day_8_impl;

    auto forest = read_input(in);

    size_t best_score = 0;
    for (size_t i = 0; i < forest.size(); ++i)
    {
        for (size_t j = 0; j < forest.size(); ++j)
        {
            best_score = std::max(best_score, scenic_score(forest, i, j));
        }
    }

    out << best_score;
}

TEST(Day8, Example)
{
    const static char* INPUT_DATA = R"in(30373
25512
65332
33549
35390)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_8(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "21");
}

TEST(Day8, ExampleAdvanced)
{
    const static char* INPUT_DATA = R"in(30373
25512
65332
33549
35390)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_8_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "8");
}
