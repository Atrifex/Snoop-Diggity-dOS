#include "syscalls.h"

asmlinkage int32_t halt(uint8_t status)
{
	return 0;	
}

asmlinkage int32_t execute(const uint8_t* command)
{
	return 0;	
}

asmlinkage int32_t read(int32_t fd, void* buf, int32_t num_bytes)
{
	return 0;
}

asmlinkage int32_t write(int32_t fd, const void* buf, int32_t num_bytes)
{
	return 0;
}

asmlinkage int32_t open(const uint8_t* filename)
{
	return 0;
}

asmlinkage int32_t close(int32_t fd)
{
	return 0;
}

asmlinkage int32_t getargs(uint8_t* buf, int32_t num_bytes)
{
	return 0;
}

asmlinkage int32_t vidmap(uint8_t** screen_start)
{
	return 0;
}
