//----------------------
//	Allocator.hpp
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

//-- include --
#include <memory>
#include "iMemoryAllocator.hpp"

//-- クラス定義 --
class Allocator
{
public:

	static bool Init(void* ptr, size_t size);
	static void Uninit();

	template<class T, class... Args>
	static T* Alloc(Args&&... args);

	template<class T>
	static void Free(T* ptr);

private:

	static std::unique_ptr<iMemoryAllocator> m_allocator;
};

template<class T, class ...Args>
inline T* Allocator::Alloc(Args && ...args)
{
	void* ptr = m_allocator->Alloc(sizeof(T));
	return ptr == nullptr ? nullptr : new(ptr)T(args...);
}

template<class T>
inline void Allocator::Free(T* ptr)
{
	ptr->~T();
	m_allocator->Free(ptr);
}



