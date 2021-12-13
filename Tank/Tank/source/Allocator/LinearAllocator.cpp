//----------------------
//	線形リストアロケータ
//-- author --
//	HatoriMasashi
//----------------------

//-- include --
#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(void* ptr, const size_t size)
{
	m_HeadAddress = reinterpret_cast<std::uintptr_t>(ptr);
	m_memorySize = size;

	//-- 先頭ヘッダの初期化 --
	auto* headInfo = reinterpret_cast<LinearAllocator::Header*>(ptr);
	headInfo->IsAllocated = false;
	headInfo->nextAddress = 0;
	headInfo->prevAddress = 0;
	headInfo->size = size - sizeof(LinearAllocator::Header);
}

/*
	確保関数
	引数 : 確保サイズ
*/
void* LinearAllocator::Alloc(const size_t size)
{
	//-- 確保領域検索 --
	auto memoryInfo = reinterpret_cast<LinearAllocator::Header*>(m_HeadAddress);
	while (memoryInfo != nullptr)
	{
		if (!memoryInfo->IsAllocated && memoryInfo->size >= size) break;
		memoryInfo = reinterpret_cast<LinearAllocator::Header*>(memoryInfo->nextAddress);
	}

	if (memoryInfo == nullptr) return nullptr;

	//-- 情報更新 --
	std::uintptr_t infoAddress = reinterpret_cast<std::uintptr_t>(memoryInfo);	//現在アドレス
	std::uintptr_t nextAddress = infoAddress + size + sizeof(LinearAllocator::Header);	//次の予定地

	if (nextAddress >= m_HeadAddress + m_memorySize)
	{
		nextAddress = 0;	//領域外
	}
	auto remainSize = memoryInfo->size - size;	//残量を更新

	if (remainSize > sizeof(LinearAllocator::Header) && nextAddress != 0)
	{
		//-- ヘッダ情報 --
		auto nextInfo = reinterpret_cast<LinearAllocator::Header*>(nextAddress);
		nextInfo->IsAllocated = false;
		nextInfo->nextAddress = memoryInfo->nextAddress;
		nextInfo->prevAddress = infoAddress;
		nextInfo->size = remainSize - sizeof(LinearAllocator::Header);

		nextInfo = reinterpret_cast<LinearAllocator::Header*>(nextInfo->nextAddress);
		if (nextInfo != nullptr)
		{
			nextInfo->prevAddress = nextAddress;
		}
	}
	else
	{
		nextAddress = memoryInfo->nextAddress;
	}

	memoryInfo->IsAllocated = true;
	memoryInfo->nextAddress = nextAddress;
	memoryInfo->size = size;

	return reinterpret_cast<void*>(infoAddress + sizeof(LinearAllocator::Header));
}

void LinearAllocator::Free(void* ptr)
{
	if (ptr == nullptr) return;

	std::uintptr_t memoryAddress = reinterpret_cast<std::uintptr_t>(ptr);
	if (memoryAddress - m_HeadAddress >= m_memorySize) return;	//管理外

	auto memoryInfo = reinterpret_cast<LinearAllocator::Header*>(memoryAddress - sizeof(LinearAllocator::Header));

	memoryInfo->IsAllocated = false;
	memoryInfo->size = memoryInfo->nextAddress - memoryAddress;
	memoryInfo = Integrate(reinterpret_cast<std::uintptr_t>(memoryInfo));
	memoryInfo = Integrate(memoryInfo->nextAddress);
}

LinearAllocator::Header* LinearAllocator::Integrate(const std::uintptr_t address)
{
	if (address == 0) return nullptr;

	//-- 領域の統合 --
	auto info = reinterpret_cast<LinearAllocator::Header*>(address);
	auto prevInfo = reinterpret_cast<LinearAllocator::Header*>(info->prevAddress);

	if (info->IsAllocated) return info;
	if (prevInfo == nullptr) return info;

	auto* nextInfo = reinterpret_cast<LinearAllocator::Header*>(info->nextAddress);	//1つ後ろ

	if (prevInfo->IsAllocated)
	{
		
		auto endAddress = info->prevAddress + sizeof(LinearAllocator::Header) + prevInfo->size;

		auto remainingSize = address - endAddress;	//間のサイズ

		if (remainingSize == 0) return info;	//空き無し

		prevInfo->nextAddress = endAddress;
		auto size = info->size + remainingSize;	//サイズ計算

		//-- 空きの分だけ移動、情報を格納 --
		info = reinterpret_cast<LinearAllocator::Header*>(endAddress);
		info->IsAllocated = false;
		info->size = size;
		info->nextAddress = reinterpret_cast<std::uintptr_t>(nextInfo);
		info->prevAddress = reinterpret_cast<std::uintptr_t>(prevInfo);

	}
	else
	{
		//-- サイズ計算 --
		prevInfo->size += info->size + sizeof(LinearAllocator::Header);
		prevInfo->nextAddress = info->nextAddress;
		info = prevInfo;
	}

	//-- 後ろの領域の情報を更新する --
	if (nextInfo != nullptr)
	{
		nextInfo->prevAddress = reinterpret_cast<std::uintptr_t>(info);
	}

	return info;
}