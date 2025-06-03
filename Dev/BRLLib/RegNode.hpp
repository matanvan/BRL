#pragma once

#include "Common.hpp"
#include "ntdll.hpp"

#include "IEnumerable.hpp"
#include "AutoCloseRegKey.hpp"
#include "RegData.hpp"

class RegNode
{
private:
	class Key
	{
	public:
		Key(const HKEY hkey);
		Key(const HKEY hkey, const std::wstring& subpath);
		Key(const Key& key, const std::wstring& subpath);
		virtual ~Key() = default;
		Key(Key&&) = default;
		Key& operator=(Key&&) = default;

		virtual HKEY get() const;
		virtual Buffer query(const KEY_INFORMATION_CLASS info_class) const;

	private:
		mutable std::variant<std::pair<AutoCloseRegKey, std::wstring>, AutoCloseRegKey> m_key;
	};

	class Value
	{
	public:
		Value();
		virtual ~Value() = default;
		Value(Value&&) = default;
		Value& operator=(Value&&) = default;

		virtual const Key& key() const { return m_key; }

	private:
		const Key m_key;
		const std::wstring m_name;
	};

private:
	RegNode(Key&& key);

public:
	virtual ~RegNode() = default;
	RegNode(RegNode&&) = default;
	RegNode& operator=(RegNode&&) = default;

	virtual RegNode subkey(const std::wstring& subpath) const;
	virtual RegData info() const;

	static const RegNode HKLM;

private:
	const std::variant<Key, Value> m_node;
};
