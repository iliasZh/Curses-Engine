#pragma once

#include "ExceptionBase.h"
#include "CursesWrapper.h"
#include "Keyboard.h"
#include "Utilities.h"
#include <vector>
#include <memory>

class EntryList
{
public:
	class Options
	{
	public:
		Options(std::vector<u8str_view> optList)
			: options{ optList }
			, currOption{ options.empty() ? -1 : 0 }
		{}
		const std::vector<u8str_view>& List() const { return options; }
		int CurrentOptionIndex() const;
		u8str_view CurrentOption() const;
		int NextOption(bool loop) const;
		int PrevOption(bool loop) const;
	private:
		std::vector<u8str_view> options;
		mutable int currOption;
	};

	using Entry = std::pair<u8str_view, Options>;
	EntryList(std::vector<Entry> entries);
	
	size_t EntryLength(size_t i) const;
	size_t MaxEntryLength(size_t i) const;
	size_t LongestEntryLength() const;
	size_t Size() { return entries.size(); }
	const u8str_view& EntryName(size_t i) const { return entries[i].first; }
	const Options& EntryOptions(size_t i) const { return entries[i].second; }
	bool EntryHasOptions(size_t i) const { return !(EntryOptions(i).List().empty()); }
	bool AnyEntryHasOptions() const;
private:
	std::vector<Entry> entries;
};

struct LayoutDesc
{
	using char_count = uint8_t;
	enum class Pos
	{
		TopLeft,
		BottomLeft,
		Centered,
		TopRight,
		BottomRight
	};
	LayoutDesc(Pos pos = Pos::Centered, char_count marginSize = 3u, char_count titleGap = 0u)
		: pos{ pos }, marginSize{ marginSize }, titleGap{ titleGap }
	{}
	Pos Positioning() const { return pos; }
	char_count MarginSize() const { return marginSize; }
	char_count TitleGap() const { return titleGap; }
private:
	Pos pos;
	char_count marginSize;
	char_count titleGap;
};

class Menu
{
public:
	using Color = Curses::Color;
	using Window = Curses::Window;
	using ucoord = Window::ucoord;
	
	Menu(const Window& win, const Keyboard& kbd, u8str_view title,
		EntryList entryList, LayoutDesc ld = LayoutDesc{});
	void Show() const;
private:
	u8str TitleToString();
	u8str EntryToString(size_t i);
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(std::string_view funcName, std::string_view errorDesc,
			std::string_view fileName, int line) noexcept
			: ExceptionBase{ funcName, errorDesc, fileName, line }
		{
			errorType = "Menu error";
		}
	};
private:
	std::unique_ptr<Window> menuPtr;
	const Keyboard& kbd;
	u8str_view title;
	EntryList entryList;
	LayoutDesc layoutDesc;
	size_t currEntry;
	Color baseBg		= Color::Cyan;
	Color selectBg		= Color::Blue;
	Color baseText		= Color::Black;
	Color selectText	= Color::Yellow;
	Color titleCol		= Color::Red;
	Color titleBg		= Color::Magenta;
	Color boxFg			= Color::Black;
};

#define THROW_MENU_EXCEPTION(funcName, errorDesc) throw Menu::Exception{(funcName), (errorDesc), __FILE__, __LINE__}