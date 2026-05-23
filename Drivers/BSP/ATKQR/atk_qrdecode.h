#ifndef __ATK_QRDECODE_H
#define __ATK_QRDECODE_H

#include "./SYSTEM/sys/sys.h"
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

/*
 * The bundled ATK_QR library accepts 8-bit grayscale input only.
 * UTF8->OEM conversion is disabled here to avoid pulling FatFs into this demo.
 */
#define ATK_QR_UTF82OEM_SUPPORT     0
#define ATK_QR_GBK_SUPPORT          0

#define ATK_QR_OK                   0
#define ATK_QR_MEM_ERR              1
#define ATK_QR_OTHER_ERR            2
#define ATK_QR_RECO_ERR             3

u8 atk_qr_init(void);
u8 atk_qr_decode(u16 bmp_width, u16 bmp_heigh, u8 *bmp, u8 btype, u8 *result);
void atk_qr_destroy(void);

void atk_qr_heap_reset(void);
void atk_qr_memset(void *p, u8 c, u32 len);
void *atk_qr_malloc(u32 size);
void *atk_qr_realloc(void *ptr, u32 size);
void atk_qr_free(void *ptr);
void atk_qr_memcpy(void *des, void *src, u32 n);
u16 atk_qr_convert(u16 unicode);

#endif
