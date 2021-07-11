#include "Menu.h"
#include "Timer.h"

Switch::Switch(u8str_view name, std::vector<u8str_view> options)
	: Entry{ name }
	, opts{ options }
	, currOpt{ 0 }
{
	if (options.empty())
		THROW_MENU_EXCEPTION("Switch constructor", "empty options not allowed");
}

std::unique_ptr<Button> MakeButton(u8str_view name, bool isClosing)
{
	return std::make_unique<Button>(name, isClosing);
}
std::unique_ptr<Switch> MakeSwitch(u8str_view name, Entry::Options opts)
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
	size_t entry_name_len = count_codepoints(entries[i]->Name());
	if (!EntryHasOptions(i))
		return entry_name_len;
	else
		return entry_name_len + count_codepoints(entries[i]->CurrentOption());
}

size_t EntryList::MaxEntryLength(size_t i) const
{
	size_t entry_name_len = count_codepoints(entries[i]->Name());
	if (!EntryHasOptions(i))
	{
		return entry_name_len;
	}
	else
	{
		size_t max_opt_len = 0u;
		for (const auto& opt : entries[i]->GetOptions())
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
		if (EntryHasOptions(i)) return true;
	return false;
}


Menu::Menu(const Window& win, Keyboard& kbd,
	u8str_view title, EntryList el, LayoutDesc ld, MenuPalette mp)
	: kbd{ kbd }
	, title{ title }
	, entryList{ std::move(el) }
	, layoutDesc{ ld }
	, currEntryIndex{ 0 }
	, palette{ mp }
{
	assert(entryList.Size() > 1u);

	// 1 additional line for title, 2 for box
	ucoord height = ucoord(entryList.Size() + layoutDesc.TitleGap() + 1u + 2u); 
	
	// width = max(max entry length, title length) + 2 margins + 2 chars for box +
	// + (if any entry has options) 2 chars
	ucoord width = ucoord
	(
		std::max(entryList.LongestEntryLength(), count_codepoints(title)) +
		+ 2u * (size_t)layoutDesc.MarginSize() + 2u +
		+ ((entryList.AnyEntryHasOptions()) ? 2u : 0u)
	);

	if (width > win.Width() || height > win.Height())
		THROW_MENU_EXCEPTION("Menu constructor", "menu out of bounds");

	auto start_pos = GetWindowStartPos(win, width, height);
	
	menuPtr = std::make_unique<Window>(start_pos.first, start_pos.second, width, height);
	
	Draw();
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
		timer.Mark();
		if (kbd.IsKeyPressedOnce(VK_DOWN))
		{
			NextEntry();
		}
		else if (kbd.IsKeyPressedOnce(VK_UP))
		{
			PrevEntry();
		}

		if (kbd.IsKeyPressedOnce(VK_RETURN))
		{
			OnSelect();
			listen = !(entryList[currEntryIndex]->IsClosing());
		}

		if (kbd.IsKeyPressedOnce(VK_RIGHT) && SwitchRight())
		{
			OnSwitchRight();
		}
		else if (kbd.IsKeyPressedOnce(VK_LEFT) && SwitchLeft())
		{
			OnSwitchLeft();
		}

		Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(int(16.6f - (timer.Peek()))));
	}
}

std::pair<Menu::ucoord, Menu::ucoord> Menu::GetWindowStartPos(const Window& win, 
	ucoord width, ucoord height) const
{
	ucoord start_x = 0u;
	ucoord start_y = 0u;
	using Pos = LayoutDesc::Pos;
	switch (layoutDesc.Positioning())
	{
	case Pos::Centered:
		start_x = (win.Width() - width) / 2u;
		start_y = (win.Height() - height) / 2u;
		break;
	case Pos::TopLeft:
		start_x = 0u;
		start_y = 0u;
		break;
	case Pos::BottomLeft:
		start_x = 0u;
		start_y = win.Height() - height;
		break;
	case Pos::TopRight:
		start_x = win.Width() - width;
		start_y = 0u;
		break;
	case Pos::BottomRight:
		start_x = win.Width() - width;
		start_y = win.Height() - height;
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
	DrawTitleGap();

	// write out entries
	for (size_t i = 0u; i < entryList.Size(); ++i)
	{
		DrawEntry(i);
	}

	DrawBox();
}

void Menu::DrawEntry(size_t i) const
{
	Color text_color = (i == currEntryIndex) ? palette.selectText : palette.baseText;
	Color bg_color = (i == currEntryIndex) ? palette.selectBg : palette.baseBg;

	menuPtr->Write(1u, ucoord(2u + layoutDesc.TitleGap() + i),
		EntryToString(i), text_color, bg_color);
}

void Menu::DrawTitle() const
{
	menuPtr->Write(1u, 1u, TitleToString(), palette.title, palette.titleBg);
}

void Menu::DrawTitleGap() const
{
	for (size_t i = 0u; i < layoutDesc.TitleGap(); ++i)
	{
		menuPtr->Write(1u, ucoord(2u + i),
			u8str(menuPtr->Width() - 2u, u8' '), palette.baseText, palette.baseBg);
	}
}

void Menu::DrawBox() const
{
	menuPtr->DrawBox(palette.box, palette.baseBg);
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
	size_t title_len = count_codepoints(title);
	size_t l_margin = (menuPtr->Width() - 2u - title_len) / 2u;
	size_t r_margin = (menuPtr->Width() - 2u - title_len) - l_margin;

	u8str title_str;
	title_str += u8str(l_margin, u8' ');
	title_str += title;
	title_str += u8str(r_margin, u8' ');
	
	return title_str;
}

u8str Menu::EntryToString(size_t i) const
{
	u8str entry;
	size_t entry_len;
	
	entry_len = entryList.EntryLength(i);
	if (entryList.EntryHasOptions(i)) entry_len += 2u; // for ": "

	size_t l_margin = (menuPtr->Width() - 2u - entry_len) / 2u;
	size_t r_margin = (menuPtr->Width() - 2u - entry_len) - l_margin;

	entry += u8str(l_margin, u8' ');
	entry += entryList[i]->Name();
	if (entryList.EntryHasOptions(i))
	{
		entry += u8": ";
		entry += entryList[i]->CurrentOption();
	}
	entry += u8str(r_margin, u8' ');
	
	if (i == currEntryIndex && entryList.EntryHasOptions(i))
	{
		int num_of_opts = (int)entryList[i]->GetOptions().size();
		if (loopSwitches || entryList[i]->CurrentOptionIndex() != 0)
			entry.front() = u8'<';
		if (loopSwitches || entryList[i]->CurrentOptionIndex() != num_of_opts - 1)
			entry.back() = u8'>';
	}
	return entry;
}