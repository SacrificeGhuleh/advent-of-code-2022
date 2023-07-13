#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <regex>
#include <set>
#include <unordered_set>
#include <variant>
#include <vector>

struct Filet;
struct Directory;

using DirectoryPtr = std::shared_ptr<Directory>;
using FiletPtr = std::shared_ptr<Filet>;
using FsElement = std::variant<DirectoryPtr, FiletPtr>;

std::vector<std::string> split(const std::string& str, char c = ' ') {
    std::vector<std::string> result;
    const char* str_c = str.c_str();
    do {
        const char* begin = str_c;

        while (*str_c != c && *str_c)
            str_c++;

        result.push_back(std::string(begin, str_c));
    } while (0 != *str_c++);

    return result;
}

struct Filet {
    Filet(const std::string& n, const uint32_t s = 0) : name{n}, size{s} {}
    virtual ~Filet() = default;

    const std::string name;
    virtual uint32_t getSize() const { return size; }

    virtual std::ostream& print(std::ostream& os, size_t indent = 0) const {
        for (size_t i = 0; i < indent; i++) {
            os << " ";
        }
        os << "- " << name << "(file, size=" << getSize() << ")\n";
        return os;
    }

private:
    const uint32_t size;
};

struct Directory : public Filet, std::enable_shared_from_this<Directory> {
    Directory(const std::string& n, const DirectoryPtr& prnt = nullptr) : Filet(n), parent(prnt) {}

    std::vector<FsElement> filets;
    const DirectoryPtr parent;

    Directory& operator[](const char* idx) {
        std::unique_ptr<Filet> ret;
        if (std::strcmp("..", idx) == 0) {
            return *(parent.get());
        }

        for (const auto& f : filets) {
            if (std::holds_alternative<DirectoryPtr>(f)) {
                auto dir = std::get<DirectoryPtr>(f);
                if (std::strcmp(dir->name.c_str(), idx) == 0) {
                    return *(dir.get());
                }
            }
        }
        std::stringstream ss;
        ss << "Key " << idx << " not found";
        throw std::out_of_range(ss.str());
    }
    Directory& operator[](const std::string& idx) { return operator[](idx.c_str()); }

    DirectoryPtr operator()() { return shared_from_this(); }

    virtual uint32_t getSize() const override {
        uint32_t s = Filet::getSize();
        for (const auto& fVariant : filets) {
            FiletPtr f;
            if (std::holds_alternative<DirectoryPtr>(fVariant)) {
                f = std::get<DirectoryPtr>(fVariant);
            }

            if (std::holds_alternative<FiletPtr>(fVariant)) {
                f = std::get<FiletPtr>(fVariant);
            }

            s += f->getSize();
        }
        return s;
    }

    virtual std::ostream& print(std::ostream& os, size_t indent = 0) const override {
        for (size_t i = 0; i < indent; i++) {
            os << " ";
        }

        if (name.compare("") != 0) {
            os << "- " << name << "(dir)\n";
            indent += 2;
        }

        for (const auto& fVariant : filets) {
            FiletPtr f;
            if (std::holds_alternative<DirectoryPtr>(fVariant)) {
                f = std::get<DirectoryPtr>(fVariant);
            }

            if (std::holds_alternative<FiletPtr>(fVariant)) {
                f = std::get<FiletPtr>(fVariant);
            }
            f->print(os, indent);
        }
        return os;
    }

    template <class T>
    void emplace(std::shared_ptr<T> arg) {
        FsElement fsElement;
        fsElement.emplace<std::shared_ptr<T>>(arg);
        filets.emplace_back(fsElement);
    }
};

std::ostream& operator<<(std::ostream& os, const Filet& filet) {
    return filet.print(os);
}

const std::regex cdRgx("^$");
struct smallestCompare {
    bool operator()(const DirectoryPtr& lhs, const DirectoryPtr& rhs) const { return lhs->getSize() < rhs->getSize(); }
};

using MultisetBySmallest = std::multiset<DirectoryPtr, smallestCompare>;
struct Filesystem {
    Filesystem(const std::vector<std::string>& commands) : fs{std::make_shared<Directory>("")} {
        fs->emplace<Directory>(std::make_unique<Directory>("/", fs));
        DirectoryPtr current = fs;
        for (const auto& command : commands) {
            const std::vector<std::string> tokenized = split(command);
            const bool isCommand = (tokenized.at(0).compare("$") == 0);

            if (isCommand) {
                if (tokenized.at(1).compare("cd") == 0) {
                    // change direktory
                    current = (*current)[tokenized.at(2).c_str()]();
                }
            } else {
                if (tokenized.at(0).compare("dir") == 0) {
                    current->emplace<Directory>(std::make_unique<Directory>(tokenized.at(1), (*current)()));
                } else {
                    size_t len = std::stoul(tokenized.at(0));
                    current->emplace<Filet>(std::make_unique<Filet>(tokenized.at(1), len));
                }
            }
        }
    }

    MultisetBySmallest getSmallest(size_t threshold, DirectoryPtr fsDirs = nullptr) {
        MultisetBySmallest dirs;
        static_cast<void>(threshold);
        if (fsDirs == nullptr)
            fsDirs = fs;
        for (const auto& fVariant : fsDirs->filets) {
            if (std::holds_alternative<DirectoryPtr>(fVariant)) {
                DirectoryPtr dir = std::get<DirectoryPtr>(fVariant);
                if (dir->getSize() < threshold) {
                    dirs.insert(dir);
                }

                auto smallestDirs = getSmallest(threshold, dir);
                for (const auto& d : smallestDirs) {
                    dirs.insert(d);
                }
            }
        }
        return dirs;
    }

    std::multiset<DirectoryPtr> getBiggest(size_t threshold, DirectoryPtr fsDirs = nullptr) {
        std::multiset<DirectoryPtr> dirs;
        static_cast<void>(threshold);
        if (fsDirs == nullptr)
            fsDirs = fs;
        for (const auto& fVariant : fsDirs->filets) {
            if (std::holds_alternative<DirectoryPtr>(fVariant)) {
                DirectoryPtr dir = std::get<DirectoryPtr>(fVariant);
                if (dir->getSize() > threshold) {
                    dirs.insert(dir);

                    auto biggestDirs = getBiggest(threshold, dir);

                    for (const auto& d : biggestDirs) {
                        dirs.insert(d);
                    }
                }
            }
        }
        return dirs;
    }
    DirectoryPtr fs;
};
void test() {
    Directory d("/");
    d.emplace<Directory>(std::make_unique<Directory>("a"));
    d.emplace<Directory>(std::make_unique<Directory>("b"));
    d.emplace<Directory>(std::make_unique<Directory>("c"));

    d["a"].emplace<Directory>(std::make_unique<Directory>("e"));
    d["a"]["e"].emplace<Directory>(std::make_unique<Directory>("e"));
    d["a"].emplace<Directory>(std::make_unique<Directory>("f"));
    d["a"].emplace<Filet>(std::make_unique<Filet>("fokotina.fo", 123456789));

    std::cout << d << "\n";

    const std::vector<std::string> commands = {
        "$ cd /",  "$ ls",    "dir a",  "14848514 b.txt", "8504156 c.dat", "dir d",         "$ cd a",   "$ ls",
        "dir e",   "29116 f", "2557 g", "62596 h.lst",    "$ cd e",        "$ ls",          "584 i",    "$ cd ..",
        "$ cd ..", "$ cd d",  "$ ls",   "4060174 j",      "8033020 d.log", "5626152 d.ext", "7214296 k"};

    Filesystem fs(commands);
    std::cout << "Size: " << fs.fs->getSize() << "\n";
    std::cout << *fs.fs << "\n";

    std::cout << "Small dirs: \n";

    const auto smallest = fs.getSmallest(100000);

    for (const auto& smallDir : smallest) {
        std::cout << smallDir->name << " (size=" << smallDir->getSize() << ")\n";
    }

    assert(fs.fs->getSize() == 48381165);
}

int main() {
    std::cout << "Tests:\n";
    test();
    std::cout << "\nSolution:\n";

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");
    std::vector<std::string> commands;
    while (std::getline(inputFile, input)) {
        commands.push_back(input.c_str());
        // imput
    }
    inputFile.close();

    Filesystem fs(commands);
    std::cout << "Size: " << fs.fs->getSize() << "\n";
    // std::cout << *fs.fs << "\n";

    const auto smallest = fs.getSmallest(100000);

    size_t smallDirsSum = 0;
    for (const auto& smallDir : smallest) {
        // std::cout << smallDir->name << " (size=" << smallDir->getSize() << ")\n";
        smallDirsSum += smallDir->getSize();
    }

    std::cout << "Smallest dirs size sum: " << smallDirsSum << "\n";
    // Close the file

    const size_t requiredSpace = 30000000;
    const size_t filesystemSpace = 70000000;
    const size_t availableSpace = filesystemSpace - fs.fs->getSize();

    const auto allDirs = fs.getSmallest(std::numeric_limits<size_t>::max());
    for (const auto& smallDir : allDirs) {
        if (availableSpace + smallDir->getSize() >= requiredSpace) {
            std::cout << "To remove: " << smallDir->name << " (size=" << smallDir->getSize() << ")\n";
            break;
        }
    }
}
