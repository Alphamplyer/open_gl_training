#pragma once
#include <vector>
#include <GL/glew.h>
#include <stdexcept>
#include "renderer.h"


struct vertex_buffer_element
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int get_size_of_type(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class vertex_buffer_layout
{
private:
	std::vector<vertex_buffer_element> m_Elements;
	unsigned int m_Stride;
public:
	vertex_buffer_layout()
		: m_Stride(0) {}

	template<typename T>
	void push(unsigned int count)
	{
		std::runtime_error(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * vertex_buffer_element::get_size_of_type(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * vertex_buffer_element::get_size_of_type(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * vertex_buffer_element::get_size_of_type(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<vertex_buffer_element> get_elements() const { return m_Elements; }
	inline unsigned int get_stride() const { return m_Stride; }
};