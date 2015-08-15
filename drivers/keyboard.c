#include "keyboard.h" 

string read_str()
{
 	char buff; 
	string buffstr; 
	uint8 i = 0; 
	uint8 reading = 1; 

	while(reading)
	{
		// If "inportb(0x64) & 0x01" is true , it means that users press the keys  
  	 	if(inportb(0x64) & 0x01) 	
		{
			switch(inportb(0x60)) 
			{
 	 	 	 	case 1:  
					printch((char)27, rc_green); 	//Escape button 
					buffstr[i] = (char)27; 
					i++; 
					break;  
				case 2: 
					printch('1', rc_green);  
					buffstr[i] = '1'; 
					i++; 
					break; 
				case 3: 
					printch('2', rc_green);  
					buffstr[i] = '2'; 
					i++; 
					break; 
				case 4: 
					printch('3', rc_green);  
					buffstr[i] = '3'; 
					i++; 
					break; 
				case 5: 
					printch('4', rc_green);  
					buffstr[i] = '4'; 
					i++; 
					break; 
				case 6: 
					printch('5', rc_green);  
					buffstr[i] = '5'; 
					i++; 
					break; 
				case 7: 
					printch('6', rc_green);  
					buffstr[i] = '6'; 
					i++; 
					break; 
				case 8: 
					printch('7', rc_green);  
					buffstr[i] = '7'; 
					i++; 
					break; 
				case 9: 
					printch('8', rc_green);  
					buffstr[i] = '8'; 
					i++; 
					break; 
				case 10: 
					printch('9', rc_green);  
					buffstr[i] = '9'; 
					i++; 
					break; 
				case 11: 
					printch('0', rc_green);  
					buffstr[i] = '0'; 
					i++; 
					break; 
				case 12: 
					printch('-', rc_green);  
					buffstr[i] = '-'; 
					i++; 
					break; 
				case 13: 
					printch('=', rc_green);  
					buffstr[i] = '-'; 
					i++; 
					break; 
				case 14: 
					printch('\b', rc_green);  
					buffstr[i] = '\b'; 
					i++; 
					break; 
			/*  case 15: 
					printch('\t', rc_green);  	 Tab button  
					buffstr[i] = '\t'; 
					i++; 
					break; 
			 */ 
				case 16: 
					printch('q', rc_green);  
					buffstr[i] = 'q'; 
					i++; 
					break; 
				case 17: 
					printch('w', rc_green);  
					buffstr[i] = 'w'; 
					i++; 
					break; 
				case 18: 
					printch('e', rc_green);  
					buffstr[i] = 'e'; 
					i++; 
					break; 
				case 19: 
					printch('r', rc_green);  
					buffstr[i] = 'r'; 
					i++; 
					break; 
				case 20: 
					printch('t', rc_green);  
					buffstr[i] = 't'; 
					i++; 
					break; 
				case 21: 
					printch('y', rc_green);  
					buffstr[i] = 'y'; 
					i++; 
					break; 
				case 22: 
					printch('u', rc_green);  
					buffstr[i] = 'u'; 
					i++; 
					break; 
				case 23: 
					printch('i', rc_green);  
					buffstr[i] = 'i'; 
					i++; 
					break; 
				case 24: 
					printch('o', rc_green);  
					buffstr[i] = 'o'; 
					i++; 
					break; 
				case 25: 
					printch('p', rc_green);  
					buffstr[i] = 'p'; 
					i++; 
					break; 
				case 26: 
					printch('[', rc_green);  
					buffstr[i] = '['; 
					i++; 
					break; 				
				case 27: 
					printch(']', rc_green);  
					buffstr[i] = ']'; 
					i++; 
					break; 
				case 28: 
					//printch('\n', rc_green);  
					//buffstr[i] = '\n'; 
					i++; 
					reading = 0; 
					break; 
			/* case 29: 
					printch('L CTRL', rc_green);   	 Left Control  
					buffstr[i] = 'L CTRL'; 
					i++; 
					break; 
				case 29: 	 	 	 	 	 Right Control  
					printch('R CTRL', rc_green);  
					buffstr[i] = 'R CTRL'; 
					i++; 
					break; 
			 */ 
				case 30: 
					printch('a', rc_green);  
					buffstr[i] = 'a'; 
					i++; 
					break; 
				case 31: 
					printch('s', rc_green);  
					buffstr[i] = 's'; 
					i++; 
					break; 
				case 32: 
					printch('d', rc_green);  
					buffstr[i] = 'd'; 
					i++; 
					break; 
				case 33: 
					printch('f', rc_green);  
					buffstr[i] = 'f'; 
					i++; 
					break; 
				case 34: 
					printch('g', rc_green);  
					buffstr[i] = 'g'; 
					i++; 
					break; 
				case 35: 
					printch('h', rc_green);  
					buffstr[i] = 'h'; 
					i++; 
					break; 
				case 36: 
					printch('j', rc_green);  
					buffstr[i] = 'j'; 
					i++; 
					break; 
				case 37: 
					printch('k', rc_green);  
					buffstr[i] = 'k'; 
					i++; 
					break; 
				case 38: 
					printch('l', rc_green);  
					buffstr[i] = 'l'; 
					i++; 
					break; 
				case 39: 
					printch(';', rc_green);  
					buffstr[i] = ';'; 
					i++; 
					break; 
				case 40: 
					printch((char)44, rc_green);  	// Single quote (')  
					buffstr[i] = (char)44; 
					i++; 
					break; 
				case 41: 
					printch('`', rc_green);  
					buffstr[i] = '`'; 
					i++; 
					break; 
			/*	case 42: 	 	 	 	   // Left Shift  
					printch('L SHIFT', rc_green);  
					buffstr[i] = 'L SHIFT'; 
					i++; 
					break;
				case 43:  
					printch((char)92, rc_green);     // \ () for somekeyboards  
					buffstr[i] = 'q'; 
					i++; 
					break; 
			*/  
				case 44: 
					printch('z', rc_green);  
					buffstr[i] = 'z'; 
					i++; 
					break; 
				case 45: 
					printch('x', rc_green);  
					buffstr[i] = 'x'; 
					i++; 
					break; 
				case 46: 
					printch('c', rc_green);  
					buffstr[i] = 'c'; 
					i++; 
					break; 
				case 47: 
					printch('v', rc_green);  
					buffstr[i] = 'v'; 
					i++; 
					break; 
				case 48: 
					printch('b', rc_green);  
					buffstr[i] = 'b'; 
					i++; 
					break; 
				case 49: 
					printch('n', rc_green);  
					buffstr[i] = 'n'; 
					i++; 
					break; 
				case 50: 
					printch('m', rc_green);  
					buffstr[i] = 'm'; 
					i++; 
					break; 
				case 51: 
					printch(',', rc_green);  
					buffstr[i] = ','; 
					i++; 
					break; 
				case 52: 
					printch('.', rc_green);  
					buffstr[i] = '.'; 
					i++; 
					break;   
				case 53: 
					printch('/', rc_green);  
					buffstr[i] = '/'; 
					i++; 
					break; 
				case 54: 
					printch('.', rc_green);  
					buffstr[i] = '.'; 
					i++; 
					break; 
				case 55: 
					printch('/', rc_green);  
					buffstr[i] = '/'; 
					i++; 
					break; 
			/*  case 56: 
					printch('R SHIFT', rc_green);   	// Right SHIFT  
					buffstr[i] = '.'; 
					i++; 
					break; 
			 */ 
				case 57: 
					printch(' ', rc_green);  
					buffstr[i] = ' '; 
					i++; 
					break; 
			}
		}
	}
	buffstr[i] = '\0'; 
	return buffstr; 
}

