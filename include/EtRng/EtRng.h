#ifndef __ETRNG_H__
#define __ETRNG_H__
#include "EtTypeDef.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define ETRNG_VERSION_A 	1
#define ETRNG_VERSION_B 	0
#define ETRNG_VERSION_C 	0
#define ETRNG_VERSION_D 	0

#ifndef NUM2STR
#	define	_NUM2STR(n)	#n
#	define	NUM2STR(n)	_NUM2STR(n)
#endif
// 字符串形式的版本号
#define ETRNG_VERSION		NUM2STR(ETRNG_VERSION_A) "." NUM2STR(ETRNG_VERSION_B) "." NUM2STR(ETRNG_VERSION_C) "." NUM2STR(ETRNG_VERSION_D)

// return 0 mean success, other value mean error
typedef u32 (*EtRngAcquireEntropy)(IN const void *pContext, OUT u8* pBuf, IN u32 u32EntropyNeededLen);
typedef u32 (*EtRngReleaseContext)(void** ppContext);

typedef struct BlkAcquireEntropyEntryTag {
    EtRngAcquireEntropy m_callback;
	EtRngReleaseContext m_releaseContext;
    void *m_context; // http 的 url 之类的额外需要的参数
} BlkAcquireEntropyEntry;

const char* ETCALL EtRngVersion(void);
const char* ETCALL EtRngOutVersion(void);

u32 ETCALL EtRngLibInit(IN const BlkAcquireEntropyEntry *pAcquireEntropyEntries, IN int nNumEntries);
u32 ETCALL EtRngLibRelease(void);

/*
*@pu8Pers 个性化字符串
*@sPerslen  personalization string length
*/
u32 ETCALL EtRngGetInstance(IN const u8 *pu8Pers, IN int nPerslen, OUT ETHANDLE* phHandle);
u32 ETCALL EtRngReleaseInstance(INOUT ETHANDLE* phHandle);

/*
*@pu8Entropy 外部输入的熵字节串， 可以为 NULL
*@sEntEntropyLen 外部输入的熵字节串长度， 可以为 0
*@pu8Additional 可选的额外输入的字节串
*@sAdditionalLen 可选的额外输入的字节串长度
*/
u32 ETCALL EtRngReseed(IN ETHANDLE hHandle, IN const u8 *pu8Entropy, IN int nEntEntropyLen, IN const u8 *pu8Additional, IN int nAdditionalLen);
/*
*@pu8Additional 可选的额外输入的字节串
*@sAdditionalLen 可选的额外输入的字节串长度
*@nRandomlen  nRandomlen 为期望的随机字节串长度
*@pu8Random 输出的随机字节串
*/
u32 ETCALL EtRngGenerate(IN ETHANDLE hHandle, IN const u8 *pu8Additional, IN int nAdditionalLen, IN int nRandomlen, OUT u8* pu8Random);

/*
*@nEntropyMin  最小的熵字节串长度
*@nEntropyNeed  需要的熵字节串长度
*@ppu8Entropy  输出的熵字节串
*@pu32EntropyLen 输入时为数组的字节串长度（代表最大长度）, 输出的是实际生成的熵字节串长度
*/
u32 ETCALL EtRngGetEntropy(IN int nEntropyMin,IN int nEntropyNeed, OUT u8* pu8Entropy, INOUT int* pnEntropyLen);

// u32 ETCALL EtRngVerifyZeroization(ETHANDLE handle);
#if defined(__cplusplus)
}
#endif

#endif
