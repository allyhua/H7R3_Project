#ifndef __PRODUCT_CATALOG_H
#define __PRODUCT_CATALOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    PRODUCT_PRICING_BY_WEIGHT = 0,
    PRODUCT_PRICING_BY_COUNT_FROM_WEIGHT
} product_pricing_mode_t;

typedef enum
{
    PRODUCT_ID_APPLE = 0,
    PRODUCT_ID_BANANA,
    PRODUCT_ID_ORANGE,
    PRODUCT_ID_NONGFU_SPRING,
    PRODUCT_ID_YIBAO,
    PRODUCT_ID_COUNT,
    PRODUCT_ID_UNKNOWN = 0xFF
} product_id_t;

typedef struct
{
    product_id_t product_id;
    const char *product_name;
    const char *ai_label_name;
    product_pricing_mode_t pricing_mode;
    uint32_t unit_price_cent;
    const char *price_unit_text;
    const char *display_unit_text;
    uint32_t nominal_unit_weight_g;
    uint32_t min_valid_weight_g;
    uint32_t max_valid_weight_g;
    uint32_t rounding_tolerance_g;
} product_info_t;

const product_info_t *product_catalog_get(product_id_t product_id);
const product_info_t *product_catalog_find_by_ai_label(const char *ai_label_name);

#ifdef __cplusplus
}
#endif

#endif
