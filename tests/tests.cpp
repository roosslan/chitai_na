// Тесты chitai_na: логика выбора вопросов и целостность ресурсов собранного exe.
//
// Запуск: chitai_na_tests.exe [--exe <путь к chitai_na.exe>] [--mp4 <директория mp4>]
// Без --exe и --mp4 соответствующие тесты пропускаются.

#define NOMINMAX
#include <windows.h>

#include "../quiz_logic.h"
#include "../resource.h"
#include "../Version.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#pragma comment(lib, "version.lib")

namespace
{
	struct test_context
	{
		std::wstring exe_path;
		std::wstring mp4_dir;
	};

	struct test_case
	{
		const char* name;
		std::function<void(const test_context&)> body;
	};

	/* Исключение для пропуска теста, когда не передан нужный аргумент */
	struct skip_test : std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	struct check_failed : std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	#define CHECK(cond) \
		do { if (!(cond)) throw check_failed(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": CHECK(" #cond ")"); } while (0)

	struct resource_prefix
	{
		int type;
		const wchar_t* prefix;
	};

	constexpr resource_prefix all_prefixes[] = {
		{ IDB, L"IDB_BITMAP" },
		{ IDW, L"IDW_RADICAL" },
		{ IDR, L"IDR_RADICAL" },
		{ IDE, L"IDE_RADICAL" },
		{ IDRU, L"IDRU_RADICAL" },
		{ IDP, L"IDP_RADICAL" },
		{ IDREX, L"IDREX_RADICAL" },
	};

	std::wstring resource_name(const wchar_t* prefix, int number)
	{
		return prefix + std::to_wstring(number);
	}

	/* Модуль exe, загруженный как данные (без выполнения кода) */
	class resource_module
	{
	public:
		explicit resource_module(const std::wstring& path)
			: module_(LoadLibraryExW(path.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))
		{
			if (!module_)
				throw check_failed("cannot load resources from exe");
		}
		~resource_module() { FreeLibrary(module_); }
		resource_module(const resource_module&) = delete;
		resource_module& operator=(const resource_module&) = delete;

		HMODULE get() const { return module_; }

	private:
		HMODULE module_;
	};

	const test_context& require_exe(const test_context& ctx)
	{
		if (ctx.exe_path.empty())
			throw skip_test("--exe not specified");
		return ctx;
	}

	// ---------------------------------------------------------------- логика

	void unique_numbers_are_valid(const test_context&)
	{
		std::mt19937 gen(1);
		for (int i = 0; i < 20000; ++i)
		{
			const auto numbers = get_unique_numbers(gen);
			CHECK(numbers.size() == CHOICE_COUNT);
			for (const auto n : numbers)
				CHECK(n >= 1 && n <= RADICAL_COUNT);
			CHECK(std::set<uint8_t>(numbers.begin(), numbers.end()).size() == numbers.size());
		}
	}

	void unique_numbers_cover_whole_range(const test_context&)
	{
		std::mt19937 gen(2);
		std::set<uint8_t> seen;
		for (int i = 0; i < 20000; ++i)
			for (const auto n : get_unique_numbers(gen))
				seen.insert(n);
		CHECK(seen.size() == static_cast<size_t>(RADICAL_COUNT));
	}

	void unique_numbers_full_permutation(const test_context&)
	{
		std::mt19937 gen(3);
		const auto numbers = get_unique_numbers(gen, 10, 10);
		CHECK(std::set<uint8_t>(numbers.begin(), numbers.end()).size() == 10);
	}

	void unique_numbers_reject_invalid_arguments(const test_context&)
	{
		std::mt19937 gen(4);
		const auto throws = [&](size_t count, int max_value)
		{
			try { get_unique_numbers(gen, count, max_value); }
			catch (const std::invalid_argument&) { return true; }
			return false;
		};
		CHECK(throws(5, 4));
		CHECK(throws(1, 0));
		CHECK(throws(1, 300));
	}

	void guess_symbol_is_from_set(const test_context&)
	{
		std::mt19937 gen(5);
		const std::vector<uint8_t> symbols = { 10, 20, 30, 40 };
		std::set<uint8_t> seen;
		for (int i = 0; i < 1000; ++i)
		{
			const auto s = pick_guess_symbol(gen, symbols);
			CHECK(std::ranges::find(symbols, s) != symbols.end());
			seen.insert(s);
		}
		CHECK(seen.size() == symbols.size());
	}

	void guess_symbol_rejects_empty_set(const test_context&)
	{
		std::mt19937 gen(6);
		bool thrown = false;
		try { pick_guess_symbol(gen, {}); }
		catch (const std::invalid_argument&) { thrown = true; }
		CHECK(thrown);
	}

	void str_to_uid_finds_all_resources(const test_context&)
	{
		std::set<uint16_t> ids;
		for (const auto& p : all_prefixes)
			for (int n = 1; n <= RADICAL_COUNT; ++n)
			{
				const auto id = str_to_uid(p.type, resource_name(p.prefix, n).c_str());
				CHECK(id != 0);
				ids.insert(id);
			}
		/* Все ID различны */
		CHECK(ids.size() == std::size(all_prefixes) * RADICAL_COUNT);
	}

	void str_to_uid_ignores_case(const test_context&)
	{
		CHECK(str_to_uid(IDW, L"idw_radical1") == str_to_uid(IDW, L"IDW_RADICAL1"));
		CHECK(str_to_uid(IDB, L"idb_Bitmap214") == str_to_uid(IDB, L"IDB_BITMAP214"));
	}

	void str_to_uid_rejects_unknown(const test_context&)
	{
		CHECK(str_to_uid(IDW, L"IDW_RADICAL0") == 0);
		CHECK(str_to_uid(IDW, L"IDW_RADICAL215") == 0);
		CHECK(str_to_uid(IDB, L"IDW_RADICAL1") == 0);	/* имя другого типа */
		CHECK(str_to_uid(99, L"IDW_RADICAL1") == 0);	/* неизвестный тип */
	}

	// ------------------------------------------------------- ресурсы и файлы

	void exe_has_all_strings(const test_context& ctx)
	{
		resource_module module(require_exe(ctx).exe_path);
		for (const auto& p : all_prefixes)
		{
			if (p.type == IDB)
				continue;
			for (int n = 1; n <= RADICAL_COUNT; ++n)
			{
				const auto id = str_to_uid(p.type, resource_name(p.prefix, n).c_str());
				const wchar_t* text = nullptr;
				/* При нулевом размере буфера LoadStringW возвращает указатель на строку в ресурсах */
				const int len = LoadStringW(module.get(), id, reinterpret_cast<LPWSTR>(&text), 0);
				CHECK(len > 0);
			}
		}
		const wchar_t* license = nullptr;
		CHECK(LoadStringW(module.get(), IDS_LICENSE, reinterpret_cast<LPWSTR>(&license), 0) > 0);
	}

	void exe_has_all_bitmaps(const test_context& ctx)
	{
		resource_module module(require_exe(ctx).exe_path);
		for (int n = 1; n <= RADICAL_COUNT; ++n)
		{
			const auto id = str_to_uid(IDB, resource_name(L"IDB_BITMAP", n).c_str());
			CHECK(FindResourceW(module.get(), MAKEINTRESOURCEW(id), MAKEINTRESOURCEW(2) /* RT_BITMAP */) != nullptr);
		}
	}

	void exe_version_matches_version_h(const test_context& ctx)
	{
		const auto& path = require_exe(ctx).exe_path;
		DWORD handle = 0;
		const DWORD size = GetFileVersionInfoSizeW(path.c_str(), &handle);
		CHECK(size > 0);
		std::vector<BYTE> data(size);
		CHECK(GetFileVersionInfoW(path.c_str(), 0, size, data.data()));

		VS_FIXEDFILEINFO* info = nullptr;
		UINT info_len = 0;
		CHECK(VerQueryValueW(data.data(), L"\\", reinterpret_cast<void**>(&info), &info_len) && info);

		const DWORD expected_ms = MAKELONG(FILE_MINOR, FILE_MAJOR);
		const DWORD expected_ls = MAKELONG(FILE_REVISION, FILE_BUILD);
		CHECK(info->dwFileVersionMS == expected_ms && info->dwFileVersionLS == expected_ls);
		CHECK(info->dwProductVersionMS == expected_ms && info->dwProductVersionLS == expected_ls);
	}

	void mp4_exists_for_every_radical(const test_context& ctx)
	{
		if (ctx.mp4_dir.empty())
			throw skip_test("--mp4 not specified");
		for (int n = 1; n <= RADICAL_COUNT; ++n)
		{
			const std::filesystem::path file = std::filesystem::path(ctx.mp4_dir) / (std::to_wstring(n) + L".mp4");
			CHECK(std::filesystem::is_regular_file(file) && std::filesystem::file_size(file) > 0);
		}
	}

	/* Числовые поля версии совпадают со строкой FILE_VERSIONSTRING */
	void version_h_is_consistent(const test_context&)
	{
		const std::string expected = std::to_string(FILE_MAJOR) + "." + std::to_string(FILE_MINOR) + "." +
			std::to_string(FILE_BUILD) + "." + std::to_string(FILE_REVISION);
		CHECK(std::string(FILE_VERSIONSTRING) == expected);
	}
}

int wmain(int argc, wchar_t* argv[])
{
	test_context ctx;
	for (int i = 1; i + 1 < argc; i += 2)
	{
		const std::wstring key = argv[i];
		if (key == L"--exe")
			ctx.exe_path = argv[i + 1];
		else if (key == L"--mp4")
			ctx.mp4_dir = argv[i + 1];
	}

	const test_case tests[] = {
		{ "unique_numbers_are_valid", unique_numbers_are_valid },
		{ "unique_numbers_cover_whole_range", unique_numbers_cover_whole_range },
		{ "unique_numbers_full_permutation", unique_numbers_full_permutation },
		{ "unique_numbers_reject_invalid_arguments", unique_numbers_reject_invalid_arguments },
		{ "guess_symbol_is_from_set", guess_symbol_is_from_set },
		{ "guess_symbol_rejects_empty_set", guess_symbol_rejects_empty_set },
		{ "str_to_uid_finds_all_resources", str_to_uid_finds_all_resources },
		{ "str_to_uid_ignores_case", str_to_uid_ignores_case },
		{ "str_to_uid_rejects_unknown", str_to_uid_rejects_unknown },
		{ "version_h_is_consistent", version_h_is_consistent },
		{ "exe_has_all_strings", exe_has_all_strings },
		{ "exe_has_all_bitmaps", exe_has_all_bitmaps },
		{ "exe_version_matches_version_h", exe_version_matches_version_h },
		{ "mp4_exists_for_every_radical", mp4_exists_for_every_radical },
	};

	int passed = 0, failed = 0, skipped = 0;
	for (const auto& t : tests)
	{
		try
		{
			t.body(ctx);
			std::printf("[ PASS ] %s\n", t.name);
			++passed;
		}
		catch (const skip_test& e)
		{
			std::printf("[ SKIP ] %s (%s)\n", t.name, e.what());
			++skipped;
		}
		catch (const std::exception& e)
		{
			std::printf("[ FAIL ] %s: %s\n", t.name, e.what());
			++failed;
		}
	}

	std::printf("\n%d passed, %d failed, %d skipped\n", passed, failed, skipped);
	return failed == 0 ? 0 : 1;
}
