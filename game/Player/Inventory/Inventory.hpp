#pragma once
#include "Util.hpp"
#include "Item.hpp"

class Inventory
{
public:
    Inventory(int numslots);
    ~Inventory();
    void AddItem(ItemType type, int enumidx, int cnt);
    void RemoveItem(ItemType type, int enumidx, int cnt);
    Item* GetCurrItem();


private:
    int m_Slots;
    int m_CurrSlot;
    std::vector <Item*> m_Items;
};
