#include <stdio.h>
#include "memory_manager.h"

int main() {

	printf("Ole los caracoles\n");
	MemoryManager& mm_instance = MemoryManager::get_instance();
	mm_instance.init("memory.ini");

	void* adress1 = mm_instance.allocate(16);
	void* adress2 = mm_instance.allocate(2);
	void* adress3 = mm_instance.allocate(256);

	mm_instance.deallocate(adress1);
	//mm_instance.deallocate(adress2);
	mm_instance.deallocate(adress3);

	return 0;
}