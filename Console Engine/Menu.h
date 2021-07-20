#pragma once

#include "ExceptionBase.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Utilities.h"
#include "GlobalParameters.h"
#include "Expression.h"
#include "CommonExpressions.h"
#include "ConsoleWrapper.h"
#include <vector>
#include <memory>
#include <queue>

class Entry
{
public:
	using AbstractOptions = std::vector<Expression>;
	using Options = std::vector<std::wstring_view>;
	Entry(Expression name) : name{ name } {}
	virtual ~Entry() = 0;
	std::wstring_view Name(Lang lang) const { return name[lang]; }
	virtual bool IsClosing() const { return false; }
	virtual int Next(bool) const { return -1; }
	virtual int Prev(bool) const { return -1; }
	bool HasOptions() const { return CurrentOptionIndex() != -1; }
	virtual Options GetOptions(Lang lang) const { return {}; }
	virtual int CurrentOptionIndex() const { return -1; }
	virtual std::wstring_view CurrentOption(Lang lang) const { return L""; }
protected:
	Expression name;
};
inline Entry::~Entry() {} // you have to provide an implementation for a pure virtual destructor! o_O

class Button final : public Entry
{
public:
	Button(Expression name, bool isClosing)
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
	using AbstractOptions = Entry::AbstractOptions;
	Switch(Expression name, AbstractOptions options);
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
	Options GetOptions(Lang lang) const override
	{
		Options opts_ret{};
		opts_ret.reserve(opts.size());
		for (const auto& opt : opts)
			opts_ret.push_back(opt[lang]);
		return opts_ret;
	}
	int CurrentOptionIndex() const override { return currOpt; }
	std::wstring_view CurrentOption(Lang lang) const override { return opts[currOpt][lang]; }
private:
	AbstractOptions opts;
	mutable int currOpt;
};

std::unique_ptr<Button> MakeButton(Expression name, bool isClosing);
std::unique_ptr<Switch> MakeSwitch(Expression name, Switch::AbstractOptions opts);

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
	LayoutDesc(Pos pos = Pos::Centered, char_count marginHoriz = 1u, char_count marginVert = 0u)
		: pos{ pos }, marginHoriz{ marginHoriz }, marginVert{ marginVert }
	{}
	Pos Positioning() const { return pos; }
	char_count HorizontalMargin() const { return marginHoriz; }
	char_count VerticalMargin() const { return marginVert; }
private:
	Pos pos;
	char_count marginHoriz;
	char_count marginVert;
};

struct MenuPalette
{
private:
	using Color = Window::Color;
public:
	Color baseBg			= Color::Black;
	Color entrySelectBg		= Color::Blue;
	Color entrySelectText	= Color::Yellow;
	Color entryBg			= Color::Cyan;
	Color entryText			= Color::Black;
	Color title				= Color::Green;
	Color box				= Color::White;
};

class Menu
{
public:
	using Color = Window::Color;
	using ucoord = USHORT;
	Menu(const Window&, Keyboard&, Mouse&, Expression title,
		EntryList entryList, LayoutDesc ld = {}, MenuPalette mp = {});
	virtual ~Menu() = default;
	void Refresh() const;
	void Listen();

	// overriden by user!
	virtual void OnSelect(std::wstring_view name)							= 0;
	virtual void OnSwitch(std::wstring_view name, std::wstring_view opt)	= 0;

public: // getters/setters
	const Expression& Title() const { return title; }
	bool IsLooping() const { return loopEntryList; }
	void EnableLooping() { loopEntryList = true; }
	void DisableLooping() { loopEntryList = false; }
	bool AreSwitchesLooping() const { return loopSwitches; }
	void EnableSwitchesLooping() { loopSwitches = true; }
	void DisableSwitchesLooping() { loopSwitches = false; }
	MenuPalette& GetPalette() { return palette; }
	void SetLayoutDesc(LayoutDesc ld);
	void SetEntryList(EntryList&& el);
protected: // helpers
	void CreateMenuWindow();
	void Draw() const;
private:
	std::pair<ucoord, ucoord> GetWindowStartPos(ucoord width, ucoord height) const;
	void DrawEntry(size_t i) const;
	void DrawTitle() const;
	void DrawUpperMargin() const;
	void DrawLowerMargin() const;
	void DrawBox() const;
	int GoToEntry(int);
	int NextEntry();
	int PrevEntry();
	bool SwitchLeft();
	bool SwitchRight();
	std::wstring TitleToString() const;
	std::wstring EntryToString(size_t i) const;
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
protected:
	const Window& parentWin;
	std::unique_ptr<Window> menuPtr;
	MenuPalette palette;
	Expression title;
private:
	Keyboard& kbd;
	Mouse& mouse;
	EntryList entryList;
	LayoutDesc layoutDesc;
	int currEntryIndex;
	bool loopEntryList = false;
	bool loopSwitches = false;
	bool listen = false;
};

#define THROW_MENU_EXCEPTION(funcName, errorDesc) throw Menu::Exception{(funcName), (errorDesc), __FILE__, __LINE__}