#pragma once
#include <vector>
class VertexBufferLayout
{
public:
	void Calculate();
	template<typename T>
	void Push(int count)
	{
		m_Stride += count;
		counts.push_back(count);
	}
private:
	unsigned int m_Stride = 0;
	std::vector<unsigned int> counts;
};
