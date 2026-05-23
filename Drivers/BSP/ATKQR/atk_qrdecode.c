#include "./BSP/ATKQR/atk_qrdecode.h"
#include "./BSP/DCMIPP/dcmipp.h"
#include <string.h>

#define ATK_QR_GRAY_OFFSET_BYTES    (1024U * 1024U)
#define ATK_QR_GRAY_MAX_BYTES       (512U * 1024U)
#define ATK_QR_HEAP_ALIGN           8U
#define ATK_QR_BLOCK_MAGIC          0x41544B51UL

typedef struct
{
    u32 magic;
    u32 size;
    u32 free;
    u32 reserved;
} atk_qr_block_t;

static u8 *g_atk_qr_heap_base;
static u32 g_atk_qr_heap_size;
static u8 g_atk_qr_heap_ready;

static u32 atk_qr_align_up(u32 value)
{
    return (value + (ATK_QR_HEAP_ALIGN - 1U)) & ~(ATK_QR_HEAP_ALIGN - 1U);
}

static void atk_qr_heap_prepare(void)
{
    uintptr_t camera_start = (uintptr_t)camera_date_buf;
    uintptr_t heap_start = camera_start + ATK_QR_GRAY_OFFSET_BYTES + ATK_QR_GRAY_MAX_BYTES;
    uintptr_t camera_end = camera_start + sizeof(camera_date_buf);

    heap_start = (heap_start + (ATK_QR_HEAP_ALIGN - 1U)) & ~(uintptr_t)(ATK_QR_HEAP_ALIGN - 1U);
    if (heap_start < camera_end)
    {
        g_atk_qr_heap_base = (u8 *)heap_start;
        g_atk_qr_heap_size = (u32)(camera_end - heap_start);
    }
    else
    {
        g_atk_qr_heap_base = NULL;
        g_atk_qr_heap_size = 0U;
    }

    g_atk_qr_heap_ready = 1U;
}

void atk_qr_heap_reset(void)
{
    if (g_atk_qr_heap_ready == 0U)
    {
        atk_qr_heap_prepare();
    }

    if ((g_atk_qr_heap_base != NULL) && (g_atk_qr_heap_size > sizeof(atk_qr_block_t)))
    {
        atk_qr_block_t *block = (atk_qr_block_t *)g_atk_qr_heap_base;

        block->magic = ATK_QR_BLOCK_MAGIC;
        block->size = g_atk_qr_heap_size - sizeof(atk_qr_block_t);
        block->free = 1U;
        block->reserved = 0U;
    }
}

void atk_qr_memset(void *p, u8 c, u32 len)
{
    memset(p, c, len);
}

void *atk_qr_malloc(u32 size)
{
    u8 *cursor;
    u8 *end;
    u32 need;

    if (size == 0U)
    {
        return NULL;
    }

    if (g_atk_qr_heap_ready == 0U)
    {
        atk_qr_heap_reset();
    }

    need = atk_qr_align_up(size);
    cursor = g_atk_qr_heap_base;
    end = g_atk_qr_heap_base + g_atk_qr_heap_size;

    while ((cursor != NULL) && ((cursor + sizeof(atk_qr_block_t)) <= end))
    {
        atk_qr_block_t *block = (atk_qr_block_t *)cursor;

        if ((block->magic != ATK_QR_BLOCK_MAGIC) || (block->size == 0U))
        {
            break;
        }

        if ((block->free != 0U) && (block->size >= need))
        {
            u32 remain = block->size - need;

            if (remain > (sizeof(atk_qr_block_t) + ATK_QR_HEAP_ALIGN))
            {
                atk_qr_block_t *next = (atk_qr_block_t *)(cursor + sizeof(atk_qr_block_t) + need);

                next->magic = ATK_QR_BLOCK_MAGIC;
                next->size = remain - sizeof(atk_qr_block_t);
                next->free = 1U;
                next->reserved = 0U;
                block->size = need;
            }

            block->free = 0U;
            return cursor + sizeof(atk_qr_block_t);
        }

        cursor += sizeof(atk_qr_block_t) + block->size;
    }

    return NULL;
}

void *atk_qr_realloc(void *ptr, u32 size)
{
    void *new_ptr;
    atk_qr_block_t *old_block;
    u32 copy_size;

    if (ptr == NULL)
    {
        return atk_qr_malloc(size);
    }

    if (size == 0U)
    {
        atk_qr_free(ptr);
        return NULL;
    }

    old_block = (atk_qr_block_t *)((u8 *)ptr - sizeof(atk_qr_block_t));
    if (old_block->magic != ATK_QR_BLOCK_MAGIC)
    {
        return NULL;
    }

    if (old_block->size >= size)
    {
        return ptr;
    }

    new_ptr = atk_qr_malloc(size);
    if (new_ptr != NULL)
    {
        copy_size = old_block->size;
        if (copy_size > size)
        {
            copy_size = size;
        }
        memcpy(new_ptr, ptr, copy_size);
        atk_qr_free(ptr);
    }

    return new_ptr;
}

void atk_qr_free(void *ptr)
{
    atk_qr_block_t *block;
    u8 *cursor;
    u8 *end;

    if (ptr == NULL)
    {
        return;
    }

    block = (atk_qr_block_t *)((u8 *)ptr - sizeof(atk_qr_block_t));
    if (block->magic == ATK_QR_BLOCK_MAGIC)
    {
        block->free = 1U;

        cursor = g_atk_qr_heap_base;
        end = g_atk_qr_heap_base + g_atk_qr_heap_size;

        while ((cursor != NULL) && ((cursor + sizeof(atk_qr_block_t)) <= end))
        {
            atk_qr_block_t *current = (atk_qr_block_t *)cursor;
            u8 *next_addr;
            atk_qr_block_t *next;

            if ((current->magic != ATK_QR_BLOCK_MAGIC) || (current->size == 0U))
            {
                break;
            }

            next_addr = cursor + sizeof(atk_qr_block_t) + current->size;
            if ((next_addr + sizeof(atk_qr_block_t)) > end)
            {
                break;
            }

            next = (atk_qr_block_t *)next_addr;
            if (next->magic != ATK_QR_BLOCK_MAGIC)
            {
                break;
            }

            if ((current->free != 0U) && (next->free != 0U))
            {
                current->size += sizeof(atk_qr_block_t) + next->size;
                continue;
            }

            cursor = next_addr;
        }
    }
}

void atk_qr_memcpy(void *des, void *src, u32 n)
{
    memcpy(des, src, n);
}

u16 atk_qr_convert(u16 unicode)
{
    return unicode;
}
