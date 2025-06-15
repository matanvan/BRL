#pragma once

#include "Common.hpp"

struct RegKeyData
{
	using clock = std::chrono::file_clock;
	clock::time_point m_last_write_time;
	std::wstring m_class_name;
	uint32_t m_subkeys_count;
	// In unicode characters, not including null terminator
	uint32_t m_max_subkey_name_len;
	uint32_t m_values_count;
	// In unicode characters, not including null terminator
	uint32_t m_max_value_name_len;
	// In bytes
	uint32_t m_max_value_data_size;
	// In unicode characters, not including null terminator
	uint32_t m_max_class_name_len;
};

struct RegValueData
{

};

using RegData = std::variant<RegKeyData, RegValueData>;
