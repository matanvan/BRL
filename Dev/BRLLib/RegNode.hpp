#pragma once

#include "Common.hpp"
#include "ntdll.hpp"

#include "AutoClose.hpp"
#include "RegData.hpp"

class RegNode
{
private:
	class Value;

	class Key
	{
	public:
		explicit Key(AutoCloseRegKey hkey) :
			m_key(std::move(hkey))
		{}

		Key(AutoCloseRegKey hkey, const std::wstring& subpath) :
			m_key(std::move(hkey)),
			m_subpath(subpath)
		{}

		virtual ~Key() = default;
		Key(const Key&) = delete;
		Key& operator=(const Key&) = delete;
		Key(Key&&) noexcept = default;
		Key& operator=(Key&&) noexcept = default;

		virtual Key subkey(const std::wstring& subpath) const { return { m_key.clone(), m_subpath ? (*m_subpath + L'\\' + subpath) : subpath }; }
		virtual Value value(const std::wstring& name) const { return Value(clone(), name); }
		virtual HKEY get() const;
		virtual Key clone() const { return m_subpath ? Key(m_key.clone(), *m_subpath) : Key(m_key.clone()); }
		virtual RegKeyData info() const;
		virtual std::wstring path() const;

	private:
		mutable AutoCloseRegKey m_key;
		mutable std::optional<std::wstring> m_subpath;
	};

	class Value
	{
	public:
		Value(Key key, const std::wstring& name) noexcept :
			m_key(std::move(key)),
			m_name(name)
		{}

		virtual ~Value() = default;
		Value(const Value&) = delete;
		Value& operator=(const Value&) = delete;
		Value(Value&&) noexcept = default;
		Value& operator=(Value&&) noexcept = default;

		virtual const Key& key() const { return m_key; }
		virtual const std::wstring& name() const { return m_name; }
		virtual const std::wstring path() const { return m_key.path() + L'\\' + m_name; }

	private:
		Key m_key;
		std::wstring m_name;
	};

public:
	class Iter
	{
	public:
		Iter() {}

		explicit Iter(Key key) :
			m_key(std::move(key))
		{ this->operator++(); }

		virtual ~Iter() = default;
		Iter(const Iter&) = delete;
		Iter& operator=(const Iter&) = delete;

		virtual bool operator==(const Iter& other) const { return (!m_key) && (!other.m_key); }
		virtual Iter& operator++();
		virtual RegNode operator*();

	private:
		std::optional<Key> m_key = std::nullopt;
		uint32_t m_current_subkey = 0;
		uint32_t m_current_value = 0;
		Buffer m_current_name;
		bool m_is_current_key = true;
	};

private:
	explicit RegNode(Key key) :
		m_node(std::move(key))
	{}

	explicit RegNode(Value value) :
		m_node(std::move(value))
	{}

public:
	virtual ~RegNode() = default;
	RegNode(RegNode&&) = default;
	RegNode& operator=(RegNode&&) = default;

	virtual RegNode subkey(const std::wstring& subpath) const;
	virtual std::wstring path() const;
	virtual RegData info() const;
	virtual Iter begin() const;
	virtual Iter end() const { return Iter(); }

	inline static RegNode HKCR() { return RegNode(Key(AutoCloseRegKey(HKEY_CLASSES_ROOT))); }
	inline static RegNode HKCU() { return RegNode(Key(AutoCloseRegKey(HKEY_CURRENT_USER))); }
	inline static RegNode HKLM() { return RegNode(Key(AutoCloseRegKey(HKEY_LOCAL_MACHINE))); }

private:
	std::variant<Key, Value> m_node;
};
