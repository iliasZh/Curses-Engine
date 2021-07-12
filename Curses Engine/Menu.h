#pragma once

#include "ExceptionBase.h"
#include "CursesWrapper.h"
#include "Keyboard.h"
#include "Utilities.h"
#include <vector>
#include <memory>
#include <queue>

class Entry
{
public:
	using Options = std::vector<u8str_view>;
	Entry(u8str_view name) : name{ name } {}
	virtual ~Entry() = 0;
	u8str_view Name() const { return name; }
	virtual bool IsClosing() const { return false; }
	virtual int Next(bool) const { return -1; }
	virtual int Prev(bool) const { return -1; }
	virtual Options GetOptions() const { return {}; }
	virtual int CurrentOptionIndex() const { return -1; }
	virtual u8str_view CurrentOption() const { return u8""; }
protected:
	u8str_view name;
};
inline Entry::~Entry() {} // you have to provide an implementation for a pure virtual destructor! o_O

class Button final : public Entry
{
public:
	Button(u8str_view name, bool isClosing)
		: Entry{ name }, isClosing{ isClosing }
	{}
	bool IsClosing() const override { return isClosing; }
	~Button() override {}
private:
	bool isClosing;
};

class Switch final : public Entry
{
public:
	using Options = Entry::Options;
	Switch(u8str_view name, std::vector<u8str_view> options);
	~Switch() override {}
	int Next(bool loop) const override
	{
		if (int size = (int)opts.size(); ++currOpt == size)
			currOpt = loop ? 0 : (size - 1);
		return currOpt;
	}
	int Prev(bool loop) const override
	{
		if (int size = (int)opts.size(); --currOpt == -1)
			currOpt = loop ? (size - 1) : 0;
		return currOpt;
	}
	Options GetOptions() const override { return opts; }
	int CurrentOptionIndex() const override { return currOpt; }
	u8str_view CurrentOption() const override { return opts[currOpt]; }
private:
	Options opts;
	mutable int currOpt;
};

std::unique_ptr<Button> MakeButton(u8str_view name, bool isClosing);
std::unique_ptr<Switch> MakeSwitch(u8str_view name, Entry::Options opts);

class EntryList
{
public:
	EntryList(std::vector<std::unique_ptr<Entry>>&& entries);
	
	EntryList(EntryList&& el) noexcept;
	EntryList& operator=(EntryList&& el) noexcept;

	size_t EntryLength(size_t i) const;
	size_t MaxEntryLength(size_t i) const;
	size_t LongestEntryLength() const;
	size_t Size() const { return entries.size(); }
	const std::unique_ptr<Entry>& operator[](size_t i) const { return entries[i]; }
	bool EntryHasOptions(size_t i) const { return !(entries[i]->GetOptions().empty()); }
	bool AnyEntryHasOptions() const;
private:
	std::vector<std::unique_ptr<Entry>> entries;
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

struct MenuPalette
{
private:
	using Color = Curses::Color;
public:
	Color baseBg		= Color::Cyan;
	Color selectBg		= Color::Blue;
	Color baseText		= Color::Black;
	Color selectText	= Color::Yellow;
	Color title			= Color::Green;
	Color titleBg		= Color::Black;
	Color box			= Color::Black;
};

class Menu
{
public:
	using Color = Curses::Color;
	using Window = Curses::Window;
	using ucoord = Window::ucoord;
	Menu(const Window& win, Keyboard& kbd, u8str_view title,
		EntryList entryList, LayoutDesc ld = {}, MenuPalette mp = {});
	void Touch() const;
	void Refresh() const;
	void Listen();

	// overriden by user!
	virtual void OnSelect(u8str_view name) {}
	virtual void OnSwitch(u8str_view name, u8str_view opt) {}

public: // getters/setters
	bool IsLooping() const { return loopEntryList; }
	void EnableLooping() { loopEntryList = true; }
	void DisableLooping() { loopEntryList = false; }
	bool AreSwitchesLooping() const { return loopSwitches; }
	void EnableSwitchesLooping() { loopSwitches = true; }
	void DisableSwitchesLooping() { loopSwitches = false; }
	MenuPalette& GetPalette() { return palette; }
	void SetLayoutDesc(LayoutDesc ld);
private: // helpers
	void CreateMenuWindow();
	std::pair<ucoord, ucoord> GetWindowStartPos(ucoord width, ucoord height) const;
	void Draw() const;
	void DrawEntry(size_t i) const;
	void DrawTitle() const;
	void DrawTitleGap() const;
	void DrawBox() const;
	int NextEntry();
	int PrevEntry();
	bool SwitchLeft();
	bool SwitchRight();
	u8str TitleToString() const;
	u8str EntryToString(size_t i) const;
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
	const Window& parentWin;
	std::unique_ptr<Window> menuPtr;
	Keyboard& kbd;
	u8str_view title;
	EntryList entryList;
	LayoutDesc layoutDesc;
	MenuPalette palette;
	int currEntryIndex;
	bool loopEntryList = false;
	bool loopSwitches = false;
	bool listen = false;
};

#define THROW_MENU_EXCEPTION(funcName, errorDesc) throw Menu::Exception{(funcName), (errorDesc), __FILE__, __LINE__}