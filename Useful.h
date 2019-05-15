#pragma once
#include <string.h>
#include  <time.h>
#include  <conio.h>
#include  <iostream>
#include  <malloc.h>
#include  <fstream>
#include  <fstream>

using namespace std;

template <typename T>
void output_array(T* array, int size)
{
	if (array && size > 0)
		for (int i = 0; i < size; i++)
			cout << *(array + i);
}

//convert binary array to decade number
int binary_to_decade(bool* bi, size_t array_len)
{
	if (bi)
	{
		int decade = 0;
		int power = 1;
		for (int i = array_len - 1; i >= 0; i--, power *= 2)
			decade += *(bi + i) * power;
		return decade;
	}
	else return 0;
}

template <typename T>
void out_array(T* array, size_t number)
{
	if (array)
		for (int i = 0; i < number; i++)
			cout << *(array + i) << " ";
}

//return strind filled with this number of this character
char* give_some_chars(int number, char c)
{
	if (number > 0)
	{
		char* string = (char*)malloc((number + 1) * sizeof(char));
		for (int i = 0; i < number; i++)
			*(string + i) = c;
		*(string + number) = 0;
		return string;
	}
	else return nullptr;
}

//-1 if the less string on the left, 0 for equal, 1 - the less on the right. Nullptr is less
int string_compare(char *left, char *right)
{
	if (left && right)
	{
		if (strlen(right) && strlen(left))
		{
			int least_len = (strlen(left) < strlen(right)) ? strlen(left) : strlen(right);
			for (int i = 0; i < least_len; i++)
			{
				if (*(left + i) < *(right + i)) return -1;
				else
					if (*(left + i) > *(right + i)) return 1;
			}
			if (strlen(left) == strlen(right)) return 0;
			else
				if (least_len == strlen(left)) return -1;
				else return 1;
		}
		else
			if (!strlen(right) && !strlen(left)) return 0;
			else
				if (!strlen(left) && strlen(right)) return -1;
				else return 1;
	}
	else
	{
		if (!left && !right) return 0;
		else
			if (!left && right) return -1;
			else return 1;
	}
}

//copy string 
char *string_copy(char* string)
{
	if (string)
	{
		char *new_str = (char*)malloc((strlen(string) + 1) * sizeof(char));
		for (int i = 0; i < strlen(string); i++)
			*(new_str + i) = *(string + i);
		*(new_str + strlen(string)) = 0;
		return new_str;
	}
	else return nullptr;
}


bool is_in_string(char* string, char c)
{
	if (string)
	{
		for (int i = 0; i < strlen(string); i++)
			if (*(string + i) == c) return true;
	}
	return false;
}

//copy string 
template <typename T>
T *copy_array(T* string, size_t string_len)
{
	if (string)
	{
		T *new_str = (T*)malloc(string_len * sizeof(T));
		for (int i = 0; i < string_len; i++)
			*(new_str + i) = *(string + i);
		return new_str;
	}
	else return nullptr;
}

//add from after dest
template <typename T>
T  *array_concat(T * dest, T * from, size_t len_dest, size_t len_from)
{
	if (dest || from)
		if (dest)
		{
			if (from)
			{
				T  *new_str = (T *)malloc((len_dest + len_from + 1) * sizeof(T));
				for (int i = 0; i < len_dest; i++)
					*(new_str + i) = *(dest + i);
				for (int i = 0; i < len_from; i++)
					*(new_str + len_dest + i) = *(from + i);
				return new_str;
			}
			else return copy_array(dest, len_dest);
		}
		else return copy_array(from, len_from);
	else return nullptr;
}

char *string_concat(char* dest, char* from)
{
	if (dest)
	{
		if (from)
		{
			char *new_str = (char*)malloc((strlen(dest) + strlen(from) + 1) * sizeof(char));
			for (int i = 0; i < strlen(dest); i++)
				*(new_str + i) = *(dest + i);
			for (int i = 0; i < strlen(from); i++)
				*(new_str + strlen(dest) + i) = *(from + i);
			*(new_str + strlen(dest) + strlen(from)) = 0;
			return new_str;
		}
		else return string_copy(dest);
	}
	else return string_copy(from);
}

// return array as a line with '\n' symbols to look like array
char* straighten_array(char** array, int width, int height)
{
	char* line = nullptr;
	for (int i = 0; i < height; i++)
	{
		line = string_concat(line, array[i]);
		line = string_concat(line, "\n");
		line = string_concat(line, "\n"); //not necessary line, but more beautiful
	}
	return line;
}

char* inttostr(int i)
{
	if (i == 0)
	{
		char* nul = (char*)malloc(2 * sizeof(char));
		*nul = '0';
		*(nul + 1) = 0;
		return nul;
	}
	int minus = 0;
	if (i < 0) { i = -i; minus = 1; }
	int n;						//разряды
	int r = i;					//сохряняем значение i
	for (n = 0; i > 0; n++)
		i = i / 10;
	char *s = (char*)malloc((n + 1 + minus) * sizeof(char));
	for (int i = n - 1 + minus; i >= minus; i--, r /= 10)
		*(s + i) = r % 10 + 48;
	if (minus) *s = '-';
	*(s + n + minus) = '\0';
	return s;
}