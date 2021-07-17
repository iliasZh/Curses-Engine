#pragma once

#include <array>
#include <cassert>
#include <algorithm>
#include <string_view>

enum class Lang
{
	EN, RU, 
	Size
};

// designed for many langs, I use it for two
class Expression
{
public:
	using Word = std::pair<Lang, std::wstring_view>;
	Expression() : translations{} {}
	Expression(std::initializer_list<Word> words)
	{
		assert(HasAllTranslations(words));
		for (auto it = words.begin(); it != words.end(); ++it)
		{
			translations[size_t(it->first)] = it->second;
		}
	}
	const std::wstring_view& operator[](Lang lang) const
	{
		return translations[(size_t)lang];
	}
private:
	bool HasAllTranslations(const std::initializer_list<Word>& words)
	{
		if (words.size() != numLangs) return false;
		std::array<uint8_t, numLangs> lang_indices{};
		for (auto it = words.begin(); it != words.end(); ++it)
		{
			lang_indices[it - words.begin()] = uint8_t(it->first);
		}
		std::sort(lang_indices.begin(), lang_indices.end());
		for (size_t i = 0u; i < numLangs; ++i)
		{
			if (lang_indices[i] != i) return false;
		}
		return true;
	}
private:
	static constexpr size_t numLangs = size_t(Lang::Size);
	std::array<std::wstring_view, numLangs> translations;
};

inline bool operator==(const std::wstring_view& str, const Expression& exp)
{
	for (size_t i = 0u; i < (size_t)Lang::Size; ++i)
		if (exp[Lang(i)] == str) return true;
	return false;
}