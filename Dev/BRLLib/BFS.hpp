#pragma once

#include <iterator>
#include <queue>

template <typename T>
class BFS
{
public:
	BFS(T root) { m_queue.push(std::move(root)); }

	virtual ~BFS() = default;
	BFS(const BFS&) = delete;
	BFS& operator=(const BFS&) = delete;
	BFS(BFS&&) = default;

	virtual BFS begin() { return std::move(*this); }
	virtual nullptr_t end() { return nullptr; }
	virtual bool operator!=(nullptr_t) { return !m_queue.empty(); }
	virtual BFS& operator++();
	virtual T& operator*() { return m_queue.front(); }

private:
	std::queue<T> m_queue;
};

template<typename T>
inline BFS<T>& BFS<T>::operator++()
{
	const auto& node = m_queue.front();
	for (auto child : node)
	{
		m_queue.push(std::move(child));
	}

	m_queue.pop();
	return *this;
}
