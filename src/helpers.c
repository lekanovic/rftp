#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int rm_crlf(char* str)
{
	int len,ret=0;
	len = strlen(str);

	if (str[len-2] == '\r' && str[len-1] == '\n') {
		str[len-2] = str[len-1] = '\0';
		ret = 1;
	}

	return ret;
}

void replace(char*ip)
{
	int i;
	char *tmp = ip;
	for (i=0; tmp[i] != '\0';i++) {
		if( tmp[i] == '.')
			tmp[i] = ',';
	}
}

/*
 * Description:
 *   Find and replace text within a string.
 *
 * Parameters:
 *   src  (in) - pointer to source string
 *   from (in) - pointer to search text
 *   to   (in) - pointer to replacement text
 *
 * Returns:
 *   Returns a pointer to dynamically-allocated memory containing string
 *   with occurences of the text pointed to by 'from' replaced by with the
 *   text pointed to by 'to'.
 * http://www.daniweb.com/software-development/c/code/216517
 */
char *replace_str(const char *src, const char *from, const char *to)
{
	/*
	* Find out the lengths of the source string, text to replace, and
	* the replacement text.
	*/
	size_t size    = strlen(src) + 1;
	size_t fromlen = strlen(from);
	size_t tolen   = strlen(to);

	char *value = malloc(size);

	char *dst = value;

	if (value == NULL) return NULL;

	for ( ;; ) {
		/*
		* Try to find the search text.
		*/
		const char *match = strstr(src, from);
		if ( match != NULL ) {
			/*
			* Found search text at location 'match'. :)
			* Find out how many characters to copy up to the 'match'.
			*/
			size_t count = match - src;
			/*
			* We are going to realloc, and for that we will need a
			* temporary pointer for safe usage.
			*/
			char *temp;
			/*
			* Calculate the total size the string will be after the
			* replacement is performed.
			*/
			size += tolen - fromlen;
			/*
			* Attempt to realloc memory for the new size.
			*/
			temp = realloc(value, size);
			if ( temp == NULL ) {
				/*
				* Attempt to realloc failed. Free the previously malloc'd
				* memory and return with our tail between our legs. :(
				*/
				free(value);
				return NULL;
			}
			/*
			* The call to realloc was successful. :) But we'll want to
			* return 'value' eventually, so let's point it to the memory
			* that we are now working with. And let's not forget to point
			* to the right location in the destination as well.
			*/
			dst = temp + (dst - value);
			value = temp;
			/*
			* Copy from the source to the point where we matched. Then
			* move the source pointer ahead by the amount we copied. And
			* move the destination pointer ahead by the same amount.
			*/
			memmove(dst, src, count);
			src += count;
			dst += count;
			/*
			* Now copy in the replacement text 'to' at the position of
			* the match. Adjust the source pointer by the text we replaced.
			* Adjust the destination pointer by the amount of replacement
			* text.
			*/
			memmove(dst, to, tolen);
			src += fromlen;
			dst += tolen;
		} else {
			/*
			* Copy any remaining part of the string. This includes the null
			* termination character.
			*/
			strcpy(dst, src);
			break;
		}
	}

	return value;
}


