#pragma once

#ifdef MM_BUILD_DLL
#define MM_API __declspec(dllexport) // Exporta símbolos si estás construyendo la DLL
#else
#define MM_API __declspec(dllimport) // Importa símbolos si estás usando la DLL
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>

class MM_API MemoryManager final{

	struct Block {
		unsigned int size;
		void* adress;
		bool in_use = false;
	};

private:
	MemoryManager();
public:
	static MemoryManager& get_instance() {
		static MemoryManager instance;
		return instance;
	}
	MemoryManager(const MemoryManager&) = delete;
	MemoryManager(MemoryManager&&);
	~MemoryManager();

	void init(const char* conf_path);

	void* allocate(unsigned int bytes);
	void deallocate(void* adress);

private:

	// Size and amount of each block
	std::map<int, int> m_memory;

	// Pre allocated blocks of each size
	std::unordered_map<void*, Block> m_blocks;
	std::unordered_map<void*, Block> m_original_blocks;

	unsigned int m_max_block_size = 0;

};