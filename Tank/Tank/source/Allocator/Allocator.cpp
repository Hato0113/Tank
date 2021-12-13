//----------------------
//	Allocator.cpp
//-- author --
//	HatoriMasashi
//----------------------

//-- include --
#include "Allocator.hpp"
#include "LinearAllocator.h"

std::unique_ptr<iMemoryAllocator> Allocator::m_allocator;

bool Allocator::Init(void* ptr, size_t size)
{
	m_allocator = std::make_unique<LinearAllocator>(ptr, size);
	return m_allocator != nullptr;	//true 確保成功
}

void Allocator::Uninit()
{
	//none
}
