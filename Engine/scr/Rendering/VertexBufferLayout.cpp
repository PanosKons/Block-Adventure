#include <Engine.h>
#include "VertexBufferLayout.h"
#include "glad/glad.h"
void VertexBufferLayout::Calculate()
{
	unsigned int offset = 0;
	for (unsigned int i = 0; i < counts.size(); i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, counts[i], GL_FLOAT, GL_FALSE, m_Stride * sizeof(float), (const void*)offset);
		offset += counts[i] * sizeof(float);
	}
}