#include "Inventory.hpp"

Inventory::Inventory(int numslots):m_CurrSlot(0), m_Slots(numslots)
{
    
}

Inventory::~Inventory()
{
    
}

void Inventory::AddItem(ItemType type, int enumidx, int cnt)
{
    if (cnt < 0) return;
    for (int i = 0; i < (int)m_Items.size(); i++)
    {
        Item* item = m_Items[i];
        if ((item -> GetType() == type) && (item -> GetEnumIdx() == enumidx) && (item -> GetDuplicateCount()) < (item -> GetMaxDuplicateCount()))
        {
            int delta = std::min (cnt, (item -> GetMaxDuplicateCount()) - (item -> GetDuplicateCount()));
            item -> Increment (delta);
            cnt -= delta;
            if (cnt == 0) break;
        }
    }
    while (cnt > 0 && (int)m_Items.size() < m_Slots)
    {
        if (type == ItemType::Block)
        {
            if (cnt <= 64)
            {
                m_Items.push_back(new Item (type, enumidx, cnt));
                cnt = 0;
            }
            else
            {
                Item newi (type, enumidx, 64);
                m_Items.push_back(new Item (type, enumidx, 64));
                cnt -= 64;
            }
        }
    }
}

void Inventory::RemoveItem(ItemType type, int enumidx, int cnt)
{
    if (cnt < 0) return;
    for (int i = 0; i < (int)m_Items.size(); i++)
    {
        Item* item = m_Items[i];
        if ((item -> GetType() == type) && (item -> GetEnumIdx() == enumidx) && (item -> GetDuplicateCount() > 0))
        {
            int delta = std::min (cnt, item -> GetDuplicateCount());
            bool b = ((item -> GetDuplicateCount()) == delta);
            item -> Decrement(delta);
            if (b)
            {
                delete m_Items[i];
                m_Items[i] = nullptr;
            }
            cnt -= delta;
            if (cnt == 0) break;
        }
    }
    std::vector <Item*> temp;
    for (int i = 0; i < (int)m_Items.size(); i++)
    {
        if (m_Items[i] != nullptr) temp.push_back(m_Items[i]);
    }
    m_Items.clear();
    for (int i = 0; i < (int)temp.size(); i++) m_Items.push_back(temp[i]);
}

Item* Inventory::GetCurrItem ()
{
    if (!(m_CurrSlot < (int)m_Items.size())) return nullptr;
    return m_Items[m_CurrSlot];
}
