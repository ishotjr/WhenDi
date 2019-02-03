#include "ds1307.h"

static inline int DS1307_isupper(char c)
{
    return (c >= 'A' && c <= 'Z');
}

static inline int DS1307_isalpha(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}


static inline int DS1307_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\12');
}

static inline int DS1307_isdigit(char c)
{
    return (c >= '0' && c <= '9');
}

/*
 * Convert a string to a long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
long DS1307_strtol( const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (DS1307_isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	} else if ((base == 0 || base == 2) &&
	    c == '0' && (*s == 'b' || *s == 'B')) {
		c = s[1];
		s += 2;
		base = 2;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)0xffffffffffffffff : 0xefffffffffffffff;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (DS1307_isdigit(c))
			c -= '0';
		else if (DS1307_isalpha(c))
			c -= DS1307_isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? 0xffffffffffffffff : 0xefffffffffffffff;
//		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *)(any ? s - 1 : nptr);
	return (acc);
}

/**
* 
*
* @param
* @return
*/
void DS1307_WriteRegister(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char addr, unsigned char value)
{
	
	if(addr >= DS1307_HIGH_RAM_ADDR)
	{
		return;
	}
	
	ATMO_I2C_MasterWrite(i2cInstance, i2cAddr, &addr, 1, &value, 1, 0);
}

/**
* 
*
* @param
* @return
*/
void DS1307_WriteRegisters(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char addr, unsigned char *buffer, unsigned char length)
{
}

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_ReadRegister(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char addr)
{
	if(addr >= DS1307_HIGH_RAM_ADDR)
	{
		return 0;
	}
	
	unsigned char readBytes[1];

	ATMO_I2C_MasterRead(i2cInstance, i2cAddr, &addr, 1, readBytes, 1, 0);
	
	return readBytes[0];
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetSQWOutput(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, DS1307_SQW_Setting value)
{
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_CONTROL_ADDR, value);
}

void DS1307_StopClock(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR, DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR) | 0x80);
}

void DS1307_StartClock(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR, DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR) & 0x7F);
}

void DS1307_ResetClock(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR, 0x80);
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_MINUTES_ADDR, 0x00);
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_HOURS_ADDR, 0x00);
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_DAY_ADDR, 0x00);
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_DATE_ADDR, 0x00);
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_MONTH_ADDR, 0x00);
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_YEAR_ADDR, 0x00);
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_CONTROL_ADDR, 0x00);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetSeconds(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	unsigned char chBit = DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR) & 0x80;
	
	value = value % 60;

	unsigned char tens = value / 10;
	unsigned char ones = value - (tens * 10);
	
	value = chBit | (tens << 4) | ones;
	
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR, value);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetMinutes(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	value = value % 60;

	unsigned char tens = value / 10;
	unsigned char ones = value - (tens * 10);
	
	value = (tens << 4) | ones;
	
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_MINUTES_ADDR, value);
}

void DS1307_SetHour(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	DS1307_SetHours(i2cInstance, i2cAddr, value);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetHours(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	bool ampm = false;
	
	if(value >= 12)
	{
		ampm = true;
	}
	
	value = value % 24;
	
	unsigned char tens = (value % 12) / 10;
	unsigned char ones = (value % 12) - (tens * 10);
	
	
	value = 0x40 | (ampm << 5) | (tens << 4) | (ones);
	
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_HOURS_ADDR, value);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetDay(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	value = value % 7;
	
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_DAY_ADDR, value);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetDate(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	//This is only a very very basic sanity check.
	value = value % 31;

	unsigned char tens = value / 10;
	unsigned char ones = value - (tens * 10);
	
	value = (tens << 4) | ones;
	
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_DATE_ADDR, value);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetMonth(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	value = value % 12;

	unsigned char tens = value / 10;
	unsigned char ones = value - (tens * 10);
	
	value = (tens << 4) | ones;
	
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_MONTH_ADDR, value);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetYear(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned int value)
{
	//If the user wants to enter it in a complete form instead of just the last two digits then
	//this should do the trick.
	if(value >= DS1307_EPOCH_YEAR && value < DS1307_EPOCH_YEAR + 100)
	{
		value = value - DS1307_EPOCH_YEAR;
	}
	
	else
	{
		value = value % 99;
	}

	unsigned char tens = value / 10;
	unsigned char ones = value - (tens * 10);
	
	value = (tens << 4) | ones;
	
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_YEAR_ADDR, value);
}

/**
* 
*
* @param
* @return
*/
void DS1307_SetControl(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value)
{
	//Masking out the unsafe stuff
	value = value & 0x93;
	DS1307_WriteRegister(i2cInstance, i2cAddr, DS1307_CONTROL_ADDR, value);
}

/**
* 
*
* @param
* @return
*/
DS1307_SQW_Setting DS1307_GetSQWOutput(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	return (DS1307_SQW_Setting)DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_CONTROL_ADDR);
}

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetSeconds(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	unsigned char value = DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_SECONDS_ADDR);
	return (((value & 0x70) >> 4) * 10) + (value & 0x0F);
}

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetMinutes(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	unsigned char value = DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_MINUTES_ADDR);
	return (((value & 0x70) >> 4) * 10) + (value & 0x0F);
}


/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetHours(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	unsigned char value = DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_HOURS_ADDR);
	
	bool ampm = (value & 0x20) >> 5;
	unsigned char tens = (value & 0x10) >> 4;
	unsigned char ones = (value & 0x0F);
	
	return ((ampm * 12) + (tens * 10) + ones) % 24;
}

unsigned char DS1307_GetHour(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	return DS1307_GetHours(i2cInstance, i2cAddr);
}

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetDay(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	return DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_DAY_ADDR);
}

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetDate(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	unsigned char value = DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_DATE_ADDR);
	return (((value & 0x30) >> 4) * 10) + (value & 0x0F);
}

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetMonth(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	unsigned char value = DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_MONTH_ADDR);
	
	unsigned char tens = (value & 0x10) >> 4;
	unsigned char ones = (value & 0x0F);
	
	return (tens * 10) + ones;
}

/**
* 
*
* @param
* @return
*/
unsigned int DS1307_GetYear(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	unsigned char value = DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_YEAR_ADDR);
	
	unsigned char tens = value >> 4;
	unsigned char ones = (value & 0x0F);
	
	return (tens * 10) + ones + DS1307_EPOCH_YEAR;
}

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetControl(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr)
{
	return DS1307_ReadRegister(i2cInstance, i2cAddr, DS1307_CONTROL_ADDR);
}


/**
* 
*
* @param
* @return
*/
void DS1307_GetISO8601Time(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, char *isoRetBuffer)
{
	unsigned int year = DS1307_GetYear(i2cInstance, i2cAddr);
	unsigned char month = DS1307_GetMonth(i2cInstance, i2cAddr);
	unsigned char date = DS1307_GetDate(i2cInstance, i2cAddr);
	unsigned char hours = DS1307_GetHours(i2cInstance, i2cAddr);
	unsigned char minutes = DS1307_GetMinutes(i2cInstance, i2cAddr);
	unsigned char seconds = DS1307_GetSeconds(i2cInstance, i2cAddr);
	
	//P[YYYY]-[MM]-[DD]T[hh]:[mm]:[ss]
	sprintf(isoRetBuffer, "%04d-%02d-%02dT%02d:%02d:%02d", year, month, date, hours, minutes, seconds);
	
	return isoRetBuffer;
}

void DS1307_SetISO8601Time(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, char *value)
{
	unsigned int year = 0;
	unsigned char month = 0;
	unsigned char date = 0;
	unsigned char hours = 0;
	unsigned char minutes = 0;
	unsigned char seconds = 0;
	char *end;
	
	year = DS1307_strtol(value, &end, 10);
	month = DS1307_strtol(end + 1, &end, 10);
	date = DS1307_strtol(end + 1, &end, 10);
	hours = DS1307_strtol(end + 1, &end, 10);
	minutes = DS1307_strtol(end + 1, &end, 10);
	seconds = DS1307_strtol(end + 1, &end, 10);
	
	DS1307_SetYear(i2cInstance, i2cAddr, year);
	DS1307_SetMonth(i2cInstance, i2cAddr, month);
	DS1307_SetDate(i2cInstance, i2cAddr, date);
	DS1307_SetHours(i2cInstance, i2cAddr, hours);
	DS1307_SetMinutes(i2cInstance, i2cAddr, minutes);
	DS1307_SetSeconds(i2cInstance, i2cAddr, seconds);
	
	return;
}

void DS1307_SetDateTime(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned int year, unsigned char month, unsigned char date, unsigned char hours, unsigned char minutes, unsigned char seconds)
{
	DS1307_SetYear(i2cInstance, i2cAddr, year);
	DS1307_SetMonth(i2cInstance, i2cAddr, month);
	DS1307_SetDate(i2cInstance, i2cAddr, date);
	DS1307_SetHours(i2cInstance, i2cAddr, hours);
	DS1307_SetMinutes(i2cInstance, i2cAddr, minutes);
	DS1307_SetSeconds(i2cInstance, i2cAddr, seconds);
	
	return;
}
