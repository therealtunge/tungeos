char* itoa(int value)
{
	static char buffer[12];
	char* ptr = buffer + sizeof(buffer) - 1;
	unsigned int uvalue;
	int negative = 0;
	*ptr = '\0';
	if(value < 0)
	{
		negative = 1;
		uvalue = (unsigned int)(-value);
	}
	else uvalue = (unsigned int)value;
	if(uvalue == 0)
	{
		*(--ptr) = '0';
	}
	else
	{
		while(uvalue)
		{
			*(--ptr) = '0' + (uvalue % 10);
			uvalue /= 10;
		}
	}
	if(negative)
		*(--ptr) = '-';
	return ptr;
}