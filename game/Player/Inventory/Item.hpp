#pragma once
#include "Util.hpp"

class Item
{
public:
    
    Item(ItemType type, int enumidx);
    Item(ItemType type, int enumidx, int duplicatecnt);

    ~Item();
    
    void Increment (int delta = 1);
    void Decrement (int delta = 1);
    int GetDuplicateCount() const;
    int GetEnumIdx() const;
    int GetMaxDuplicateCount() const;
    ItemType GetType() const;
    
private:
    ItemType m_Type;
    int m_EnumIdx;
    int m_MaxDuplicateCount;
    int m_NumDuplicates;
};
