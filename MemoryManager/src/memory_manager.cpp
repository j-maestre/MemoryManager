#include "memory_manager.h"
#include <stdio.h>

MemoryManager::MemoryManager() {
	printf("\n*** Memory constructor ***\n");
}

MemoryManager::MemoryManager(MemoryManager&&)
{
}

MemoryManager::~MemoryManager(){
	printf("\n*** Deallocating memory ***\n");
	bool mem_leaks = false;

	for (const auto& v : m_blocks) {

		if (v.second.in_use) {
			mem_leaks = true;
			// RED text printf("\033[1;31mMemory leak in adress %p with size of: %d bytes\033[0m\n", v.second.adress, v.second.size);
			printf("\033[1;33mMemory leak in adress 0x%p with size of: %d bytes\033[0m\n", v.second.adress, v.second.size);
		}
		free(v.second.adress);
	}

	if(!mem_leaks)printf("\033[1;32mNo memory leaks founded\033[0m\n");

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
		printf("Configuration file readed correctly\n");

		// Pre allocate memory
		for (const auto& value : m_memory) {

			printf("Pre allocating %d blocks of %d bytes\n", value.second, value.first);
			for (unsigned int i = 0; i < value.second; i++) {
				Block b;
				b.size = value.first;
				b.adress = malloc(value.first);
				m_blocks[b.adress] = b;
			}

			
		}


	}
}

void* MemoryManager::allocate(size_t bytes){

	// TODO: Round to next power of 2, for the moment has to be power of 2 only for developing
	for (auto& v : m_blocks) {

		// only can be allocated if is not in use
		if (v.second.size == bytes && v.second.in_use == false) {

			printf("MM Allocating %d bytes in adress 0x%p\n", bytes, v.second.adress);
			// Mark this block as in use
			v.second.in_use = true;

			return (void*)v.second.adress;
		}
	}

	return nullptr;
}

void MemoryManager::deallocate(void* adress){

	m_blocks[adress].in_use = false;
	
}
