#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return countIndex; }
private:
	unsigned int rendererId = 0;
	unsigned int countIndex = 0;
};


#endif // !INDEXBUFFER_H

