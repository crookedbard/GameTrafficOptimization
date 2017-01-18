#include "list.h"

CList::CList()
{
	items = static_cast<void**>(malloc(5 * sizeof(void*)));
	buffsize = 5;
	count = 0;
}

CList::CList(int buffsize)
{
	buffsize = std::max(buffsize, 1);
	this->buffsize = buffsize;
	items = static_cast<void**>(malloc(buffsize * sizeof(void*)));
	count = 0;
}

CList::~CList()
{
	free(items);
}

int CList::Add(void* item)
{
	if (count >= buffsize)
	{
		buffsize += 5;
		items = static_cast<void**>(realloc(items, buffsize * sizeof(void**)));
	}
	items[count] = item;
	count++;
	return count;
}

void CList::Remove(void* item)
{
	for (auto i = 0; i < count; i++)
		{
			if (items[i] == item)
			{
				memmove(items + i, items + i + 1, (count - i - 1) * sizeof(void**));
				count--;
				break;
			}
		}
}

void CList::Remove(int i)
{
	if (i < 0 || i >= count)return;
	if (count > 1)
	{
		memmove(items + i, items + i + 1, (count - i - 1) * sizeof(void**));
		count--;
	}
	else Clear();
}

void CList::set(int pos, void* item) const
{
	if (pos < 0 || pos >= count)return;
	items[pos] = item;
}

void CList::Clear()
{
	free(items);
	items = static_cast<void**>(malloc(5 * sizeof(void*)));
	buffsize = 5;
	count = 0;
}

void* CList::find(void* item) const
{
	for (auto i = 0; i < count; i++)
			if (items[i] == item)return items[i];
	return nullptr;
}

void* CList::operator [](int i) const
{
	if (i < 0 || i >= count)return nullptr;
	return items[i];
}

void* CList::item(int i) const
{
	if (i < 0 || i >= count)return nullptr;
	return items[i];
}
