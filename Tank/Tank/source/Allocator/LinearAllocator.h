//----------------------
//	線形リストアロケータ
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

//-- include --
#include "iMemoryAllocator.hpp"
#include <cstdint>

//-- クラス定義 --
class LinearAllocator : public iMemoryAllocator
{
public:
	LinearAllocator(void* ptr,const size_t size);
	virtual ~LinearAllocator() = default;

	virtual void* Alloc(const size_t size) override;
	virtual void Free(void* ptr) override;

private:
	struct Header
	{
		bool IsAllocated{ false };	//割り当て済みか否か
		size_t size{ 0 };
		std::uintptr_t nextAddress{ 0 };
		std::uintptr_t prevAddress{ 0 };
	};

	//領域統合関数
	LinearAllocator::Header* Integrate(const std::uintptr_t address);
	
	size_t m_memorySize{ 0 };	//全体サイズ
	std::uintptr_t m_HeadAddress{ 0 };
};
