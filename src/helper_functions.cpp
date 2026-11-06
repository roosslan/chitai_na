#include "stdafx.h"
#include "helper_functions.h"

#include <shlobj.h>
#include <system_error>

#pragma comment(lib, "shell32.lib")

CString get_exe_dir() {
	std::vector<wchar_t> buffer(MAX_PATH);
	for (;;) {
		const DWORD len = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
		if (len == 0)
			throw std::system_error(GetLastError(), std::system_category(), "GetModuleFileNameW");
		if (len < buffer.size()) {
			const CString exe_path(buffer.data(), static_cast<int>(len));
			const int pos = exe_path.ReverseFind(L'\\');
			return pos >= 0 ? exe_path.Left(pos) : exe_path;
		}
		buffer.resize(buffer.size() * 2);
	}
}

namespace
{
	std::mutex log_mutex;
	std::ofstream log_file;
}

void r_logger::init_logging() {
	/* Каталог пользователя доступен для записи независимо от места установки. */
	PWSTR local_app_data = nullptr;
	const HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &local_app_data);
	if (FAILED(result)) {
		CoTaskMemFree(local_app_data);
		OutputDebugStringW(L"chitai_na: cannot locate LocalAppData; file logging disabled\n");
		return;
	}
	const std::filesystem::path log_path = std::filesystem::path(local_app_data) / L"chitai_na" / L"logs";
	CoTaskMemFree(local_app_data);
	std::error_code error;
	std::filesystem::create_directories(log_path, error);
	if (error) {
		OutputDebugStringW(L"chitai_na: cannot create log directory; file logging disabled\n");
		return;
	}
	const CString log_dir(log_path.c_str());

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString file_name;
	file_name.Format(L"%s\\chitai_na_%04u-%02u-%02u_%02u-%02u-%02u.log", log_dir.GetString(),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	{
		std::lock_guard lock(log_mutex);
		log_file.open(file_name.GetString(), std::ios_base::app);
		if (!log_file.is_open()) {
			OutputDebugStringW(L"chitai_na: cannot open log file; file logging disabled\n");
			return;
		}
	}

	LOG_SAVE << "chitai_na's r_logger init";
}

void r_logger::write_line(const std::string& message) {
	SYSTEMTIME st;
	GetLocalTime(&st);

	std::lock_guard lock(log_mutex);
	if (!log_file.is_open())
		return;

	log_file << std::format("[{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}] ",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds)
		<< message << std::endl;
}

std::string r_logger::path_to_filename(std::string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}
