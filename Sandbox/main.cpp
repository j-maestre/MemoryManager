#include <stdio.h>
#include "memory_manager.h"

int main() {

	MemoryManager& mm_instance = MemoryManager::get_instance();
	mm_instance.init("mini_memory.ini");

	void* adress2 = mm_instance.allocate(1);
	
	/*
	void* adress4 = mm_instance.allocate(1);
	void* adress5 = mm_instance.allocate(3);
	void* adress6 = mm_instance.allocate(12);
	void* adress7 = mm_instance.allocate(200);
	void* adress8 = mm_instance.allocate(67108863);
	void* adress9 = mm_instance.allocate(67108860);
	void* adress10 = mm_instance.allocate(33554430);
	void* adress11 = mm_instance.allocate(33554433);
	*/

	// Allocating full of 16384
	/*
	void* adresses[8192];
	void* adresses2[8192];
	for (unsigned int i = 0; i < 8191; i++) {
		adresses[i] = mm_instance.allocate(2);

		if (adresses[i] == nullptr) { 
			printf("cuidao");
		}
		adresses2[i] = mm_instance.allocate(4);
		if (adresses2[i] == nullptr) {
			printf("cuidao");
		}
	}
	*/
	void* debug1 = mm_instance.allocate(2);
	void* debug2 = mm_instance.allocate(4);
	void* debug3 = mm_instance.allocate(8);
	//void* debug4 = mm_instance.allocate(16);
	void* debug5 = mm_instance.allocate(32);

	void* last = mm_instance.allocate(2);

	/*
	for (unsigned int i = 0; i < 8191; i++) {
		mm_instance.deallocate(adresses[i]);
		mm_instance.deallocate(adresses2[i]);
	}
	*/

	mm_instance.deallocate(last);
	mm_instance.deallocate(adress2);
	mm_instance.deallocate(debug1);
	mm_instance.deallocate(debug2);
	mm_instance.deallocate(debug3);
	mm_instance.deallocate(debug5);

	return 0;
}