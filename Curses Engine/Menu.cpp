#include "Menu.h"
#include "Timer.h"
#include "GlobalParameters.h"

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
	size_t entry_name_len = count_codepoints(entries[i]->Name(global.lang));
	if (!entries[i]->HasOptions())
		return entry_name_len;
	else
		return entry_name_len + count_codepoints(entries[i]->CurrentOption(global.lang));
}

size_t EntryList::MaxEntryLength(size_t i) const
{
	size_t entry_name_len = count_codepoints(entries[i]->Name(global.lang));
	if (!entries[i]->HasOptions())
	{
		return entry_name_len;
	}
	else
	{
		size_t max_opt_len = 0u;
		for (const auto& opt : entries[i]->GetOptions(global.lang))
		{
			if (size_t opt_len = count_codepoints(opt); opt_len > max_opt_len)
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


Menu::Menu(const Window& win, Keyboard& kbd,
	Expression title, EntryList el, LayoutDesc ld, MenuPalette mp)
	: kbd{ kbd }
	, title{ title }
	, entryList{ std::move(el) }
	, layoutDesc{ ld }
	, currEntryIndex{ 0 }
	, palette{ mp }
	, parentWin{ win }
{
	CreateMenuWindow();
}

void Menu::Touch() const
{
	menuPtr->Touch();
}

void Menu::Refresh() const
{
	menuPtr->Refresh();
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
			OnSelect(curr_entry->Name(global.lang));
			listen = !(curr_entry->IsClosing());
		}

		if (kbd.IsBindingPressedOnce(Controls::Right) && SwitchRight())
		{
			OnSwitch(curr_entry->Name(global.lang), curr_entry->CurrentOption(global.lang));
		}
		else if (kbd.IsBindingPressedOnce(Controls::Left) && SwitchLeft())
		{
			OnSwitch(curr_entry->Name(global.lang), curr_entry->CurrentOption(global.lang));
		}

		Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(int(global.frametime - (timer.Peek()))));
	}
}

void Menu::SetLayoutDesc(LayoutDesc ld)
{
	layoutDesc = ld;
	parentWin.Touch();
	parentWin.Refresh();
	CreateMenuWindow();
}

void Menu::CreateMenuWindow()
{
	assert(entryList.Size() > 1u);

	// 2 additional lines for title, 2 for box
	ucoord height = ucoord(entryList.Size() + 2u * layoutDesc.VerticalMargin() + 2u + 2u);

	// width = max(max entry length, title length) + 2 margins + 2 chars for box +
	// + (if any entry has options) 2 chars
	ucoord width = ucoord
	(
		std::max(entryList.LongestEntryLength(), count_codepoints(title[global.lang])) +
		+2u * (size_t)layoutDesc.HorizontalMargin() + 2u +
		+((entryList.AnyEntryHasOptions()) ? 2u : 0u)
	);

	if (width > parentWin.Width() || height > parentWin.Height())
		THROW_MENU_EXCEPTION("Menu constructor", "menu out of bounds");

	auto start_pos = GetWindowStartPos(width, height);

	menuPtr = std::make_unique<Window>(start_pos.first, start_pos.second, width, height);

	separatorLine = u8"";
	separatorLine.reserve(std::size(u8"├") - 1 + (menuPtr->Width() - 2) * (std::size(u8"─") - 1) + std::size(u8"┤") - 1 + 1);
	separatorLine += u8"├";
	for (size_t i = 1u; i < menuPtr->Width() - 1; ++i)
	{
		separatorLine += u8"─";
	}
	separatorLine += u8"┤";

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
			u8str(menuPtr->Width() - 2u, u8' '), palette.baseBg, palette.baseBg);
	}
}

void Menu::DrawLowerMargin() const
{
	for (size_t i = 0u; i < layoutDesc.VerticalMargin(); ++i)
	{
		menuPtr->Write(1u, ucoord(3u + layoutDesc.VerticalMargin() + entryList.Size() + i),
			u8str(menuPtr->Width() - 2u, u8' '), palette.baseBg, palette.baseBg);
	}
}

void Menu::DrawBox() const
{
	menuPtr->DrawBox(palette.box, palette.baseBg);

	menuPtr->Write(0u, ucoord(2u),
		separatorLine, palette.box, palette.baseBg);
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

u8str Menu::TitleToString() const
{
	size_t title_len = count_codepoints(title[global.lang]);
	size_t l_margin = (menuPtr->Width() - 2u - title_len) / 2u;
	size_t r_margin = (menuPtr->Width() - 2u - title_len) - l_margin;

	u8str title_str;
	title_str += u8str(l_margin, u8' ');
	title_str += title[global.lang];
	title_str += u8str(r_margin, u8' ');
	
	return title_str;
}

u8str Menu::EntryToString(size_t i) const
{
	u8str entry;
	size_t entry_len;
	
	entry_len = entryList.EntryLength(i);
	if (entryList[i]->HasOptions()) entry_len += 2u; // for ": "

	size_t l_margin = (menuPtr->Width() - 2u - entry_len) / 2u;
	size_t r_margin = (menuPtr->Width() - 2u - entry_len) - l_margin;

	entry += u8str(l_margin, u8' ');
	entry += entryList[i]->Name(global.lang);
	if (entryList[i]->HasOptions())
	{
		entry += u8": ";
		entry += entryList[i]->CurrentOption(global.lang);
	}
	entry += u8str(r_margin, u8' ');
	
	if (i == currEntryIndex && entryList[i]->HasOptions() && layoutDesc.HorizontalMargin() > 0u)
	{
		int num_of_opts = (int)entryList[i]->GetOptions(global.lang).size();
		if (loopSwitches || entryList[i]->CurrentOptionIndex() != 0)
			entry.front() = u8'<';
		if (loopSwitches || entryList[i]->CurrentOptionIndex() != num_of_opts - 1)
			entry.back() = u8'>';
	}
	return entry;
}