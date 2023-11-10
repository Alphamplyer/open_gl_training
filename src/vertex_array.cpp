#include "vertex_array.h"
#include "renderer.h"

vertex_array::vertex_array()
{
	GL_CALL(glGenVertexArrays(1, &m_RendererID))
}

vertex_array::~vertex_array()
{
	GL_CALL(glDeleteVertexArrays(1, &m_RendererID))
}

void vertex_array::add_buffer(const vertex_buffer& vb, const vertex_buffer_layout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.get_elements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GL_CALL(glEnableVertexAttribArray(i))
		GL_CALL(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.get_stride(), (const void*)offset))
		offset += element.count * vertex_buffer_element::get_size_of_type(element.type);
	}
}

void vertex_array::bind() const
{
	GL_CALL(glBindVertexArray(m_RendererID))
}

void vertex_array::unbind() const
{
	GL_CALL(glBindVertexArray(0))
}
