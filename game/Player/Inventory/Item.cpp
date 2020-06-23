#include "Item.hpp"

Item::Item(ItemType type, int enumidx):m_Type(type), m_EnumIdx(enumidx), m_NumDuplicates(0)
{
    Increment();
    if (type == ItemType::Block) m_MaxDuplicateCount = 64;
    else m_MaxDuplicateCount = 1;
}

Item::Item(ItemType type, int enumidx, int duplicatecnt):m_Type(type), m_EnumIdx(enumidx), m_NumDuplicates(0)
{
    Increment(duplicatecnt);
    if (type == ItemType::Block) m_MaxDuplicateCount = 64;
    else m_MaxDuplicateCount = 1;
}

Item::~Item()
{
    
}

ItemType Item::GetType() const {return m_Type;}

void Item::Increment(int delta)
{
    if (m_NumDuplicates + delta > m_MaxDuplicateCount) return;
    m_NumDuplicates += delta;
}

void Item::Decrement(int delta) 
{
    if (m_NumDuplicates - delta < 0) return;
    m_NumDuplicates -= delta;
}

int Item::GetDuplicateCount() const
{
    return m_NumDuplicates;
}

int Item::GetEnumIdx() const
{
    return m_EnumIdx;
}

int Item::GetMaxDuplicateCount() const
{
    return m_MaxDuplicateCount;
}
