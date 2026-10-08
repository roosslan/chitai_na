#include "quiz_logic.h"
#include "resource.h"
#include "resource_lists.h"

#include <algorithm>
#include <cwchar>
#include <stdexcept>

std::vector<uint8_t> get_unique_numbers(std::mt19937& gen, size_t count, int max_value)
{
	if (max_value < 1 || max_value > UINT8_MAX || count > static_cast<size_t>(max_value))
		throw std::invalid_argument("get_unique_numbers: count exceeds range");

	std::uniform_int_distribution<> dist(1, max_value);

	std::vector<uint8_t> numbers;
	numbers.reserve(count);

	while (numbers.size() < count)
	{
		const auto num = static_cast<uint8_t>(dist(gen));
		if (std::ranges::find(numbers, num) == numbers.end())
			numbers.push_back(num);
	}

	return numbers;
}

uint8_t pick_guess_symbol(std::mt19937& gen, const std::vector<uint8_t>& symbols)
{
	if (symbols.empty())
		throw std::invalid_argument("pick_guess_symbol: empty set");

	std::uniform_int_distribution<size_t> dist(0, symbols.size() - 1);
	return symbols[dist(gen)];
}

uint16_t str_to_uid(int id_type, const wchar_t* resource_name)
{
	#define WIDEN2(s) L ## s
	#define WIDEN(s) WIDEN2(s)
	#define X(id) if (_wcsicmp(resource_name, WIDEN(#id)) == 0) return id;
	switch (id_type)
	{
	case IDB: RESOURCE_LIST_BITMAP;
		break;
	case IDW: RESOURCE_LIST_WRITTEN;
		break;
	case IDR: RESOURCE_LIST_READ;
		break;
	case IDE: RESOURCE_LIST_ENGLISH;
		break;
	case IDRU: RESOURCE_LIST_RUSSIAN;
		break;
	case IDP: RESOURCE_LIST_PRONOUN;
		break;
	case IDREX: RESOURCE_LIST_DESCRIPTION;
		break;
	}
	#undef X
	#undef WIDEN
	#undef WIDEN2

	return 0;
}
