#ifndef __cmtypes_H
#define __cmtypes_H

#include "stdint.h"

typedef enum {
	DEV_Result_Ok,   /*!< Everything OK */
	DEV_Result_Error /*!< Error occurred */
} DEV_Result_t;

typedef struct {
	int16_t X; /*!< X axis rotation */
	int16_t Y; /*!< Y axis rotation */
	int16_t Z; /*!< Z axis rotation */
} XYZ_t;

#define bool _Bool
#define FALSE 0
#define TRUE !FALSE

//Taras Ivaniv

#endif /*__ cmtypes_H */

