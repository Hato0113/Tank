//----------------------
//	メモリアロケータインターフェースクラス
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

class iMemoryAllocator
{
public:
	iMemoryAllocator() {};
	virtual ~iMemoryAllocator() {};

	virtual void* Alloc(const size_t size) = 0;
	virtual void Free(void* ptr) = 0;
};


