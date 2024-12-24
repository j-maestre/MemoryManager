#pragma once

#ifdef MM_BUILD_DLL
#define MM_API __declspec(dllexport) // Exporta s�mbolos si est�s construyendo la DLL
#else
#define MM_API __declspec(dllimport) // Importa s�mbolos si est�s usando la DLL
#endif

class MM_API MemoryManager final{

public:
	MemoryManager();
	MemoryManager(const MemoryManager&) = delete;
	MemoryManager(MemoryManager&&);
	~MemoryManager();
private:

};