#pragma once

#include "IAction.hpp"
#include "IWriter.hpp"
#include "ISerializer.hpp"

template <typename TData>
class LogAction : public IAction <TData>
{
public:
	LogAction(IWriterUPtr writer, ISerializerUPtr<TData> serialzier);
	virtual ~LogAction() = default;
	LogAction(const LogAction&) = delete;
	LogAction operator=(const LogAction&) = delete;

	void execute(const TData& data) override;

private:
	IWriterUPtr m_writer;
	ISerializerUPtr<TData> m_serializer;
};

template <typename TData>
LogAction<TData>::LogAction(IWriterUPtr writer, ISerializerUPtr<TData> serialzier) :
	IAction(),
	m_writer(std::move(writer)),
	m_serializer(std::move(serialzier))
{}

template <typename TData>
inline void LogAction<TData>::execute(const TData& data)
{
	m_writer->write(m_serializer->serialize());
}
