#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <set>
#include <string>
#include <vector>

class spell_checker {
public:
    explicit spell_checker(const std::string& words_file)
    {
        std::ifstream file(words_file);
        std::string word;
        while (std::getline(file, word)) {
            if (!word.empty()) {
                words.insert(word);
            }
        }
    }

    [[nodiscard]] bool is_correct(const std::string& word) const
    {
        std::string cleaned = clean(word);
        if (cleaned.empty()) {
            return true;
        }
        return words.count(cleaned) > 0;
    }

    [[nodiscard]] std::vector<std::string> suggestions(const std::string& word) const
    {
        std::string cleaned = clean(word);
        std::vector<std::string> result;

        for (const auto& dict_word : words) {
            if (edit_distance(cleaned, dict_word) == 1) {
                result.push_back(dict_word);
                if (result.size() >= 5) {
                    break;
                }
            }
        }

        return result;
    }

private:
    std::set<std::string> words;

    [[nodiscard]] static std::string clean(const std::string& word)
    {
        std::string result;
        for (const unsigned char ch : word) {
            if (std::isalpha(ch)) {
                result += static_cast<char>(std::tolower(ch));
            }
        }
        return result;
    }

    [[nodiscard]] static int edit_distance(const std::string& a, const std::string& b)
    {
        const auto la = a.size();
        const auto lb = b.size();

        if (la == 0) return static_cast<int>(lb);
        if (lb == 0) return static_cast<int>(la);
        if (std::abs(static_cast<int>(la) - static_cast<int>(lb)) > 1) return 2;

        std::vector<int> prev(lb + 1), curr(lb + 1);
        for (std::size_t j = 0; j <= lb; ++j) {
            prev[j] = static_cast<int>(j);
        }

        for (std::size_t i = 1; i <= la; ++i) {
            curr[0] = static_cast<int>(i);
            for (std::size_t j = 1; j <= lb; ++j) {
                const int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
                curr[j] = std::min({ prev[j] + 1, curr[j - 1] + 1, prev[j - 1] + cost });
            }
            std::swap(prev, curr);
        }

        return prev[lb];
    }
};

#endif // SPELL_CHECKER_H