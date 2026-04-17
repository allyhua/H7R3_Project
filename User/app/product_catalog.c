#include "app/product_catalog.h"

#include <string.h>

/*
 * Pricing assumptions used for the current prototype.
 * These values are centralized here so they can be tuned later without
 * touching the cart logic or workflow state machine.
 */
static const product_info_t g_product_catalog[PRODUCT_ID_COUNT] = {
    {PRODUCT_ID_APPLE,          "Apple",  "Apple", PRODUCT_PRICING_BY_WEIGHT,            1280U, "yuan/kg",   "kg",  0U,   80U,  5000U,  0U},
    {PRODUCT_ID_BANANA,         "Banana", "Banana", PRODUCT_PRICING_BY_WEIGHT,            880U, "yuan/kg",   "kg",  0U,   80U,  5000U,  0U},
    {PRODUCT_ID_ORANGE,         "Orange", "Orange", PRODUCT_PRICING_BY_WEIGHT,            980U, "yuan/kg",   "kg",  0U,   80U,  5000U,  0U},
    {PRODUCT_ID_NONGFU_SPRING,  "NFSQ",   "NFSQ",  PRODUCT_PRICING_BY_COUNT_FROM_WEIGHT, 200U, "yuan/bottle", "pcs", 596U, 350U, 4000U, 60U},
    {PRODUCT_ID_YIBAO,          "Yibao",  "Yibao", PRODUCT_PRICING_BY_COUNT_FROM_WEIGHT, 200U, "yuan/bottle", "pcs", 570U, 330U, 4000U, 60U}
};

const product_info_t *product_catalog_get(product_id_t product_id)
{
    if (product_id < PRODUCT_ID_COUNT)
    {
        return &g_product_catalog[product_id];
    }

    return 0;
}

const product_info_t *product_catalog_find_by_ai_label(const char *ai_label_name)
{
    uint32_t idx;

    if (ai_label_name == 0)
    {
        return 0;
    }

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        if (strcmp(g_product_catalog[idx].ai_label_name, ai_label_name) == 0)
        {
            return &g_product_catalog[idx];
        }
    }

    return 0;
}
