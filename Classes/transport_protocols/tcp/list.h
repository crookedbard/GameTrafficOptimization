#pragma once
#include "common.h"

class CList
{
	void**items;
	int buffsize;
public:
	int count;
	explicit CList(int buffsize);
	CList();
	~CList();
	int Add(void*item);
	void Remove(void*item);
	void Remove(int index);
	void Clear();
	void* operator[](int index) const;
	void* item(int index) const;
	void* find(void*item) const;
	void set(int pos, void*item) const;
};
