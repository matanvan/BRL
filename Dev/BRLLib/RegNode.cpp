#include "RegNode.hpp"

RegNode::RegNode(Key&& key) :
	m_node(std::move(key))
{}

RegNode RegNode::subkey(const std::wstring& subpath) const
{
	if (std::holds_alternative<Value>(m_node))
	{
		throw std::exception("TODO");
	}
	else
	{
		return RegNode(Key(std::get<Key>(m_node), subpath));
	}
}

RegData RegNode::info() const
{
	if (std::holds_alternative<Key>(m_node))
	{
		const auto full_info_buffer = std::get<Key>(m_node).query(KeyFullInformation);
		const auto name_info_buffer = std::get<Key>(m_node).query(KeyNameInformation);
		const auto& full_info = *reinterpret_cast<const KEY_FULL_INFORMATION*>(full_info_buffer.data());
		const auto& name_info = *reinterpret_cast<const KEY_NAME_INFORMATION*>(name_info_buffer.data());
		return RegKeyData {
			.m_last_write_time = std::chrono::system_clock::from_time_t(full_info.LastWriteTime.QuadPart),
			.m_path = std::wstring(&name_info.Name[0], name_info.NameLength / sizeof(wchar_t)),
			.m_class_name = std::wstring(&full_info.Class[0], full_info.ClassLength / sizeof(wchar_t)),
			.m_subkeys_count = full_info.SubKeys,
			.m_max_subkey_name_size_bytes = full_info.MaxNameLen,
			.m_values_count = full_info.Values,
			.m_max_value_name_size_bytes = full_info.MaxValueNameLen,
			.m_max_value_size_bytes = full_info.MaxValueDataLen,
			.m_max_class_name_size_bytes = full_info.MaxClassLen
		};
	}
	else
	{
		throw std::exception("TODO: implement");
	}
	
}

RegNode::Key::Key(const HKEY hkey) :
	m_key(AutoCloseRegKey(hkey))
{}

RegNode::Key::Key(const HKEY hkey, const std::wstring& subpath) :
	m_key(std::pair(AutoCloseRegKey(hkey), subpath))
{}

RegNode::Key::Key(const Key& key, const std::wstring & subpath) :
	Key(
		std::holds_alternative<AutoCloseRegKey>(key.m_key) ? std::get<1>(key.m_key).get() : std::get<0>(key.m_key).first.get(),
		std::holds_alternative<AutoCloseRegKey>(key.m_key) ? subpath : (std::get<0>(key.m_key).second + L'\\' + subpath)
	)
{}

HKEY RegNode::Key::get() const
{
	if (std::holds_alternative<AutoCloseRegKey>(m_key))
	{
		return std::get<1>(m_key).get();
	}
	else
	{
		const auto& incomplete_key = std::get<0>(m_key);
		HKEY hkey = nullptr;
		const auto lstatus = RegOpenKeyExW(
			incomplete_key.first.get(),
			incomplete_key.second.c_str(),
			0,
			KEY_READ,
			&hkey
		);
		if (ERROR_SUCCESS != lstatus)
		{
			throw std::exception("TODO");
		}
		m_key = AutoCloseRegKey(hkey);
		return hkey;
	}
}

Buffer RegNode::Key::query(const KEY_INFORMATION_CLASS info_class) const
{
	ULONG result_length = 0;
	auto status = NtQueryKey(
		get(),
		info_class,
		nullptr,
		0,
		&result_length
	);
	if (STATUS_BUFFER_TOO_SMALL != status)
	{
		throw std::exception("TODO");
	}

	Buffer info_buffer(result_length);
	status = NtQueryKey(
		get(),
		info_class,
		info_buffer.data(),
		result_length,
		&result_length
	);
	if (STATUS_SUCCESS != status)
	{
		throw std::exception("TODO");
	}

	return info_buffer;
}

const RegNode RegNode::HKLM = RegNode(Key(HKEY_LOCAL_MACHINE));
