#include "string.h" 
#include "types.h" 

void memory_set(void *dest, uint8 val, uint32 len)
{
	uint8 *dst = (uint8 *)dest;

	for ( ; len != 0; len--) {
		*dst++ = val;
	}
}

uint16 str_len(string ch)
{
 	uint16 count = 0; 	 	// Changed counter to 0  
	while(ch[count++]); 
	return count; 	 	 	// changed counter to i instead of --i  
}

// Added in episode 3
// This function compares two strings 
// and returns true (1) if they are equal 
// or false (0) if they are not equal 
//
uint8 str_equal(string ch1, string ch2)
{
 	uint8 result = 1; 
	uint8 size = str_len(ch1); 

	if(size != str_len(ch2)) 
		result = 0; 
	else
	{
  	 	uint8 i = 0; 
		for(i; i<=size; i++)
		{
	 	 	if(ch1[i] != ch2[i]) 
				result = 0; 
		}
	}
	return result; 
}

