#include "string.h" 
#include "types.h" 

void memory_set(void *dest, uint8 val, uint32 len)
{
	uint8 *dst = (uint8 *)dest;

	for ( ; len != 0; len--) {
		*dst++ = val;
	}
}

/** 
 * K&R implementation          
 */
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';        
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';   
    str[i] = '\0';

    /* TODO: implement "reverse" */ 
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

void append(char s[], char n)
{
	int len = str_len(s); 
	s[len] = n; 
	s[len + 1] = '\0'; 
}

void backspace(char s[])
{
	int len = str_len(s); 
	s[len - 1] = '\0'; 
}

int strcmp(char s1[], char s2[])
{
	int i; 
	for (i = 0; s1[i] == s2[i]; i++)
	{
		if(s1[i] == '\0')
			return 0; 
	}
	
	return s1[i] -s2[i]; 
}
