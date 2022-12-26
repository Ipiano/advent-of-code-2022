#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <type_traits>

#include <gtest/gtest.h>

#include <cassert>

namespace day_7_impl
{
struct File
{
    enum class Type
    {
        FILE,
        DIR
    };

    Type type;
    std::string name;
    size_t size;
};

struct Command
{
    enum class Type
    {
        CD,
        LS
    };

    Type type;
    std::string cd_arg;
    std::vector<File> ls_files;
};

class FileTree
{
  public:
    FileTree(const std::string& dirname, FileTree* parent = nullptr) : m_parent(parent), m_name(dirname) { }

    const std::string& name() const { return m_name; }

    // Get the parent in the tree
    FileTree* parent() const { return m_parent; }

    // Get a child of this dir; if it does not exist, it is created
    FileTree* child(const std::string& child_name)
    {
        m_dirty = true;

        auto child_it = m_children.find(child_name);
        if (child_it == m_children.end())
        {
            bool dummy;
            std::tie(child_it, dummy) = m_children.emplace(child_name, std::make_unique<FileTree>(child_name, this));
            assert(dummy == true);

            m_children_names.push_back(child_name);
        }
        return child_it->second.get();
    }

    const FileTree* child(const std::string& child_name) const
    {
        auto child_it = m_children.find(child_name);
        if (child_it == m_children.end())
        {
            throw std::runtime_error("child does not exist");
        }
        return child_it->second.get();
    }

    std::vector<std::string> children() const { return m_children_names; }

    void add_file(const std::string& name, size_t size)
    {
        assert(m_files.count(name) == 0 || m_files[name] == size);

        if (m_files.count(name) > 0)
        {
            return;
        }

        m_files[name] = size;
        m_files_size += size;

        m_dirty = true;
    }

    size_t size() const
    {
        if (m_dirty)
        {
            m_calculated_size = m_files_size;
            for (const auto& subdir_pair : m_children)
            {
                m_calculated_size += subdir_pair.second->size();
            }

            m_dirty = false;
        }

        return m_calculated_size;
    }

  private:
    FileTree* m_parent;
    std::string m_name;

    std::unordered_map<std::string, std::unique_ptr<FileTree>> m_children;
    std::vector<std::string> m_children_names;

    std::unordered_map<std::string, size_t> m_files;
    size_t m_files_size = 0;

    mutable size_t m_calculated_size = 0;
    mutable bool m_dirty             = true;
};

bool operator==(const File& l, const File& r)
{
    return l.type == r.type && l.name == r.name && (l.type == File::Type::DIR || l.size == r.size);
}

bool operator==(const Command& l, const Command& r)
{
    return l.type == r.type &&
           ((l.type == Command::Type::CD && l.cd_arg == r.cd_arg) || (l.type == Command::Type::LS && l.ls_files == r.ls_files));
}

std::vector<std::string> split(const std::string& text)
{
    std::vector<std::string> result;

    auto start = text.begin();
    for (auto it = text.begin(); it != text.end(); ++it)
    {
        if (*it == ' ')
        {
            result.emplace_back(start, it);
            start = it + 1;
        }
    }
    result.emplace_back(start, text.end());

    return result;
}

std::vector<Command> read_input(std::istream& stream)
{
    std::vector<Command> commands;

    assert(stream.peek() == '$');
    std::string line;

    while (stream)
    {
        Command next_command;
        if (!std::getline(stream, line))
        {
            break;
        }

        const auto parts = split(line);
        assert(parts[0] == "$");

        if (parts[1] == "cd")
        {
            next_command.type   = Command::Type::CD;
            next_command.cd_arg = parts[2];
        }
        else if (parts[1] == "ls")
        {
            next_command.type = Command::Type::LS;

            while (stream && stream.peek() != '$')
            {
                std::getline(stream, line);
                const auto parts = split(line);

                File f;
                f.name = parts[1];
                if (parts[0] == "dir")
                {
                    f.type = File::Type::DIR;
                }
                else
                {
                    f.type = File::Type::FILE;
                    f.size = std::stoull(parts[0]);
                }
                next_command.ls_files.push_back(std::move(f));
            }
        }
        commands.push_back(std::move(next_command));
    }

    return commands;
};

std::unique_ptr<FileTree> build_tree(const std::vector<Command>& commands)
{
    auto result    = std::make_unique<FileTree>("/", nullptr);
    FileTree* curr = result.get();
    for (const Command& c : commands)
    {
        switch (c.type)
        {
        case Command::Type::CD:
        {
            if (c.cd_arg == "/")
            {
                curr = result.get();
            }
            else if (c.cd_arg == "..")
            {
                curr = curr->parent();
            }
            else
            {
                curr = curr->child(c.cd_arg);
            }
        }
        break;
        case Command::Type::LS:
        {
            for (const auto& child : c.ls_files)
            {
                if (child.type == File::Type::FILE)
                {
                    curr->add_file(child.name, child.size);
                }
            }
        }
        break;
        }
    }
    return result;
}

template <class F> void visit(const FileTree& tree, const F& f)
{
    f(tree);

    for (const auto& c : tree.children())
    {
        visit(*tree.child(c), f);
    }
}

TEST(Day7, SplitString)
{
    EXPECT_EQ(split("$ cd .."), std::vector<std::string>({"$", "cd", ".."}));
    EXPECT_EQ(split("$ ls"), std::vector<std::string>({"$", "ls"}));
    EXPECT_EQ(split("abcd"), std::vector<std::string>({"abcd"}));
}

TEST(Day7, ParseInput)
{
    const static char* INPUT_DATA = R"in($ cd /
$ cd abc
$ ls
dir a
dir b
1234 file.txt
5678 other_file
$ cd /)in";

    std::stringstream ss_in;
    ss_in << INPUT_DATA;

    const auto commands = read_input(ss_in);

    ASSERT_EQ(commands.size(), 4U);
    EXPECT_EQ(commands[0], Command({Command::Type::CD, "/", {}}));
    EXPECT_EQ(commands[1], Command({Command::Type::CD, "abc", {}}));
    EXPECT_EQ(commands[2], Command({Command::Type::LS,
                                    "",
                                    {File({File::Type::DIR, "a", 0}), File({File::Type::DIR, "b", 0}),
                                     File({File::Type::FILE, "file.txt", 1234}), File({File::Type::FILE, "other_file", 5678})}}));
    EXPECT_EQ(commands[3], Command({Command::Type::CD, "/", {}}));
}

TEST(Day7, BuildTreeSimple)
{
    std::vector<Command> commands {Command {Command::Type::LS, "", {File {File::Type::FILE, "a", 10}, File {File::Type::FILE, "b", 20}}}};
    auto tree = build_tree(commands);

    EXPECT_EQ(tree->children().size(), 0);
    EXPECT_EQ(tree->size(), 30);
}

TEST(Day7, BuildTreeNested)
{
    std::vector<Command> commands {
        Command {
            Command::Type::LS, "", {File {File::Type::DIR, "x", 0}, File {File::Type::FILE, "a", 10}, File {File::Type::FILE, "b", 20}}},
        Command {Command::Type::CD, "x", {}}, Command {Command::Type::LS, "", {File {File::Type::FILE, "a", 30}}}};

    auto tree = build_tree(commands);

    ASSERT_EQ(tree->children().size(), 1);
    EXPECT_EQ(tree->children()[0], "x");
    EXPECT_EQ(tree->size(), 60);

    auto sub_tree = tree->child("x");
    EXPECT_EQ(sub_tree->children().size(), 0);
    EXPECT_EQ(sub_tree->size(), 30);
}
}

void day_7(std::istream& input, std::ostream& output)
{
    using namespace day_7_impl;

    const auto commands = read_input(input);
    const auto tree     = build_tree(commands);

    size_t sum_size = 0;
    visit(*tree,
          [&](const FileTree& ft)
          {
              if (ft.size() <= 100000)
              {
                  sum_size += ft.size();
              }
          });

    output << sum_size;
}

void day_7_adv(std::istream& input, std::ostream& output)
{
    using namespace day_7_impl;
}

TEST(Day7, Example)
{
    const static char* INPUT_DATA = R"in($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k)in";

    std::stringstream ss_in, ss_out;
    ss_in << INPUT_DATA;

    day_7(ss_in, ss_out);

    EXPECT_EQ(ss_out.str(), "95437");
}
