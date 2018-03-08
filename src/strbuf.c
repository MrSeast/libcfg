#include <stdlib.h>
#include <string.h>

#include "strbuf.h"

#define _new(T)			(T*)calloc(1, sizeof(T)) /* zeroed */
#define _newstr(L)		(char_t*)calloc(L, sizeof(char_t)) /* zeroed */
#define _delete(P)		free((void*)(P))
#define _zerostr(P,L)	memset((void*)(P), 0, sizeof(char_t)*(L))


size_t strlength(const char_t* s)
{
#if LIBCFG_WCHAR_MODE
	return wcslen(s);
#else
	return strlen(s);
#endif
}

int strcompare(const char_t* s1, const char_t* s2)
{
#if LIBCFG_WCHAR_MODE
	return wcscmp(s1, s2);
#else
	return strcmp(s1, s2);
#endif
}

int strcompare_nocase(const char_t* s1, const char_t* s2)
{
#if LIBCFG_WCHAR_MODE
	return _wcsicmp(s1, s2);
#else
	return stricmp(s1, s2);
#endif
}


strbuf_t* strbuf_init(void)
{
	return _new(strbuf_t);
}

strbuf_t* strbuf_init_size(size_t size)
{
	strbuf_t* buf = strbuf_init();
	if (buf != NULL && size > 0)
	{
		buf->buffer = _newstr(size);
		buf->capacity = buf->buffer ? size : 0;
	}
	return buf;
}

void strbuf_free(strbuf_t* buf)
{
	if (buf)
	{
		_delete(buf->buffer);
		_delete(buf);
	}
}

void strbuf_resize(strbuf_t* buf, size_t size)
{
	if (!buf) return;

	if (0 == size) size = 1;

	if (size > buf->capacity)
	{
		if (buf->buffer) _delete(buf->buffer);
		buf->buffer = _newstr(size);
		buf->length = 0;
		buf->capacity = buf->buffer ? size : 0;
	}
	else
	{
		_zerostr(buf->buffer, buf->capacity);
		buf->length = 0;
	}
}

const char_t* strbuf_copy(strbuf_t* buf, const char_t* src)
{
	return strbuf_copy_length(buf, src, src ? strlength(src) : 0);
}

const char_t* strbuf_copy_length(strbuf_t* buf, const char_t* src, size_t len)
{
	if (!buf || !src) return NULL;

	strbuf_resize(buf, len + 1);

	if (buf->buffer != NULL && len > 0)
	{
		memcpy(buf->buffer, src, len);
		buf->length = len;
	}

	return buf->buffer;
}

int strbuf_is_empty(const strbuf_t* buf)
{
	return (NULL == buf || 0 == buf->length);
}

int strbuf_compare(const strbuf_t* a, const strbuf_t* b, int nocase)
{
	if (strbuf_is_empty(a))
	{
		if (strbuf_is_empty(b))
			return 0;
		else
			return -1;
	}
	else if (strbuf_is_empty(b))
	{
		return 1;
	}

	return nocase ? strcompare_nocase(a->buffer, b->buffer) : strcompare(a->buffer, b->buffer);
}

int strbuf_compare_string(const strbuf_t* buf, const char_t* str, int nocase)
{
	if (strbuf_is_empty(buf))
	{
		if (NULL == str)
			return 0;
		else
			return -1;
	}
	else if (NULL == str)
	{
		return 1;
	}

	return nocase ? strcompare_nocase(buf->buffer, str) : strcompare(buf->buffer, str);
}
