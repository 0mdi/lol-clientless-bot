#pragma once
#include <Windows.h>
#include <vector>

#define MEMORYSTREAM_MAXSIZE 10000

class MemoryStream
{
public:
	MemoryStream(std::streamsize size)
	{
		m_buffer.resize(size);

		m_size = 0;
		m_wpos = 0;
		m_rpos = 0;
	}

	MemoryStream()
	{
		m_buffer.resize(MEMORYSTREAM_MAXSIZE);

		m_size = 0;
		m_wpos = 0;
		m_rpos = 0;
	}

	template<typename T>
	MemoryStream(T rData, std::streamsize size)
	{
		if (size > MEMORYSTREAM_MAXSIZE)
			return;/*m_buffer.resize(size);*/
		else
			m_buffer.resize(MEMORYSTREAM_MAXSIZE);

		m_size = 0;
		m_wpos = 0;
		m_rpos = 0;

		memcpy(&m_buffer[m_size + m_wpos], rData, size);
		m_size += size;
	}

	~MemoryStream()
	{
	}

	void clear()
	{
		m_size = 0;
		m_wpos = 0;
		m_rpos = 0;
	}

	template<typename T>
	void operator >> (const T& destination)
	{
		auto destinationSize = sizeof(destination);
		memcpy((BYTE*)&destination, &m_buffer[m_rpos], destinationSize);

		m_rpos += destinationSize;
	}

	void append(MemoryStream &memStream)
	{
		auto dataSize = memStream.size();

		if (m_size + dataSize >= m_buffer.size())
			throw std::exception("Max size reached : MemoryStream");

		memcpy(&m_buffer[m_wpos], memStream.getPtr<BYTE*>(), dataSize);
		m_size += dataSize;
		m_wpos += dataSize;
	}

	template<typename T>
	void operator<<(const T& value)
	{
		auto dataSize = sizeof(value);

		if (m_size + dataSize >= MEMORYSTREAM_MAXSIZE)
			throw std::exception("Max size reached : MemoryStream");

		memcpy(&m_buffer[m_wpos], &value, dataSize);
		m_size += dataSize;
		m_wpos += dataSize;
	}

	void operator<<(const char value[])
	{
		auto dataSize = strlen(value);

		if (m_size + dataSize >= m_buffer.size())
			throw std::exception("Max size reached : MemoryStream");

		memcpy(&m_buffer[m_wpos], value, dataSize);
		m_size += dataSize;
		m_wpos += dataSize;
	}

	template<typename T>
	T getPtr()
	{
		return (T)&m_buffer[0];
	}

	void wpos(std::streamsize writePos)
	{
		m_wpos = writePos;
	}

	std::streamsize wpos()
	{
		return m_wpos;
	}

	void rpos(std::streamsize readPos)
	{
		m_rpos = readPos;
	}

	std::streamsize rpos()
	{
		return m_rpos;
	}

	void reset()
	{
		m_buffer.clear();
		m_buffer.resize(MEMORYSTREAM_MAXSIZE);

		m_size = 0;
		m_wpos = 0;
		m_rpos = 0;
	}

	//protected:
	std::vector<BYTE>& getVector()
	{
		return m_buffer;
	}

	std::streamsize size()
	{
		return m_size/* + m_wpos*/;
	}

	void size(std::streamsize newSize)
	{
		m_size = newSize;
	}

private:
	std::vector<BYTE>	m_buffer;
	std::streamsize		m_wpos;
	std::streamsize		m_rpos;
	std::streamsize		m_size;
};