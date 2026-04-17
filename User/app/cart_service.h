#ifndef __CART_SERVICE_H
#define __CART_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "app/product_catalog.h"

typedef struct
{
    product_id_t product_id;
    char product_name[16];
    product_pricing_mode_t pricing_mode;
    uint32_t unit_price_cent;
    uint32_t total_weight_g;
    uint32_t total_count;
    uint32_t subtotal_cent;
    uint8_t line_valid;
} cart_item_t;

typedef struct
{
    cart_item_t items[PRODUCT_ID_COUNT];
    uint32_t item_count;
    uint32_t total_price_cent;
    uint32_t total_weight_g;
    uint32_t total_count_all;
    int32_t selected_item_index;
} cart_context_t;

typedef struct
{
    product_id_t product_id;
    char product_name[16];
    product_pricing_mode_t pricing_mode;
    uint32_t measured_weight_g;
    uint32_t counted_units;
    uint32_t subtotal_cent;
    uint8_t valid;
} cart_add_result_t;

typedef enum
{
    CART_ADD_OK = 0,
    CART_ADD_ERR_INVALID_ARG = -1,
    CART_ADD_ERR_UNKNOWN_PRODUCT = -2,
    CART_ADD_ERR_INVALID_WEIGHT = -3,
    CART_ADD_ERR_COUNT_ESTIMATE = -4
} cart_add_status_t;

void cart_service_init(cart_context_t *cart);
cart_add_status_t cart_service_add_item(cart_context_t *cart,
                                        const product_info_t *product,
                                        uint32_t measured_weight_g,
                                        cart_add_result_t *out_result);
int cart_service_remove_item(cart_context_t *cart, product_id_t product_id);
void cart_service_clear(cart_context_t *cart);
const cart_item_t *cart_service_get_items(const cart_context_t *cart, uint32_t *out_count);

#ifdef __cplusplus
}
#endif

#endif
