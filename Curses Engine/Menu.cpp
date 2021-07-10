#include "Menu.h"

int EntryList::Options::CurrentOptionIndex() const { return currOption; }

u8str_view EntryList::Options::CurrentOption() const
{
	if (currOption == -1)
		THROW_MENU_EXCEPTION("CurrentOption", "options empty");
	return options[currOption];
}

int EntryList::Options::NextOption(bool loop) const
{
	if (currOption != -1 && ++currOption == options.size())
		currOption = loop? 0 : (int)options.size() - 1;
	return currOption;
}

int EntryList::Options::PrevOption(bool loop) const
{
	if (currOption != -1 && --currOption == -1)
		currOption = loop ? (int)options.size() - 1 : 0;
	return currOption;
}


EntryList::EntryList(std::vector<EntryList::Entry> entries)
	: entries{ std::move(entries) }
{
	entries.shrink_to_fit();
}

size_t EntryList::EntryLength(size_t i) const
{
	size_t entry_name_len = count_codepoints(EntryName(i));
	if (!EntryHasOptions(i))
		return entry_name_len;
	else
		return entry_name_len + count_codepoints(EntryOptions(i).CurrentOption());
}

size_t EntryList::MaxEntryLength(size_t i) const
{
	size_t entry_name_len = count_codepoints(EntryName(i));
	if (!EntryHasOptions(i))
	{
		return entry_name_len;
	}
	else
	{
		size_t max_opt_len = 0u;
		for (const auto& opt : EntryOptions(i).List())
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


Menu::Menu(const Window& win, const Keyboard& kbd,
	u8str_view title, EntryList el, LayoutDesc ld)
	: kbd{ kbd }
	, title{ title }
	, entryList{ el }
	, layoutDesc{ ld }
	, currEntry{ 0u }
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

	ucoord start_x = 0u;
	ucoord start_y = 0u;

	using Pos = LayoutDesc::Pos;
	switch (layoutDesc.Positioning())
	{
	case Pos::Centered:
		start_x = (win.Width() - width) / 2u;
		start_y = (win.Height() - height) / 2u;
		break;
	// TODO: other cases
	default:
		start_x = 0u;
		start_y = 0u;
		break;
	}
	menuPtr = std::make_unique<Window>(start_x, start_y, width, height);
	menuPtr->Write(1u, 1u, TitleToString(), titleCol, titleBg);
	
	// fill title gap
	for (size_t i = 0u; i < layoutDesc.TitleGap(); ++i)
	{
		menuPtr->Write(1u, ucoord(2u + i),
			u8str(menuPtr->Width() - 2u, u8' '), baseText, baseBg);
	}

	// write out entries
	for (size_t i = 0u; i < entryList.Size(); ++i)
	{
		Color text_color = (i == currEntry) ? selectText : baseText;
		Color bg_color = (i == currEntry) ? selectBg : baseBg;

		menuPtr->Write(1u, ucoord(2u + layoutDesc.TitleGap() + i),
			EntryToString(i), text_color, bg_color);
	}
	menuPtr->DrawBox(boxFg, baseBg);
}

void Menu::Show() const
{
	menuPtr->Touch();
	menuPtr->Refresh();
}


u8str Menu::TitleToString()
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

u8str Menu::EntryToString(size_t i)
{
	u8str entry;
	size_t entry_len;
	
	entry_len = entryList.EntryLength(i);
	if (entryList.EntryHasOptions(i)) entry_len += 2u; // for ": "

	size_t l_margin = (menuPtr->Width() - 2u - entry_len) / 2u;
	size_t r_margin = (menuPtr->Width() - 2u - entry_len) - l_margin;

	entry += u8str(l_margin, u8' ');

	entry += entryList.EntryName(i);
	if (entryList.EntryHasOptions(i))
	{
		entry += u8": ";
		const EntryList::Options& opts = entryList.EntryOptions(i);
		entry += opts.CurrentOption();
	}
	entry += u8str(r_margin, u8' ');

	return entry;
}