#include "RegNode.hpp"

RegNode RegNode::subkey(const std::wstring& subpath) const
{
	if (std::holds_alternative<Value>(m_node))
	{
		throw BRLException(BRLStatus::REGNODE_SUBKEY_VALUE_HAS_NO_SUBKEYS);
	}
	
	return RegNode(std::get<Key>(m_node).subkey(subpath));
}

std::wstring RegNode::path() const
{
	if (std::holds_alternative<Key>(m_node))
	{
		return std::get<Key>(m_node).path();
	}

	return std::get<Value>(m_node).path();
}

RegData RegNode::info() const
{
	if (std::holds_alternative<Key>(m_node))
	{
		return std::get<Key>(m_node).info();
	}
	else
	{
		throw std::exception("TODO: implement");
	}
	
}

RegNode::Iter RegNode::begin() const
{
	if (std::holds_alternative<Key>(m_node))
	{
		return Iter(std::get<Key>(m_node).clone());
	}

	return Iter();
}

HKEY RegNode::Key::get() const
{
	if (!m_subpath)
	{
		return m_key.get();
	}
	else
	{
		HKEY hkey = nullptr;
		const auto lstatus = RegOpenKeyExW(
			m_key.get(),
			m_subpath->c_str(),
			0,
			KEY_READ,
			&hkey
		);
		if (ERROR_SUCCESS != lstatus)
		{
			throw BRLException(BRLStatus::REGNODE_KEY_GET_REGOPENKEYEXW_FAILED, lstatus);
		}

		m_key = AutoCloseRegKey(hkey);
		m_subpath = std::nullopt;
		return hkey;
	}
}

RegKeyData RegNode::Key::info() const
{
	DWORD class_name_size = 0;
	auto lstatus = RegQueryInfoKeyW(
		get(),
		nullptr,
		&class_name_size,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	);
	if ((ERROR_MORE_DATA != lstatus) && (ERROR_SUCCESS != lstatus))
	{
		throw BRLException(BRLStatus::REGNODE_KEY_INFO_REGQUERYINFOKEYW_FAILED_TO_GET_CLASS_NAME_LENGTH, lstatus);
	}

	Buffer class_name((class_name_size + 1) * sizeof(wchar_t));
	class_name_size = static_cast<uint32_t>(class_name.capacity() / sizeof(wchar_t));
	DWORD subkey_count = 0;
	DWORD max_subkey_name_len = 0;
	DWORD max_class_name_len = 0;
	DWORD values_count = 0;
	DWORD max_value_name_len = 0;
	DWORD max_value_data_size = 0;
	ULARGE_INTEGER last_write_time = { 0 };
	lstatus = RegQueryInfoKeyW(
		get(),
		reinterpret_cast<wchar_t*>(class_name.data()),
		&class_name_size,
		nullptr,
		&subkey_count,
		&max_subkey_name_len,
		&max_class_name_len,
		&values_count,
		&max_value_name_len,
		&max_value_data_size,
		nullptr,
		reinterpret_cast<FILETIME*>(&last_write_time)
	);
	if (ERROR_SUCCESS != lstatus)
	{
		throw BRLException(BRLStatus::REGNODE_KEY_INFO_REGQUERYINFOKEYW_FAILED, lstatus);
	}

	return RegKeyData{
		.m_last_write_time = RegKeyData::clock::time_point(RegKeyData::clock::duration(last_write_time.QuadPart)),
		.m_class_name = std::wstring(reinterpret_cast<wchar_t*>(class_name.data()), class_name_size),
		.m_subkeys_count = subkey_count,
		.m_max_subkey_name_len = max_subkey_name_len,
		.m_values_count = values_count,
		.m_max_value_name_len = max_value_name_len,
		.m_max_value_data_size = max_value_data_size,
		.m_max_class_name_len = max_class_name_len
	};
}

std::wstring RegNode::Key::path() const
{
	// This function does not use the Key::get() method deliberately as to not open an unnecessary handle
	std::wstring key_path;
	switch (reinterpret_cast<uint64_t>(m_key.get()))
	{
	// NtQueryKey seems to not work on pseudo-handles to root keys
	case reinterpret_cast<uint64_t>(HKEY_CLASSES_ROOT):
		key_path = L"HKCR";
		break;

	case reinterpret_cast<uint64_t>(HKEY_CURRENT_USER):
		key_path = L"HKCU";
		break;

	case reinterpret_cast<uint64_t>(HKEY_LOCAL_MACHINE):
		key_path = L"HKLM";
		break;

	default: {
		ULONG result_length = 0;
		auto status = NtQueryKey(
			m_key.get(),
			KeyNameInformation,
			nullptr,
			0,
			&result_length
		);
		if (STATUS_BUFFER_TOO_SMALL != status)
		{
			throw BRLException(BRLStatus::REGNODE_KEY_PATH_NTQUERYKEY_FAILED_TO_GET_RESULT_LENGTH, status);
		}

		Buffer info_buffer(result_length);
		status = NtQueryKey(
			m_key.get(),
			KeyNameInformation,
			info_buffer.data(),
			result_length,
			&result_length
		);
		if (STATUS_SUCCESS != status)
		{
			throw BRLException(BRLStatus::REGNODE_KEY_PATH_NTQUERYKEY_FAILED, status);
		}

		const auto& name_info = *reinterpret_cast<const KEY_NAME_INFORMATION*>(info_buffer.data());
		key_path = std::wstring(&name_info.Name[0], name_info.NameLength / sizeof(wchar_t));
		}
	}

	return key_path + (m_subpath ? (L'\\' + *m_subpath) : L"");
}

RegNode::Iter& RegNode::Iter::operator++()
{
	if (!m_key)
	{
		return *this;
	}

	if (0 == m_current_name.capacity())
	{
		const auto info = m_key->info();

		m_current_subkey = info.m_subkeys_count;
		m_current_value = info.m_values_count;
		m_current_name.reserve((max(info.m_max_subkey_name_len, info.m_max_value_name_len) + 1) * sizeof(wchar_t));
	}

	DWORD buffer_length = static_cast<DWORD>(m_current_name.capacity());
	if (0 < m_current_subkey)
	{
		--m_current_subkey;
		const auto lstatus = RegEnumKeyExW(
			m_key->get(),
			m_current_subkey,
			reinterpret_cast<wchar_t*>(m_current_name.data()),
			&buffer_length,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);
		if (ERROR_SUCCESS != lstatus)
		{
			throw BRLException(BRLStatus::REGNODE_ITER_INCREMENT_OPERATOR_REGENUMKEYEXW_FAILED, lstatus);
		}
	}
	else if (0 < m_current_value)
	{
		m_is_current_key = false;
		--m_current_value;
		const auto lstatus = RegEnumValueW(
			m_key->get(),
			m_current_value,
			reinterpret_cast<wchar_t*>(m_current_name.data()),
			&buffer_length,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);
		if (ERROR_SUCCESS != lstatus)
		{
			throw BRLException(BRLStatus::REGNODE_ITER_INCREMENT_OPERATOR_REGENUMVALUEW_FAILED, lstatus);
		}
	}
	else
	{
		m_key.reset();
	}

	return *this;
}

RegNode RegNode::Iter::operator*()
{
	if (!m_key)
	{
		throw BRLException(BRLStatus::REGNODE_ITER_DEREF_OPERATOR_ITERATOR_EXHAUSTED);
	}

	if (m_is_current_key)
	{
		return RegNode(m_key->subkey(reinterpret_cast<wchar_t*>(m_current_name.data())));
	}
	else
	{
		return RegNode(m_key->value(reinterpret_cast<wchar_t*>(m_current_name.data())));
	}
}
