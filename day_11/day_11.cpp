#include <iostream>
#include <cassert>
#include <cstdlib>
#include <functional>

#include <gtest/gtest.h>

namespace day_11_impl
{
using WorryFn = std::function<size_t(size_t)>;
using ThrowFn = std::function<size_t(size_t)>;

struct Monkey
{
    size_t total_items = 0;
    std::vector<size_t> items;

    WorryFn worry_fn;
    ThrowFn throw_fn;
};

template <class PostXForm> WorryFn make_transform(const std::string& operation, PostXForm p)
{
    char op;
    char arg_str[5] = {};
    std::sscanf(operation.c_str(), " Operation: new = old %1c %s", &op, arg_str);

    int arg = std::atoi(arg_str);

    if (op == '+')
    {
        return arg ? WorryFn([arg, p](size_t x) { return p(x + arg); }) : WorryFn([p](size_t x) { return p(x + x); });
    }
    else
    {
        assert(op == '*');
        return arg ? WorryFn([arg, p](size_t x) { return p(x * arg); }) : WorryFn([p](size_t x) { return p(x * x); });
    }
}

ThrowFn make_evaluation(const std::string& rule, const std::string& if_true, const std::string& if_false)
{
    size_t divisor;
    size_t true_target;
    size_t false_target;
    std::sscanf(rule.c_str(), " Test: divisible by %zd", &divisor);
    std::sscanf(if_true.c_str(), " If true: throw to monkey %zd", &true_target);
    std::sscanf(if_false.c_str(), " If false: throw to monkey %zd", &false_target);

    return [divisor, true_target, false_target](size_t x) { return (x % divisor) == 0 ? true_target : false_target; };
}

std::vector<size_t> parse_items(const std::string& item_list)
{
    std::vector<size_t> result;
    auto start = item_list.find(':');

    while (start != std::string::npos)
    {
        result.push_back(std::atoi(item_list.data() + start + 1));
        start = item_list.find(',', start + 1);
    }

    return result;
}

template <class PostXForm> std::vector<Monkey> read_input(std::istream& in, PostXForm p)
{
    std::vector<Monkey> monkeys;

    while (in)
    {
        std::array<std::string, 7> monkey_data;
        for (auto& s : monkey_data)
            std::getline(in, s);

        Monkey m;
        m.items    = parse_items(monkey_data[1]);
        m.worry_fn = make_transform(monkey_data[2], p);
        m.throw_fn = make_evaluation(monkey_data[3], monkey_data[4], monkey_data[5]);

        monkeys.push_back(std::move(m));
    }

    return monkeys;
}

TEST(Day11, ParseItemList)
{
    EXPECT_EQ(parse_items("Starting items: 10, 20, 30"), std::vector<size_t>({10, 20, 30}));
}

void simulate(std::vector<Monkey>& monkeys, size_t rounds)
{
    for (; rounds > 0; --rounds)
    {
        for (auto& m : monkeys)
        {
            m.total_items += m.items.size();
            for (size_t i : m.items)
            {
                auto j = m.worry_fn(i);
                monkeys[m.throw_fn(j)].items.push_back(j);
            }

            m.items.clear();
        }
    }
}
}

void day_11(std::istream& in, std::ostream& out)
{
    using namespace day_11_impl;

    auto monkeys = read_input(in, [](size_t x) { return x / 3; });
    simulate(monkeys, 20);

    std::sort(monkeys.begin(), monkeys.end(), [](const Monkey& l, const Monkey& r) { return l.total_items > r.total_items; });

    out << (monkeys[0].total_items * monkeys[1].total_items);
}

void day_11_adv(std::istream& in, std::ostream& out)
{
    using namespace day_11_impl;

    auto monkeys = read_input(in, [](size_t x) { return x % (2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23); });
    simulate(monkeys, 10000);

    std::sort(monkeys.begin(), monkeys.end(), [](const Monkey& l, const Monkey& r) { return l.total_items > r.total_items; });

    out << (monkeys[0].total_items * monkeys[1].total_items);
}

TEST(Day11, Example)
{
    const static char* INPUT_DATA = R"in(Monkey 0:
  Starting items: 79, 98
  Operation: new = old * 19
  Test: divisible by 23
    If true: throw to monkey 2
    If false: throw to monkey 3

Monkey 1:
  Starting items: 54, 65, 75, 74
  Operation: new = old + 6
  Test: divisible by 19
    If true: throw to monkey 2
    If false: throw to monkey 0

Monkey 2:
  Starting items: 79, 60, 97
  Operation: new = old * old
  Test: divisible by 13
    If true: throw to monkey 1
    If false: throw to monkey 3

Monkey 3:
  Starting items: 74
  Operation: new = old + 3
  Test: divisible by 17
    If true: throw to monkey 0
    If false: throw to monkey 1
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_11(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "10605");
}

TEST(Day11, ExampleAdvanced)
{
    const static char* INPUT_DATA = R"in(Monkey 0:
  Starting items: 79, 98
  Operation: new = old * 19
  Test: divisible by 23
    If true: throw to monkey 2
    If false: throw to monkey 3

Monkey 1:
  Starting items: 54, 65, 75, 74
  Operation: new = old + 6
  Test: divisible by 19
    If true: throw to monkey 2
    If false: throw to monkey 0

Monkey 2:
  Starting items: 79, 60, 97
  Operation: new = old * old
  Test: divisible by 13
    If true: throw to monkey 1
    If false: throw to monkey 3

Monkey 3:
  Starting items: 74
  Operation: new = old + 3
  Test: divisible by 17
    If true: throw to monkey 0
    If false: throw to monkey 1
)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_11_adv(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "2713310158");
}
