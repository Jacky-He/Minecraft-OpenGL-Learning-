#pragma once

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer (const unsigned int* data, unsigned int count, int mode);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    void SetData(const unsigned int* data, unsigned int count) const;

    unsigned int GetCount() const {return m_Count;}
};
