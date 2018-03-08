#ifndef __libcfg_strbuf_h__
#define __libcfg_strbuf_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if !defined(LIBCFG_WCHAR_MODE) && defined(UNICODE)
#define LIBCFG_WCHAR_MODE	1
#endif

#if LIBCFG_WCHAR_MODE
typedef wchar_t		char_t;
#else
typedef char		char_t;
#endif

/* get string length */
extern size_t strlength(const char_t* s);
/* compare string */
extern int strcompare(const char_t* s1, const char_t* s2);
extern int strcompare_nocase(const char_t* s1, const char_t* s2);


typedef struct strbuf_t
{
	char_t*		buffer;
	size_t		length;
	size_t		capacity;
} strbuf_t;

/* create new strbuf */
extern strbuf_t* strbuf_init(void);
extern strbuf_t* strbuf_init_size(size_t size);

/* destroy strbuf */
extern void strbuf_free(strbuf_t* buf);

/* make sure buffer is at least "size" big. zero buffer anyhow */
extern void strbuf_resize(strbuf_t* buf, size_t size);

/* copy a string, return new string */
extern const char_t* strbuf_copy(strbuf_t* buf, const char_t* src);
extern const char_t* strbuf_copy_length(strbuf_t* buf, const char_t* src, size_t len);

/* strbuf is NULL or buffer is empty */
extern int strbuf_is_empty(const strbuf_t* buf);

/* compare string of two strbufs */
extern int strbuf_compare(const strbuf_t* buf1, const strbuf_t* buf2, int nocase);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __libcfg_strbuf_h__ */
