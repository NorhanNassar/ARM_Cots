#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32 ;

typedef signed char s8  ;
typedef signed short int s16  ;
typedef signed long int s32 ;

typedef float f32 ; 
typedef double f64 ; 

typedef u8  * Pu8 ;
typedef u16 * Pu16 ;
typedef u32 * Pu32 ;

typedef s8  * Ps8 ;
typedef s16 * Ps16 ;
typedef s32 * Ps32 ;

typedef f32 * Pf32 ;
typedef f64 * Pf64 ;

typedef void (*CALLBACKFUN)(void);
typedef void (*taskRunnable_t)(void);

typedef enum
{
	OK,
	NOT_OK,
} STD_ERROR;

typedef enum
{
	ON,
	OFF,
} STD_STATE;

typedef u8 STD_WRITE;

#define SET 		0
#define RESET		1

#endif
