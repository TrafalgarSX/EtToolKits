#ifndef _ETTYPEDEF_H_

#define _ETTYPEDEF_H_



#ifndef _COMPILER_VER_

#	if defined(_DEBUG)

#		define _COMPILER_MODE_			dbg

#	else

#		define _COMPILER_MODE_			rls

#	endif

#	if defined _MSC_VER

#		if _MSC_VER > 1928

#			define _COMPILER_VER_       vc17

#			define _COMPILER_NAME_      VS2022

#		elif _MSC_VER == 1928

#		    define _COMPILER_VER_       vc16

#		    define _COMPILER_NAME_      VS2019

#		elif _MSC_VER == 1910

#		    define _COMPILER_VER_       vc15

#		    define _COMPILER_NAME_      VS2017

#		elif _MSC_VER == 1900

#		    define _COMPILER_VER_       vc14

#		    define _COMPILER_NAME_      VS2015

#		elif _MSC_VER == 1800

#		    define _COMPILER_VER_       vc12

#		    define _COMPILER_NAME_      VS2013

#		elif _MSC_VER == 1700

#		    define _COMPILER_VER_       vc11

#		    define _COMPILER_NAME_      VS2012

#		elif _MSC_VER == 1600

#		    define _COMPILER_VER_       vc10

#		    define _COMPILER_NAME_      VS2010

#		elif _MSC_VER == 1500

#		    define _COMPILER_VER_       vc9

#		    define _COMPILER_NAME_		VS2008

#		else

#		    error "!!!!!Unsupport visual studio version!!!!!"

#		endif

#	elif defined(__GNUC__)

#		define _COMPILER_VER_			__GNUC__

#		ifdef __GNUG__

#			define _COMPILER_NAME_		G++

#		else

#			define _COMPILER_NAME_		GCC

#		endif

#	elif defined(_CLANG_)

#		define _COMPILER_VER_			_CLANG_

#		define _COMPILER_NAME_			CLANG

#	else

#			error "!!!!!Unsupport compiler version!!!!!"

#	endif

#	define CPPVERSION(version)  ((__cplusplus >= version)||(defined(_MSVC_LANG) &&  (_MSVC_LANG >= version)))

#endif



#if !defined(_MSC_VER) || _MSC_VER >= 1910

#	include <stdint.h>

#endif



#if defined(_MSC_VER)

#	if defined(_WIN64) && !defined(_X64_)

#		define _X64_

#	elif !defined(_X32_)

#		define _X32_

#	endif

#elif defined(__GNUC__)

#	if	(defined(__amd64__) || defined(__x86_64__) || \

	defined(__ppc64__) || defined(__powerpc64__) || \

	defined(__ia64__)  || defined(__alpha__)     || \

	(defined(__sparc__) && defined(__arch64__)) || \

	defined(__s390x__) || defined(__mips64)     || \

	defined(__aarch64__)) && !defined(_X64_)

#		define _X64_

#	elif !defined(_X32_)

#		define _X32_

#	endif

#endif



#ifndef MAX

#	define MAX(x, y) (((x)>(y))?(x):(y))

#endif



#ifndef MIN

#	define MIN(x, y) (((x)<(y))?(x):(y))

#endif



// 以下四个宏定义为了通过android/IOS/Linux平台编译，三个平台底层是类UNIX系统，使用UNIX_LIKE定义使其支持某些功能，采用宏定义注释部分代码



#ifndef _OS_TYPE_

#	define	_OS_TYPE_

#	if defined(WIN32)

#		ifndef _OS_WINDOWS_

#			define	_OS_WINDOWS_

#		endif

#	endif

#endif



#if defined(_OS_WINDOWS_)

#	ifdef _WIN32_WINNT

#		undef	_WIN32_WINNT

#	endif

#	define _WIN32_WINNT		0x0501

#elif defined(ANDROID)

#	ifndef _OS_ANDROID_

#		define _OS_ANDROID_

#	endif

#	ifndef _UNIX_LIKE_

#		define _UNIX_LIKE_

#	endif

#elif defined (_OS_IOS_)

#	ifndef _UNIX_LIKE_

#		define _UNIX_LIKE_

#	endif

#elif defined (_OS_LINUX_)

#	ifndef _UNIX_LIKE_

#		define _UNIX_LIKE_

#	endif

#endif



#ifndef MAX_PATH

#   ifndef _OS_WINDOWS_

#       define MAX_PATH     PATH_MAX

#   else

#       define MAX_PATH		260

#   endif

#endif



// #ifndef NULL

// #	define	NULL			((void*)0x00)

// #endif



#ifndef OUT

#	define	OUT

#endif



#ifndef IN

#	define	IN

#endif



#ifndef INOUT

#	define	INOUT

#endif



#ifndef OPT

#	define	OPT

#endif



#ifndef NULL

#	ifdef __cplusplus

#		if !defined(__MINGW32__) && !defined(_MSC_VER)

#			define NULL __null

#		else

#			define NULL 0

#		endif

#	else

#		define NULL ((void*)0)

#	endif

#endif



// 因为OC语言定义了基本类型BOOL，混编时会冲突，Android目前未发现问题，所以编译iOS时注释掉

#ifndef _OS_IOS_	

#	ifdef __GNUC__

#		ifndef _BOOLDEF_

#			define _BOOLDEF_

typedef enum BOOLTag

{

	FALSE = (1 == 0),

	TRUE = (1 == 1)

}BOOL;

#		endif

#	endif

#endif



#ifndef _ENCHARSET_

#define _ENCHARSET_

// KEY charset & language

typedef enum EnCharSet

{

	CHAR_SET_UNKNOWN	= -1,

	CHAR_SET_UTF8		= 0,

	CHAR_SET_GBK		= 1,

	CHAR_SET_GB18030	= 2,

	CHAR_SET_GB2312		= 3,

}EnCharSet;

#endif



#ifndef _ENLANGID_

#define _ENLANGID_

typedef enum EnLangId

{

	LANGUAGE_ID_UNKNOWN				= -1,

	LANGUAGE_ID_SIMPLIFIED_CHINESE	= 0,

	LANGUAGE_ID_ENGLISH				= 1,

	LANGUAGE_ID_TRADITIONAL_CHINESE = 2,

	LANGUAGE_ID_FRENCH				= 3,

	LANGUAGE_ID_RUSSIAN				= 4,

	LANGUAGE_ID_JAPANESE			= 5,

	LANGUAGE_ID_INDONESIAN			= 6,

	LANGUAGE_ID_GERMAN				= 7,

	LANGUAGE_ID_ARABIC				= 8,

	LANGUAGE_ID_KOREAN				= 9,

	LANGUAGE_ID_KAZAKH				= 10,

	LANGUAGE_ID_THAI				= 11,

	LANGUAGE_ID_POLISH				= 12,

}EnLangId;

#endif



#ifndef INT64_C

#	ifdef _OS_WINDOWS_

#		define INT64_C(v) (v ## i64)

#	else

#		define INT64_C(v) (v ## LL)

#	endif

#endif



#ifndef UINT64_C

#	ifdef _OS_WINDOWS_

#	define UINT64_C(v) (v ## ui64)

#	else

#	define UINT64_C(v) (v ## ULL)

#	endif

#endif



#ifndef ROL32

// 32位无符号整型循环左移

#	define ROL32(a, offset) ( ( (u4)(a) << ((offset)&31) ) | ( (u4)(a) >> ( 32 - ((offset)&31)) ) )

#endif



#ifndef ROR32

// 32位无符号整型循环右移

#	define ROR32(a, offset) ( ( (u4)(a) >> ((offset)&31) ) | ( (u4)(a) << ( 32 - ((offset)&31)) ) )

#endif



#ifndef ROL16

// 16位无符号整型循环左移

#	define ROL16(a, offset) ( ( (u2)(a) << ((offset)&15) ) | ( (u2)(a) >> ( 16 - ((offset)&15)) ) )

#endif



#ifndef ROR16

// 16位无符号整型循环右移

#	define ROR16(a, offset) ( ( (u2)(a) >> ((offset)&15) ) | ( (u2)(a) << ( 16 - ((offset)&15)) ) )

#endif



#ifndef _BEGIN_C_DECL_

#	if defined(__cplusplus)

#		define	_BEGIN_C_DECL_	extern "C" {

#		define	_END_C_DECL_	};

#		define  EXTERN_C extern "C"

#	else

#		define	_BEGIN_C_DECL_

#		define	_END_C_DECL_

#		define  EXTERN_C extern

#	endif

#endif

#define ES_LOG(x) 

#ifndef IF_FAIL_GOTO_END

#	define	IF_FAIL_GOTO_END(c)						if ( c ){ES_LOG(( LOG_TYPE_ERROR, FILE_AND_LINE, "!!!Error!!!"));goto END;}

#	define	IF_FAIL_BREAK(c)						if ( c ){ES_LOG(( LOG_TYPE_ERROR, FILE_AND_LINE, "!!!Error!!!, u32Result = 0x%08X", u32Result));break;}

#endif

#ifndef IF_FAIL_SET_ERROR_GOTO_END

#	define	IF_FAIL_SET_ERROR_GOTO_END(c, var, ret)	if ( c ){var=ret; goto END;}

#endif



#ifndef VAR_OFFSET

#	define VAR_OFFSET(structname,var)				((size_t)(&((structname *)NULL)->var))

#endif



#ifndef VAR_SIZEOF

#	define VAR_SIZEOF(structname,var)				(sizeof(((structname *)NULL)->var))

#endif



#ifdef VAR_COUNTOF

#	define VAR_COUNTOF(structname,var)				(_countof(((structname *)NULL)->var))

#endif



#ifndef _countof

#	define _countof(array)							(sizeof(array)/sizeof(array[0]))

#endif



#ifdef VAR_NEXT_OFFSET

#	define VAR_NEXT_OFFSET(structname,var)			(((size_t)(&((structname *)NULL)->var)) + sizeof(((structname *)NULL)->var))

#endif



#ifndef _U1U2U4_

#	define _U1U2U4_

typedef unsigned char		u1;

typedef unsigned short		u2;

typedef signed char			s1;

typedef signed short		s2;

#	if defined(_OS_WINDOWS_) || defined(_OS_WPHONE_)

typedef unsigned long	    u4;

typedef signed long		    s4;

typedef unsigned __int64    u64t;

typedef signed __int64      s64t;

#	else

typedef unsigned int	    u4;

typedef signed int		    s4;

typedef unsigned long long  u64t;

typedef signed long long    s64t;

#	endif

#endif	// END _U1U2U4_



#ifndef _ETTYPE_

#define _ETTYPE_

typedef unsigned char	u8;

typedef signed short	s8;

typedef unsigned short	u16;

typedef signed short	s16;

#	if defined(_OS_WINDOWS_) || defined(_OS_WPHONE_)

typedef unsigned long	    u32;

typedef signed long		    s32;

typedef unsigned __int64    u64;

typedef signed __int64      s64;

#	else

typedef unsigned int	    u32;

typedef signed int		    s32;

typedef unsigned long long  u64;

typedef signed long long    s64;

#	endif

#endif

#ifndef BYTE2LONG

#	define BYTE2LONG(pu8Data)						(((u32)((pu8Data)[0])<<24) + ((u32)((pu8Data)[1])<<16) + ((u16)((pu8Data)[2])<<8) + (u8)((pu8Data)[3]))

#endif

#ifndef BYTE2WORD

#	define BYTE2WORD(pu8Data)						((u16)(((u16)((pu8Data)[0])<<8) + (u8)((pu8Data)[1])))

#endif

#ifndef LONG2BYTE

#	define LONG2BYTE(pu8Data,u32Data)				((u8*)(pu8Data))[0] = (u8)((u32Data)>>24);((u8*)(pu8Data))[1] = (u8)((u32Data)>>16);((u8*)pu8Data)[2] = (u8)((u32Data)>>8);((u8*)pu8Data)[3] = (u8)((u32Data))

#endif

#ifndef WORD2BYTE

#	define WORD2BYTE(pu8Data,u16Data)				((u8*)(pu8Data))[0] = (u8)((u16Data)>>8);((u8*)(pu8Data))[1] = (u8)((u16Data))

#endif

#ifndef BYTE2U4

#	define BYTE2U4(pu8Data)							(((u32)((pu8Data)[0])<<24) + ((u32)((pu8Data)[1])<<16) + ((u16)((pu8Data)[2])<<8) + (u8)((pu8Data)[3]))

#endif

#ifndef U42BYTE

#	define U42BYTE(pu8Data,u32Data)					((u8*)(pu8Data))[0] = (u8)((u32Data)>>24);((u8*)(pu8Data))[1] = (u8)((u32Data)>>16);((u8*)pu8Data)[2] = (u8)((u32Data)>>8);((u8*)pu8Data)[3] = (u8)((u32Data))

#endif

#ifndef BYTE2U32

#	define BYTE2U32(pu8Data)						(((u32)((pu8Data)[0])<<24) + ((u32)((pu8Data)[1])<<16) + ((u16)((pu8Data)[2])<<8) + (u8)((pu8Data)[3]))

#endif

#ifndef U322BYTE

#	define U322BYTE(pu8Data,u32Data)				((u8*)(pu8Data))[0] = (u8)((u32Data)>>24);((u8*)(pu8Data))[1] = (u8)((u32Data)>>16);((u8*)pu8Data)[2] = (u8)((u32Data)>>8);((u8*)pu8Data)[3] = (u8)((u32Data))

#endif



#ifndef BYTE2LONG_LE

#	define BYTE2LONG_LE(pu8Data)					(((u32)((pu8Data)[3])<<24) + ((u32)((pu8Data)[2])<<16) + ((u16)((pu8Data)[1])<<8) + (u8)((pu8Data)[0]))

#endif

#ifndef BYTE2U4_LE

#	define BYTE2U4_LE(pu8Data)						(((u32)((pu8Data)[3])<<24) + ((u32)((pu8Data)[2])<<16) + ((u16)((pu8Data)[1])<<8) + (u8)((pu8Data)[0]))

#endif

#ifndef BYTE2U32_LE

#	define BYTE2U32_LE(pu8Data)						(((u32)((pu8Data)[3])<<24) + ((u32)((pu8Data)[2])<<16) + ((u16)((pu8Data)[1])<<8) + (u8)((pu8Data)[0]))

#endif

#ifndef BYTE2WORD_LE

#	define BYTE2WORD_LE(pu8Data)					((u16)(((u16)((pu8Data)[1])<<8) + (u8)((pu8Data)[0])))

#endif

#ifndef LONG2BYTE_LE

#	define LONG2BYTE_LE(pu8Data,u32Data)			((u8*)(pu8Data))[3] = (u8)((u32Data)>>24);((u8*)(pu8Data))[2] = (u8)((u32Data)>>16);((u8*)pu8Data)[1] = (u8)((u32Data)>>8);((u8*)pu8Data)[0] = (u8)((u32Data))

#endif

#ifndef U42BYTE_LE

#	define U42BYTE_LE(pu8Data,u32Data)				((u8*)(pu8Data))[3] = (u8)((u32Data)>>24);((u8*)(pu8Data))[2] = (u8)((u32Data)>>16);((u8*)pu8Data)[1] = (u8)((u32Data)>>8);((u8*)pu8Data)[0] = (u8)((u32Data))

#endif

#ifndef U322BYTE_LE

#	define U322BYTE_LE(pu8Data,u32Data)				((u8*)(pu8Data))[3] = (u8)((u32Data)>>24);((u8*)(pu8Data))[2] = (u8)((u32Data)>>16);((u8*)pu8Data)[1] = (u8)((u32Data)>>8);((u8*)pu8Data)[0] = (u8)((u32Data))

#endif

#ifndef WORD2BYTE_LE

#	define WORD2BYTE_LE(pu8Data,u16Data)			((u8*)(pu8Data))[1] = (u8)((u16Data)>>8);((u8*)(pu8Data))[0] = (u8)((u16Data))

#endif

#ifndef BYTE_REV

#	define BYTE_REV(u8Byte)							((u8Byte)=((u8Byte)&0xaa)>>1|((u8Byte)&0x55)<<1, (u8Byte)=((u8Byte)&0xcc)>>2|((u8Byte)&0x33)<<2, (((u8Byte)&0xf0)>>4|((u8Byte)&0x0f)<<4))

#endif



// dynamic lib load/free/get

#ifndef _DYLIB_OP_

#	define _DYLIB_OP_

#	if defined(_OS_WINDOWS_) || defined(WIN32)

#		define DYLIB_LOAD(dylibFilePath) 			(void*)LoadLibraryA((const char*)(dylibFilePath))

#		define DYLIB_FREE(handle) 					FreeLibrary((HMODULE)(handle))

#		define DYLIB_SYMBOL(handle, name) 			GetProcAddress((HMODULE)(handle), (const char*)(name))

#	else

#		define DYLIB_LOAD(dylibFilePath) 			dlopen((const char*)(dylibFilePath), RTLD_NOW|RTLD_LOCAL)

#		define DYLIB_FREE(handle) 					dlclose((handle))

#		define DYLIB_SYMBOL(handle, name) 			dlsym((handle), (const char*)(name))

#	endif

#endif

// 宏定义: 名称转字符串

#define NANE_2_STR(r)			#r

// 宏定义: 值转字符串

#define VALUE_2_STR(r)			NANE_2_STR(r)

//////////////////////////////////////////////////////////////////////////

// 以上的定义是通用的，无厂商特性，与厂商无关

//////////////////////////////////////////////////////////////////////////



#ifndef _ETHANDLE_

#	define _ETHANDLE_

	typedef void*					ETHANDLE;

#endif



#ifndef ETTRUE

#	define	ETTRUE					1

#endif

#ifndef ETFALSE

#	define	ETFALSE					0

#endif



#ifndef _ETBOOL_

#	define _ETBOOL_

	typedef int						ETBOOL;

#endif



#ifndef ETCALL

#	define	ETCALL __stdcall

#endif



#ifndef ETBEGIN_NAMESPACE

#	define ETBEGIN_NAMESPACE(nameSpace)	namespace nameSpace{

#	define ETEND_NAMESPACE()			}

#	define ETUSING_NAMESPACE(nameSpace)	using namespace nameSpace

#endif



#ifndef ETNUM2STR

#	define	_ETNUM2STR(n)	#n

#	define	ETNUM2STR(n)	_ETNUM2STR(n)

#endif


#define PERSONALISATION_STRING_TOO_LONG 0x0001
#define IN_ERROR_STATE                  0x0002
#define ALREADY_INSTANTIATED            0x0003      
#define NOT_INSTANTIATED                0x0004  
#define ERROR_RETRIEVING_NONCE          0x0005        
#define ERROR_RETRIEVING_ENTROPY        0x0006          
#define ERROR_INSTANTIATING_DRBG        0x0007          
#define ENTROPY_OUT_OF_RANGE            0x0008      
#define ENTROPY_INPUT_TOO_LONG          0x0009
#define ADDITIONAL_INPUT_TOO_LONG       0x000A           
#define UNABLE_TO_RESEED                0x000B  
#define REQUEST_TOO_LARGE_FOR_DRBG      0x000C            
#define RESEED_ERROR                    0x000D
#define GENERATE_ERROR                  0x000E
#define POOL_MAX_LENGTH_EXCEEDED        0x000F
#define INVALID_ENTROPY_FACTOR          0x0010        
#define INSUFFICIENT_ENTROPY            0x0011      
#define BUFFER_TOO_SMALL                0x0012  
#define INVALID_BUFFER                  0x0013
#define INVALID_LENGTH                  0x0014
#define CRNG_TEST_FAILED                0x0015

#define ETRNGNAMESPACE EtRng

#endif 

