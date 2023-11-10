#pragma once

class index_buffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	index_buffer(const unsigned int* data, unsigned int count);
	~index_buffer();

	void bind() const;
	static void unbind();

	inline unsigned int get_count() const;
};
