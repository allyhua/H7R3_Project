#include "app/product_catalog.h"

#include <string.h>

static const product_info_t g_product_catalog[PRODUCT_ID_COUNT] = {
    {PRODUCT_ID_APPLE,         "苹果",         "Apple",  PRODUCT_PRICING_BY_WEIGHT,            1280U, "元/千克", "千克", 0U,   80U, 5000U, 0U},
    {PRODUCT_ID_BANANA,        "香蕉",         "Banana", PRODUCT_PRICING_BY_WEIGHT,             880U, "元/千克", "千克", 0U,   80U, 5000U, 0U},
    {PRODUCT_ID_ORANGE,        "橙子",         "Orange", PRODUCT_PRICING_BY_WEIGHT,             980U, "元/千克", "千克", 0U,   80U, 5000U, 0U},
    {PRODUCT_ID_NONGFU_SPRING, "农夫山泉",     "NFSQ",   PRODUCT_PRICING_BY_COUNT_FROM_WEIGHT,  200U, "元/瓶", "瓶", 596U, 350U, 4000U, 60U},
    {PRODUCT_ID_YIBAO,         "怡宝",         "Yibao",  PRODUCT_PRICING_BY_COUNT_FROM_WEIGHT,  200U, "元/瓶", "瓶", 570U, 330U, 4000U, 60U}
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
