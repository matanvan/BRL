#pragma once

#include "Common.hpp"

struct RegKeyData
{
	std::chrono::system_clock::time_point m_last_write_time;
	std::wstring m_path;
	std::wstring m_class_name;
	uint32_t m_subkeys_count;
	uint32_t m_max_subkey_name_size_bytes;
	uint32_t m_values_count;
	uint32_t m_max_value_name_size_bytes;
	uint32_t m_max_value_size_bytes;
	uint32_t m_max_class_name_size_bytes;
};

struct RegValueData
{

};

using RegData = std::variant<RegKeyData, RegValueData>;
