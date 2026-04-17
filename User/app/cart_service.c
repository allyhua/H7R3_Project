#include "app/cart_service.h"

#include <stdio.h>
#include <string.h>

static uint32_t cart_round_div_u32(uint32_t numerator, uint32_t denominator)
{
    return (numerator + (denominator / 2U)) / denominator;
}

static uint32_t cart_calculate_weight_subtotal_cent(uint32_t weight_g, uint32_t unit_price_cent_per_kg)
{
    return cart_round_div_u32(weight_g * unit_price_cent_per_kg, 1000U);
}

static int32_t cart_find_index(const cart_context_t *cart, product_id_t product_id)
{
    uint32_t idx;

    if (cart == 0)
    {
        return -1;
    }

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        if ((cart->items[idx].line_valid != 0U) && (cart->items[idx].product_id == product_id))
        {
            return (int32_t)idx;
        }
    }

    return -1;
}

static int32_t cart_find_empty_slot(const cart_context_t *cart)
{
    uint32_t idx;

    if (cart == 0)
    {
        return -1;
    }

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        if (cart->items[idx].line_valid == 0U)
        {
            return (int32_t)idx;
        }
    }

    return -1;
}

static void cart_recalculate_totals(cart_context_t *cart)
{
    uint32_t idx;

    cart->item_count = 0U;
    cart->total_price_cent = 0U;
    cart->total_weight_g = 0U;
    cart->total_count_all = 0U;

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        if (cart->items[idx].line_valid == 0U)
        {
            continue;
        }

        cart->item_count++;
        cart->total_price_cent += cart->items[idx].subtotal_cent;
        cart->total_weight_g += cart->items[idx].total_weight_g;
        cart->total_count_all += cart->items[idx].total_count;
    }
}

static cart_add_status_t cart_estimate_count_from_weight(const product_info_t *product,
                                                         uint32_t measured_weight_g,
                                                         uint32_t *count_out)
{
    uint32_t estimated_count;
    uint32_t expected_weight_g;
    uint32_t delta_g;

    if ((product == 0) || (count_out == 0) || (product->nominal_unit_weight_g == 0U))
    {
        return CART_ADD_ERR_INVALID_ARG;
    }

    estimated_count = cart_round_div_u32(measured_weight_g, product->nominal_unit_weight_g);
    if (estimated_count == 0U)
    {
        return CART_ADD_ERR_COUNT_ESTIMATE;
    }

    expected_weight_g = estimated_count * product->nominal_unit_weight_g;
    if (measured_weight_g >= expected_weight_g)
    {
        delta_g = measured_weight_g - expected_weight_g;
    }
    else
    {
        delta_g = expected_weight_g - measured_weight_g;
    }

    if (delta_g > product->rounding_tolerance_g)
    {
        return CART_ADD_ERR_COUNT_ESTIMATE;
    }

    *count_out = estimated_count;
    return CART_ADD_OK;
}

void cart_service_init(cart_context_t *cart)
{
    if (cart == 0)
    {
        return;
    }

    memset(cart, 0, sizeof(*cart));
    cart->selected_item_index = -1;
}

cart_add_status_t cart_service_add_item(cart_context_t *cart,
                                        const product_info_t *product,
                                        uint32_t measured_weight_g,
                                        cart_add_result_t *out_result)
{
    cart_item_t *item;
    int32_t slot_index;
    uint32_t counted_units = 0U;
    cart_add_status_t status;
    uint32_t addition_subtotal_cent = 0U;

    if ((cart == 0) || (product == 0))
    {
        return CART_ADD_ERR_INVALID_ARG;
    }

    if ((measured_weight_g < product->min_valid_weight_g) || (measured_weight_g > product->max_valid_weight_g))
    {
        return CART_ADD_ERR_INVALID_WEIGHT;
    }

    if (product->pricing_mode == PRODUCT_PRICING_BY_COUNT_FROM_WEIGHT)
    {
        status = cart_estimate_count_from_weight(product, measured_weight_g, &counted_units);
        if (status != CART_ADD_OK)
        {
            return status;
        }
    }

    slot_index = cart_find_index(cart, product->product_id);
    if (slot_index < 0)
    {
        slot_index = cart_find_empty_slot(cart);
        if (slot_index < 0)
        {
            return CART_ADD_ERR_INVALID_ARG;
        }

        item = &cart->items[slot_index];
        memset(item, 0, sizeof(*item));
        item->product_id = product->product_id;
        snprintf(item->product_name, sizeof(item->product_name), "%s", product->product_name);
        item->pricing_mode = product->pricing_mode;
        item->unit_price_cent = product->unit_price_cent;
        item->line_valid = 1U;
    }
    else
    {
        item = &cart->items[slot_index];
    }

    if (product->pricing_mode == PRODUCT_PRICING_BY_WEIGHT)
    {
        addition_subtotal_cent = cart_calculate_weight_subtotal_cent(measured_weight_g, product->unit_price_cent);
        item->total_weight_g += measured_weight_g;
        item->subtotal_cent = cart_calculate_weight_subtotal_cent(item->total_weight_g, item->unit_price_cent);
    }
    else
    {
        addition_subtotal_cent = counted_units * product->unit_price_cent;
        item->total_weight_g += measured_weight_g;
        item->total_count += counted_units;
        item->subtotal_cent = item->total_count * item->unit_price_cent;
    }

    cart_recalculate_totals(cart);

    if (out_result != 0)
    {
        memset(out_result, 0, sizeof(*out_result));
        out_result->product_id = product->product_id;
        snprintf(out_result->product_name, sizeof(out_result->product_name), "%s", product->product_name);
        out_result->pricing_mode = product->pricing_mode;
        out_result->measured_weight_g = measured_weight_g;
        out_result->counted_units = counted_units;
        out_result->subtotal_cent = addition_subtotal_cent;
        out_result->valid = 1U;
    }

    return CART_ADD_OK;
}

int cart_service_remove_item(cart_context_t *cart, product_id_t product_id)
{
    int32_t slot_index;

    if (cart == 0)
    {
        return -1;
    }

    slot_index = cart_find_index(cart, product_id);
    if (slot_index < 0)
    {
        return -1;
    }

    memset(&cart->items[slot_index], 0, sizeof(cart->items[slot_index]));
    cart_recalculate_totals(cart);
    return 0;
}

void cart_service_clear(cart_context_t *cart)
{
    if (cart == 0)
    {
        return;
    }

    memset(cart, 0, sizeof(*cart));
    cart->selected_item_index = -1;
}

const cart_item_t *cart_service_get_items(const cart_context_t *cart, uint32_t *out_count)
{
    if (cart == 0)
    {
        return 0;
    }

    if (out_count != 0)
    {
        *out_count = cart->item_count;
    }

    return cart->items;
}
