# Memory Manager C++

Memory Manager developed in C++ with preallocated memory blocks and memory leaks control.

## How to use

Easy steps to use this Memory Manager

### Create instance 

```C++
MemoryManager& mm_instance = MemoryManager::get_instance();
```

### Initialise

You can create an file.ini to set memoryblocks size and number, there is an example in ./Sandbox/memory.ini
Example:

__
1			= 32
2			= 32
4			= 32
8			= 16
16			= 8
32			= 2
__

size 1 with 32 blocks
size 2 with 32 blocks
size 4 with 32 blocks
size 8 with 16 blocks
size 16 with 8 blocks
size 32 with 2 blocks

```C++
mm_instance.init("mini_memory.ini");
```

### Allocate and Deallocate

Call allocate method with size of the block
```C++
void* adress = mm_instance.allocate(2);
```
Size should be power of 2, if not, next power of 2 will be seted
If there is not a block of that size avaliable, next power of 2 block will be selected and subdivided 
If there is no more memory available, a message will be throwed

Call deallocate method with the adress
```C++
mm_instance.deallocate(adress);
```


## Memory leaks

If you do not deallocate a block of memory previously allocated, a message will be throwed with the adress and size of the block not deallocated
```C++
Deallocating memory...
Memory leak in adress [0xdce24b30] with size of: 32 bytes
```

Otherwise, a succesfull message wil be throwed
```C++
Deallocating memory...
*** No memory leaks founded ***
```

All memory blocks will be deallocated at the end.