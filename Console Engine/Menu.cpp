#include "precompiled.h"
#include "Menu.h"
#include "Timer.h"
#include "GlobalParameters.h"
#include <thread>

Switch::Switch(Expression name, AbstractOptions options)
	: Entry{ name }
	, opts{ options }
	, currOpt{ 0 }
{
	if (options.empty())
		THROW_MENU_EXCEPTION("Switch constructor", "empty options not allowed");
}

std::unique_ptr<Button> MakeButton(Expression name, bool isClosing)
{
	return std::make_unique<Button>(name, isClosing);
}
std::unique_ptr<Switch> MakeSwitch(Expression name, Switch::AbstractOptions opts)
{
	return std::make_unique<Switch>(name, opts);
}

EntryList::EntryList(std::vector<std::unique_ptr<Entry>>&& ents)
	: entries{ std::move(ents) }
{
	entries.shrink_to_fit();
}

EntryList::EntryList(EntryList&& el) noexcept
	: entries{ std::move(el.entries) }
{}

EntryList& EntryList::operator=(EntryList&& el) noexcept
{
	entries = std::move(el.entries);
	return *this;
}

size_t EntryList::EntryLength(size_t i) const
{
	size_t entry_name_len = entries[i]->Name(global::lang).size();
	if (!entries[i]->HasOptions())
		return entry_name_len;
	else
		return entry_name_len + entries[i]->CurrentOption(global::lang).size();
}

size_t EntryList::MaxEntryLength(size_t i) const
{
	size_t entry_name_len = entries[i]->Name(global::lang).size();
	if (!entries[i]->HasOptions())
	{
		return entry_name_len;
	}
	else
	{
		size_t max_opt_len = 0u;
		for (const auto& opt : entries[i]->GetOptions(global::lang))
		{
			if (size_t opt_len = opt.size(); opt_len > max_opt_len)
				max_opt_len = opt_len;
		}
		return entry_name_len + max_opt_len;
	}
}

size_t EntryList::LongestEntryLength() const
{
	size_t max_entry_len = 0u;
	for (size_t i = 0u; i < entries.size(); ++i)
	{
		if (size_t curr_entry_len = MaxEntryLength(i); curr_entry_len > max_entry_len)
			max_entry_len = curr_entry_len;
	}
	return max_entry_len;
}

bool EntryList::AnyEntryHasOptions() const
{
	for (size_t i = 0u; i < entries.size(); ++i)
		if (entries[i]->HasOptions()) return true;
	return false;
}


Menu::Menu(const Window& win, Keyboard& kbd, Mouse& mouse,
	Expression title, EntryList el, LayoutDesc ld, MenuPalette mp)
	: kbd{ kbd }
	, mouse{ mouse }
	, title{ title }
	, entryList{ std::move(el) }
	, layoutDesc{ ld }
	, currEntryIndex{ 0 }
	, palette{ mp }
	, parentWin{ win }
{
	CreateMenuWindow();
}

void Menu::Refresh() const
{
	menuPtr->Render();
}

void Menu::Listen()
{
	assert(!listen);
	listen = true;
	Timer timer{};
	while (listen)
	{
		const auto& curr_entry = entryList[currEntryIndex];
		const bool has_opts = entryList[currEntryIndex]->HasOptions();
		timer.Mark();


		// separate function
		auto [w, h] = mouse.GetPos();
		
		w /= menuPtr->GetConsole().FontWidth();
		h /= 2u * menuPtr->GetConsole().FontWidth();

		if (w > menuPtr->Left() && w < menuPtr->Right())
		{
			if (h > menuPtr->Top() + 2u + layoutDesc.VerticalMargin()
				&& h < menuPtr->Bottom() - layoutDesc.VerticalMargin())
			{
				h -= (menuPtr->Top() + 3u + layoutDesc.VerticalMargin());
				GoToEntry(h);
			}
		}

		if (kbd.IsBindingPressedOnce(Controls::Down))
		{
			NextEntry();
		}
		else if (kbd.IsBindingPressedOnce(Controls::Up))
		{
			PrevEntry();
		}

		if (kbd.IsBindingPressedOnce(Controls::Select) && !has_opts)
		{
			OnSelect(curr_entry->Name(global::lang));
			listen = !(curr_entry->IsClosing());
		}

		if (kbd.IsBindingPressedOnce(Controls::Right) && SwitchRight())
		{
			OnSwitch(curr_entry->Name(global::lang), curr_entry->CurrentOption(global::lang));
		}
		else if (kbd.IsBindingPressedOnce(Controls::Left) && SwitchLeft())
		{
			OnSwitch(curr_entry->Name(global::lang), curr_entry->CurrentOption(global::lang));
		}

		Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(int(global::frametime - (timer.Peek()))));
	}
}

void Menu::SetLayoutDesc(LayoutDesc ld)
{
	layoutDesc = ld;
	parentWin.Render();
	CreateMenuWindow();
}

void Menu::SetEntryList(EntryList&& el)
{
	assert(entryList.Size() == 0u);
	entryList = std::move(el);
	parentWin.Render();
	CreateMenuWindow();
}

void Menu::CreateMenuWindow()
{
	//assert(entryList.Size() > 1u);

	// 2 additional lines for title, 2 for box
	ucoord height = ucoord(entryList.Size() + 2u * layoutDesc.VerticalMargin() + 2u + 2u);

	// width = max(max entry length, title length) + 2 margins + 2 chars for box +
	// + (if any entry has options) 2 chars
	ucoord width = ucoord
	(
		std::max(entryList.LongestEntryLength(), title[global::lang].size()) +
		+2u * (size_t)layoutDesc.HorizontalMargin() + 2u +
		+((entryList.AnyEntryHasOptions()) ? 2u : 0u)
	);

	if (width > parentWin.Width() || height > parentWin.Height())
		THROW_MENU_EXCEPTION("Menu constructor", "menu out of bounds");

	auto start_pos = GetWindowStartPos(width, height);

	menuPtr = std::make_unique<Window>(parentWin.GetConsole(), 
		start_pos.first, start_pos.second, width, height);

	Draw();
}

std::pair<Menu::ucoord, Menu::ucoord> Menu::GetWindowStartPos(ucoord width, ucoord height) const
{
	ucoord start_x = 0u;
	ucoord start_y = 0u;
	using Pos = LayoutDesc::Pos;
	switch (layoutDesc.Positioning())
	{
	case Pos::Centered:
		start_x = (parentWin.Width() - width) / 2u;
		start_y = (parentWin.Height() - height) / 2u;
		break;
	case Pos::TopLeft:
		start_x = 0u;
		start_y = 0u;
		break;
	case Pos::BottomLeft:
		start_x = 0u;
		start_y = parentWin.Height() - height;
		break;
	case Pos::TopRight:
		start_x = parentWin.Width() - width;
		start_y = 0u;
		break;
	case Pos::BottomRight:
		start_x = parentWin.Width() - width;
		start_y = parentWin.Height() - height;
		break;
	default:
		start_x = 0u;
		start_y = 0u;
		break;
	}
	return std::make_pair(start_x, start_y);
}

void Menu::Draw() const
{
	DrawTitle();
	DrawBox();
	DrawUpperMargin();

	// write out entries
	for (size_t i = 0u; i < entryList.Size(); ++i)
	{
		DrawEntry(i);
	}
	DrawLowerMargin();
}

void Menu::DrawEntry(size_t i) const
{
	Color text_color = (i == currEntryIndex) ? palette.entrySelectText : palette.entryText;
	Color bg_color = (i == currEntryIndex) ? palette.entrySelectBg : palette.entryBg;

	menuPtr->Write(1u, ucoord(3u + layoutDesc.VerticalMargin() + i),
		EntryToString(i), text_color, bg_color);
}

void Menu::DrawTitle() const
{
	menuPtr->Write(1u, 1u, TitleToString(), palette.title, palette.baseBg);
}

void Menu::DrawUpperMargin() const
{
	for (size_t i = 0u; i < layoutDesc.VerticalMargin(); ++i)
	{
		menuPtr->Write(1u, ucoord(3u + i),
			std::wstring(menuPtr->Width() - 2u, L' '), palette.baseBg, palette.baseBg);
	}
}

void Menu::DrawLowerMargin() const
{
	for (size_t i = 0u; i < layoutDesc.VerticalMargin(); ++i)
	{
		menuPtr->Write(1u, ucoord(3u + layoutDesc.VerticalMargin() + entryList.Size() + i),
			std::wstring(menuPtr->Width() - 2u, L' '), palette.baseBg, palette.baseBg);
	}
}

void Menu::DrawBox() const
{
	menuPtr->DrawBox(palette.box, palette.baseBg);

	// draw title/buttons separator
	menuPtr->WriteChar(0u, 2u, L'├', palette.box, palette.baseBg);
	menuPtr->WriteChar(menuPtr->Width() - 1u, 2u, L'┤', palette.box, palette.baseBg);
	for (ucoord i = 1u; i < menuPtr->Width() - 1u; ++i)
		menuPtr->WriteChar(i, 2u, L'─', palette.box, palette.baseBg);
}

int Menu::GoToEntry(int index)
{
	int prev_entry_index = currEntryIndex;
	currEntryIndex = index;
	if (prev_entry_index != currEntryIndex)
	{
		DrawEntry(prev_entry_index);
		DrawEntry(currEntryIndex);
	}
	return currEntryIndex;
}

int Menu::NextEntry()
{
	int prev_entry_index = currEntryIndex;
	if (int list_size = (int)entryList.Size(); ++currEntryIndex == list_size)
		currEntryIndex = loopEntryList ? 0 : (list_size - 1);
	if (prev_entry_index != currEntryIndex)
	{
		DrawEntry(prev_entry_index);
		DrawEntry(currEntryIndex);
	}
	return currEntryIndex;
}

int Menu::PrevEntry()
{
	int prev_entry_index = currEntryIndex;
	if (--currEntryIndex == -1)
		currEntryIndex = loopEntryList ? ((int)entryList.Size() - 1) : 0;
	if (prev_entry_index != currEntryIndex)
	{
		DrawEntry(prev_entry_index);
		DrawEntry(currEntryIndex);
	}
	return currEntryIndex;
}

bool Menu::SwitchLeft()
{
	const auto& curr_entry = entryList[currEntryIndex];
	const bool switched = (curr_entry->Prev(loopSwitches) != curr_entry->CurrentOptionIndex());
	if (switched) DrawEntry(currEntryIndex);
	return switched;
}

bool Menu::SwitchRight()
{
	const auto& curr_entry = entryList[currEntryIndex];
	const bool switched = (curr_entry->Next(loopSwitches) != curr_entry->CurrentOptionIndex());
	if (switched) DrawEntry(currEntryIndex);
	return switched;
}

std::wstring Menu::TitleToString() const
{
	size_t title_len = title[global::lang].size();
	size_t l_margin = (menuPtr->Width() - 2u - title_len) / 2u;
	size_t r_margin = (menuPtr->Width() - 2u - title_len) - l_margin;

	std::wstring title_str;
	title_str += std::wstring(l_margin, L' ');
	title_str += title[global::lang];
	title_str += std::wstring(r_margin, L' ');
	
	return title_str;
}

std::wstring Menu::EntryToString(size_t i) const
{
	std::wstring entry;
	size_t entry_len;
	
	entry_len = entryList.EntryLength(i);
	if (entryList[i]->HasOptions()) entry_len += 2u; // for ": "

	size_t l_margin = (menuPtr->Width() - 2u - entry_len) / 2u;
	size_t r_margin = (menuPtr->Width() - 2u - entry_len) - l_margin;

	entry += std::wstring(l_margin, L' ');
	entry += entryList[i]->Name(global::lang);
	if (entryList[i]->HasOptions())
	{
		entry += L": ";
		entry += entryList[i]->CurrentOption(global::lang);
	}
	entry += std::wstring(r_margin, L' ');
	
	if (i == currEntryIndex && entryList[i]->HasOptions() && layoutDesc.HorizontalMargin() > 0u)
	{
		int num_of_opts = (int)entryList[i]->GetOptions(global::lang).size();
		if (loopSwitches || entryList[i]->CurrentOptionIndex() != 0)
			entry.front() = L'<';
		if (loopSwitches || entryList[i]->CurrentOptionIndex() != num_of_opts - 1)
			entry.back() = L'>';
	}
	return entry;
}