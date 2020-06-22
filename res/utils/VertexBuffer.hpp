#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer (const void* data, unsigned int size, int mode);
    ~VertexBuffer(); //destructor
    
    void Bind() const;
    void Unbind() const;
    void SetData(const void* data, unsigned int size) const;
    void SetDataMap(const void* data, unsigned int size);
};
