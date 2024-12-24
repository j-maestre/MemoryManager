#include "memory_manager.h"
#include <stdio.h>

MemoryManager::MemoryManager() {
}

MemoryManager::MemoryManager(MemoryManager&&)
{
}

MemoryManager::~MemoryManager(){
#ifdef MM_DEBUG
	printf("\nDeallocating memory...\n");
#endif
	bool mem_leaks = false;

	// Free de los bloques que han podido subdividirse en varios
	for (const auto& v : m_blocks) {

		if (v.second.in_use) {
			mem_leaks = true;
			printf("\033[38;2;255;255;0mMemory leak in adress [0x%lx] with size of: %d bytes\033[0m\n", (uintptr_t)v.second.adress, v.second.size);
		}
	}

	if(!mem_leaks)printf("\033[1;32m*** No memory leaks founded ***\033[0m\n");

	// Free de los bloques originales
	for (const auto& v : m_original_blocks) {
		free(v.second.adress);
	}

}

void MemoryManager::init(const char* conf_path){
	// Read block data size and pre allocate

	std::ifstream file(conf_path);
	if (!file.is_open()) {
		std::cerr << "No se puede abrir el archivo: " << conf_path << std::endl;
	}else {

		std::string line;
		while (std::getline(file, line)) {
		
			// Eliminar espacios y saltos de línea al principio y al final
			line.erase(0, line.find_first_not_of(" \t"));
			line.erase(line.find_last_not_of(" \t") + 1);

			// Ignorar líneas vacías y comentarios (líneas que comienzan con ';')
			if (line.empty() || line[0] == ';') {
				continue;
			}

			// Buscar la separación entre la clave y el valor
			size_t equal_pos = line.find('=');
			if (equal_pos != std::string::npos) {
				// Obtener la clave (antes del "=") y el valor (después del "=")
				std::string key_str = line.substr(0, equal_pos);
				std::string value_str = line.substr(equal_pos + 1);

				// Convertir la clave y el valor a enteros
				int key = std::stoi(key_str);
				int value = std::stoi(value_str);


				// Almacenar en el mapa
				m_memory[key] = value;
			}
		}
		file.close();
#ifdef MM_DEBUG
		printf("Configuration file readed correctly\n");
#endif

		// Pre allocate memory
		for (const auto& value : m_memory) {

#ifdef MM_DEBUG
			printf("Pre allocating %d blocks of %d bytes\n", value.second, value.first);
#endif
			for (unsigned int i = 0; i < value.second; i++) {
				Block b;
				b.size = value.first;
				b.adress = malloc(value.first);
				m_blocks[b.adress] = b;
				m_original_blocks[b.adress] = b;
			}	

			if (value.first > m_max_block_size) m_max_block_size = value.first;
		}
	}
}

void* MemoryManager::allocate(unsigned int bytes){

	if (bytes == 0) return nullptr;

	// Round bytes to next power of 2
	//printf("Round to next power of 2 with number: %d\n", bytes);
	bytes--;
	bytes |= bytes >> 1;
	bytes |= bytes >> 2;
	bytes |= bytes >> 4;
	bytes |= bytes >> 8;
	bytes |= bytes >> 16;
	bytes |= bytes >> 32;
	bytes++;
	//printf("Rounded number %d\n", bytes);

	// TODO: Round to next power of 2, for the moment has to be power of 2 only for developing
	for (auto& v : m_blocks) {

		// only can be allocated if is not in use
		if (v.second.size == bytes && v.second.in_use == false) {

			printf("MM Allocating %d bytes in adress [0x%lx]\n", bytes, (uintptr_t)v.second.adress);
			// Mark this block as in use
			v.second.in_use = true;

			return (void*)v.second.adress;
		}
	}

	// Si llega aqui esque no ha encontrado ningun bloque libre, comprobar si se puede dividir alguno

	bool next_size_found = false;
	unsigned int subdivisions = 0;

	do {

		bytes = bytes << 1; // round to next power of 2
		subdivisions++;

#ifdef MM_DEBUG
		printf("\033[1;33mBlock not founded, searching next power of 2 for %d bytes\033[0m\n", bytes);
#endif

		for (auto& v : m_blocks) {

			// only can be allocated if is not in use
			if (v.second.size == bytes && v.second.in_use == false) {

				// Encontrado un bloque el doble de grande, dividir en 2 y anadirlo al unordered map
				next_size_found = true;
#ifdef MM_DEBUG
				printf("MM Allocating next power of 2, %d bytes in adress 0x%p\n", bytes, v.second.adress);
#endif
				// Mark this block as in use
				v.second.in_use = true;
				v.second.size = bytes >> subdivisions; // last block size / subdivisions

				// recorrer el primero sumando bytes >> subdivisions para subdividir el bloque entre los minibloques que hagan falta

				void* next_adress = v.second.adress;
				for (unsigned int i = 0; i < (bytes >> 1) - 1; i++) {

					next_adress = (char*)(next_adress) + (bytes >> subdivisions);

					// add next block 
					Block b;
					b.adress = next_adress;
					b.size = bytes >> subdivisions;
					b.in_use = false;

					m_blocks[b.adress] = b;
				}

				return (void*)v.second.adress;
			}
		}
	} while (!next_size_found && bytes <= m_max_block_size);

	printf("\033[1;31mMemory can not be allocated with size of %d bytes\033[0m\n", bytes >> subdivisions);
	return nullptr;
}

void MemoryManager::deallocate(void* adress){

	m_blocks[adress].in_use = false;
	
}
