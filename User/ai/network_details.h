/**
  ******************************************************************************
  * @file    network.h
  * @date    2026-03-29T14:04:40+0000
  * @brief   ST.AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */
#ifndef STAI_NETWORK_DETAILS_H
#define STAI_NETWORK_DETAILS_H

#include "stai.h"
#include "layers.h"

const stai_network_details g_network_details = {
  .tensors = (const stai_tensor[86]) {
   { .size_bytes = 27648, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 96, 96}}, .scale = {1, (const float[1]){0.01865844801068306}}, .zeropoint = {1, (const int16_t[1]){-14}}, .name = "input_output" },
   { .size_bytes = 27649, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 96, 96, 3}}, .scale = {1, (const float[1]){0.01865844801068306}}, .zeropoint = {1, (const int16_t[1]){-14}}, .name = "input_Transpose_output" },
   { .size_bytes = 80000, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 32, 48, 48}}, .scale = {1, (const float[1]){0.014573031105101109}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_output" },
   { .size_bytes = 80000, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 32, 50, 50}}, .scale = {1, (const float[1]){0.014573031105101109}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_1_pad_before_output" },
   { .size_bytes = 73728, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 48, 48, 32}}, .scale = {1, (const float[1]){0.0235294122248888}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_1_output" },
   { .size_bytes = 36864, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 48, 48, 16}}, .scale = {1, (const float[1]){0.061115458607673645}}, .zeropoint = {1, (const int16_t[1]){-13}}, .name = "getitem_6_output" },
   { .size_bytes = 240000, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 96, 48, 48}}, .scale = {1, (const float[1]){0.0235294122248888}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_2_output" },
   { .size_bytes = 240000, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 96, 50, 50}}, .scale = {1, (const float[1]){0.0235294122248888}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_3_pad_before_output" },
   { .size_bytes = 55296, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 24, 24, 96}}, .scale = {1, (const float[1]){0.0235294122248888}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_3_output" },
   { .size_bytes = 13824, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 24, 24, 24}}, .scale = {1, (const float[1]){0.036474939435720444}}, .zeropoint = {1, (const int16_t[1]){-10}}, .name = "getitem_15_output" },
   { .size_bytes = 97344, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 144, 24, 24}}, .scale = {1, (const float[1]){0.007497061975300312}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_4_output" },
   { .size_bytes = 97344, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 144, 26, 26}}, .scale = {1, (const float[1]){0.007497061975300312}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_5_pad_before_output" },
   { .size_bytes = 82944, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 24, 24, 144}}, .scale = {1, (const float[1]){0.012918814085423946}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_5_output" },
   { .size_bytes = 13824, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 24, 24, 24}}, .scale = {1, (const float[1]){0.03861986845731735}}, .zeropoint = {1, (const int16_t[1]){-18}}, .name = "getitem_24_output" },
   { .size_bytes = 13824, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 24, 24, 24}}, .scale = {1, (const float[1]){0.04933002591133118}}, .zeropoint = {1, (const int16_t[1]){-11}}, .name = "add_output" },
   { .size_bytes = 97344, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 144, 24, 24}}, .scale = {1, (const float[1]){0.010229479521512985}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_6_output" },
   { .size_bytes = 97344, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 144, 26, 26}}, .scale = {1, (const float[1]){0.010229479521512985}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_7_pad_before_output" },
   { .size_bytes = 20736, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 144}}, .scale = {1, (const float[1]){0.011080938391387463}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_7_output" },
   { .size_bytes = 4608, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 32}}, .scale = {1, (const float[1]){0.024918919429183006}}, .zeropoint = {1, (const int16_t[1]){12}}, .name = "getitem_33_output" },
   { .size_bytes = 37632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 192, 12, 12}}, .scale = {1, (const float[1]){0.0055095539428293705}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_8_output" },
   { .size_bytes = 37632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 192, 14, 14}}, .scale = {1, (const float[1]){0.0055095539428293705}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_9_pad_before_output" },
   { .size_bytes = 27648, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 192}}, .scale = {1, (const float[1]){0.00837071891874075}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_9_output" },
   { .size_bytes = 4608, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 32}}, .scale = {1, (const float[1]){0.023046167567372322}}, .zeropoint = {1, (const int16_t[1]){-12}}, .name = "getitem_42_output" },
   { .size_bytes = 4608, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 32}}, .scale = {1, (const float[1]){0.038145385682582855}}, .zeropoint = {1, (const int16_t[1]){-7}}, .name = "add_1_output" },
   { .size_bytes = 37632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 192, 12, 12}}, .scale = {1, (const float[1]){0.005275337491184473}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_10_output" },
   { .size_bytes = 37632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 192, 14, 14}}, .scale = {1, (const float[1]){0.005275337491184473}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_11_pad_before_output" },
   { .size_bytes = 27648, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 192}}, .scale = {1, (const float[1]){0.010762277990579605}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_11_output" },
   { .size_bytes = 4608, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 32}}, .scale = {1, (const float[1]){0.019440552219748497}}, .zeropoint = {1, (const int16_t[1]){3}}, .name = "getitem_51_output" },
   { .size_bytes = 4608, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 12, 12, 32}}, .scale = {1, (const float[1]){0.041437845677137375}}, .zeropoint = {1, (const int16_t[1]){5}}, .name = "add_2_output" },
   { .size_bytes = 37632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 192, 12, 12}}, .scale = {1, (const float[1]){0.00702090235427022}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_12_output" },
   { .size_bytes = 37632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 192, 14, 14}}, .scale = {1, (const float[1]){0.00702090235427022}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_13_pad_before_output" },
   { .size_bytes = 6912, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 192}}, .scale = {1, (const float[1]){0.010315286926925182}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_13_output" },
   { .size_bytes = 2304, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 64}}, .scale = {1, (const float[1]){0.019600583240389824}}, .zeropoint = {1, (const int16_t[1]){-8}}, .name = "getitem_60_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 6, 6}}, .scale = {1, (const float[1]){0.00422650296241045}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_14_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 8, 8}}, .scale = {1, (const float[1]){0.00422650296241045}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_15_pad_before_output" },
   { .size_bytes = 13824, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 384}}, .scale = {1, (const float[1]){0.006199080031365156}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_15_output" },
   { .size_bytes = 2304, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 64}}, .scale = {1, (const float[1]){0.014918401837348938}}, .zeropoint = {1, (const int16_t[1]){5}}, .name = "getitem_69_output" },
   { .size_bytes = 2304, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 64}}, .scale = {1, (const float[1]){0.024069471284747124}}, .zeropoint = {1, (const int16_t[1]){-11}}, .name = "add_3_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 6, 6}}, .scale = {1, (const float[1]){0.0032411073334515095}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_16_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 8, 8}}, .scale = {1, (const float[1]){0.0032411073334515095}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_17_pad_before_output" },
   { .size_bytes = 13824, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 384}}, .scale = {1, (const float[1]){0.005039352923631668}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_17_output" },
   { .size_bytes = 2304, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 64}}, .scale = {1, (const float[1]){0.0114357965067029}}, .zeropoint = {1, (const int16_t[1]){8}}, .name = "getitem_78_output" },
   { .size_bytes = 2304, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 64}}, .scale = {1, (const float[1]){0.027987157925963402}}, .zeropoint = {1, (const int16_t[1]){-3}}, .name = "add_4_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 6, 6}}, .scale = {1, (const float[1]){0.0034559844061732292}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_18_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 8, 8}}, .scale = {1, (const float[1]){0.0034559844061732292}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_19_pad_before_output" },
   { .size_bytes = 13824, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 384}}, .scale = {1, (const float[1]){0.011522933840751648}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_19_output" },
   { .size_bytes = 2304, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 64}}, .scale = {1, (const float[1]){0.01968309096992016}}, .zeropoint = {1, (const int16_t[1]){28}}, .name = "getitem_87_output" },
   { .size_bytes = 2304, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 64}}, .scale = {1, (const float[1]){0.028103968128561974}}, .zeropoint = {1, (const int16_t[1]){-2}}, .name = "add_5_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 6, 6}}, .scale = {1, (const float[1]){0.004286992829293013}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_20_output" },
   { .size_bytes = 24576, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 384, 8, 8}}, .scale = {1, (const float[1]){0.004286992829293013}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_21_pad_before_output" },
   { .size_bytes = 13824, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 384}}, .scale = {1, (const float[1]){0.009298519231379032}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_21_output" },
   { .size_bytes = 3456, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 96}}, .scale = {1, (const float[1]){0.021040504798293114}}, .zeropoint = {1, (const int16_t[1]){2}}, .name = "getitem_96_output" },
   { .size_bytes = 36864, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 576, 6, 6}}, .scale = {1, (const float[1]){0.004924986511468887}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_22_output" },
   { .size_bytes = 36864, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 576, 8, 8}}, .scale = {1, (const float[1]){0.004924986511468887}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_23_pad_before_output" },
   { .size_bytes = 20736, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 576}}, .scale = {1, (const float[1]){0.007832746021449566}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_23_output" },
   { .size_bytes = 3456, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 96}}, .scale = {1, (const float[1]){0.011386662721633911}}, .zeropoint = {1, (const int16_t[1]){-9}}, .name = "getitem_105_output" },
   { .size_bytes = 3456, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 96}}, .scale = {1, (const float[1]){0.02219851315021515}}, .zeropoint = {1, (const int16_t[1]){-3}}, .name = "add_6_output" },
   { .size_bytes = 36864, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 576, 6, 6}}, .scale = {1, (const float[1]){0.006472608540207148}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_24_output" },
   { .size_bytes = 36864, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 576, 8, 8}}, .scale = {1, (const float[1]){0.006472608540207148}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_25_pad_before_output" },
   { .size_bytes = 20736, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 576}}, .scale = {1, (const float[1]){0.007372942753136158}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_25_output" },
   { .size_bytes = 3456, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 96}}, .scale = {1, (const float[1]){0.01759873889386654}}, .zeropoint = {1, (const int16_t[1]){7}}, .name = "getitem_114_output" },
   { .size_bytes = 3456, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 6, 6, 96}}, .scale = {1, (const float[1]){0.028100553900003433}}, .zeropoint = {1, (const int16_t[1]){1}}, .name = "add_7_output" },
   { .size_bytes = 36864, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 576, 6, 6}}, .scale = {1, (const float[1]){0.00515981437638402}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_26_output" },
   { .size_bytes = 36864, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_FIRST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 576, 8, 8}}, .scale = {1, (const float[1]){0.00515981437638402}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_27_pad_before_output" },
   { .size_bytes = 5184, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 576}}, .scale = {1, (const float[1]){0.008898069150745869}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_27_output" },
   { .size_bytes = 1440, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 160}}, .scale = {1, (const float[1]){0.011738085187971592}}, .zeropoint = {1, (const int16_t[1]){13}}, .name = "getitem_123_output" },
   { .size_bytes = 8640, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 960}}, .scale = {1, (const float[1]){0.004450960550457239}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_28_output" },
   { .size_bytes = 24000, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 5, 5, 960}}, .scale = {1, (const float[1]){0.004450960550457239}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_29_pad_before_output" },
   { .size_bytes = 8640, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 960}}, .scale = {1, (const float[1]){0.009433678351342678}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_29_output" },
   { .size_bytes = 1440, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 160}}, .scale = {1, (const float[1]){0.010824562981724739}}, .zeropoint = {1, (const int16_t[1]){-16}}, .name = "getitem_132_output" },
   { .size_bytes = 1440, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 160}}, .scale = {1, (const float[1]){0.013574520125985146}}, .zeropoint = {1, (const int16_t[1]){6}}, .name = "add_8_output" },
   { .size_bytes = 8640, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 960}}, .scale = {1, (const float[1]){0.005065144971013069}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_30_output" },
   { .size_bytes = 24000, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 5, 5, 960}}, .scale = {1, (const float[1]){0.005065144971013069}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_31_pad_before_output" },
   { .size_bytes = 8640, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 960}}, .scale = {1, (const float[1]){0.008346958085894585}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_31_output" },
   { .size_bytes = 1440, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 160}}, .scale = {1, (const float[1]){0.01941019669175148}}, .zeropoint = {1, (const int16_t[1]){-12}}, .name = "getitem_141_output" },
   { .size_bytes = 1440, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 160}}, .scale = {1, (const float[1]){0.027217984199523926}}, .zeropoint = {1, (const int16_t[1]){-18}}, .name = "add_9_output" },
   { .size_bytes = 8640, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 960}}, .scale = {1, (const float[1]){0.0029977066442370415}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_32_output" },
   { .size_bytes = 24000, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 5, 5, 960}}, .scale = {1, (const float[1]){0.0029977066442370415}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_33_pad_before_output" },
   { .size_bytes = 8640, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 960}}, .scale = {1, (const float[1]){0.005531640723347664}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_33_output" },
   { .size_bytes = 2880, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 320}}, .scale = {1, (const float[1]){0.009980068542063236}}, .zeropoint = {1, (const int16_t[1]){-5}}, .name = "getitem_150_output" },
   { .size_bytes = 11520, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 3, 3, 1280}}, .scale = {1, (const float[1]){0.0235294122248888}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "hardtanh_34_output" },
   { .size_bytes = 46080, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 3, 3, 1280}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "hardtanh_34_0_0_mean_conversion_output" },
   { .size_bytes = 5120, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 1, 1, 1280}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "mean_output" },
   { .size_bytes = 5120, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 1, 1, 1280}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "mean_Mul_output" },
   { .size_bytes = 1280, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 1, 1, 1280}}, .scale = {1, (const float[1]){0.023325162008404732}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "mean_Mul_0_0_view_conversion_output" },
   { .size_bytes = 5, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 5}}, .scale = {1, (const float[1]){0.038118213415145874}}, .zeropoint = {1, (const int16_t[1]){-45}}, .name = "output_QuantizeLinear_Input_output" }
  },
  .nodes = (const stai_node_details[85]){
    {.id = 2, .type = AI_LAYER_TRANSPOSE_TYPE, .input_tensors = {1, (const int32_t[1]){0}}, .output_tensors = {1, (const int32_t[1]){1}} }, /* input_Transpose */
    {.id = 109, .type = AI_LAYER_OPTIMIZED_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){1}}, .output_tensors = {1, (const int32_t[1]){2}} }, /* hardtanh */
    {.id = 112, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){2}}, .output_tensors = {1, (const int32_t[1]){3}} }, /* hardtanh_1_pad_before */
    {.id = 112, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){3}}, .output_tensors = {1, (const int32_t[1]){4}} }, /* hardtanh_1 */
    {.id = 115, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){4}}, .output_tensors = {1, (const int32_t[1]){5}} }, /* getitem_6 */
    {.id = 118, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){5}}, .output_tensors = {1, (const int32_t[1]){6}} }, /* hardtanh_2 */
    {.id = 121, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){6}}, .output_tensors = {1, (const int32_t[1]){7}} }, /* hardtanh_3_pad_before */
    {.id = 121, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){7}}, .output_tensors = {1, (const int32_t[1]){8}} }, /* hardtanh_3 */
    {.id = 124, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){8}}, .output_tensors = {1, (const int32_t[1]){9}} }, /* getitem_15 */
    {.id = 127, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){9}}, .output_tensors = {1, (const int32_t[1]){10}} }, /* hardtanh_4 */
    {.id = 130, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){10}}, .output_tensors = {1, (const int32_t[1]){11}} }, /* hardtanh_5_pad_before */
    {.id = 130, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){11}}, .output_tensors = {1, (const int32_t[1]){12}} }, /* hardtanh_5 */
    {.id = 133, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){12}}, .output_tensors = {1, (const int32_t[1]){13}} }, /* getitem_24 */
    {.id = 136, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){9, 13}}, .output_tensors = {1, (const int32_t[1]){14}} }, /* add */
    {.id = 139, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){14}}, .output_tensors = {1, (const int32_t[1]){15}} }, /* hardtanh_6 */
    {.id = 142, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){15}}, .output_tensors = {1, (const int32_t[1]){16}} }, /* hardtanh_7_pad_before */
    {.id = 142, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){16}}, .output_tensors = {1, (const int32_t[1]){17}} }, /* hardtanh_7 */
    {.id = 145, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){17}}, .output_tensors = {1, (const int32_t[1]){18}} }, /* getitem_33 */
    {.id = 148, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){18}}, .output_tensors = {1, (const int32_t[1]){19}} }, /* hardtanh_8 */
    {.id = 151, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){19}}, .output_tensors = {1, (const int32_t[1]){20}} }, /* hardtanh_9_pad_before */
    {.id = 151, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){20}}, .output_tensors = {1, (const int32_t[1]){21}} }, /* hardtanh_9 */
    {.id = 154, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){21}}, .output_tensors = {1, (const int32_t[1]){22}} }, /* getitem_42 */
    {.id = 157, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){18, 22}}, .output_tensors = {1, (const int32_t[1]){23}} }, /* add_1 */
    {.id = 160, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){23}}, .output_tensors = {1, (const int32_t[1]){24}} }, /* hardtanh_10 */
    {.id = 163, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){24}}, .output_tensors = {1, (const int32_t[1]){25}} }, /* hardtanh_11_pad_before */
    {.id = 163, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){25}}, .output_tensors = {1, (const int32_t[1]){26}} }, /* hardtanh_11 */
    {.id = 166, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){26}}, .output_tensors = {1, (const int32_t[1]){27}} }, /* getitem_51 */
    {.id = 169, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){23, 27}}, .output_tensors = {1, (const int32_t[1]){28}} }, /* add_2 */
    {.id = 172, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){28}}, .output_tensors = {1, (const int32_t[1]){29}} }, /* hardtanh_12 */
    {.id = 175, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){29}}, .output_tensors = {1, (const int32_t[1]){30}} }, /* hardtanh_13_pad_before */
    {.id = 175, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){30}}, .output_tensors = {1, (const int32_t[1]){31}} }, /* hardtanh_13 */
    {.id = 178, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){31}}, .output_tensors = {1, (const int32_t[1]){32}} }, /* getitem_60 */
    {.id = 181, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){32}}, .output_tensors = {1, (const int32_t[1]){33}} }, /* hardtanh_14 */
    {.id = 184, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){33}}, .output_tensors = {1, (const int32_t[1]){34}} }, /* hardtanh_15_pad_before */
    {.id = 184, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){34}}, .output_tensors = {1, (const int32_t[1]){35}} }, /* hardtanh_15 */
    {.id = 187, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){35}}, .output_tensors = {1, (const int32_t[1]){36}} }, /* getitem_69 */
    {.id = 190, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){32, 36}}, .output_tensors = {1, (const int32_t[1]){37}} }, /* add_3 */
    {.id = 193, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){37}}, .output_tensors = {1, (const int32_t[1]){38}} }, /* hardtanh_16 */
    {.id = 196, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){38}}, .output_tensors = {1, (const int32_t[1]){39}} }, /* hardtanh_17_pad_before */
    {.id = 196, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){39}}, .output_tensors = {1, (const int32_t[1]){40}} }, /* hardtanh_17 */
    {.id = 199, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){40}}, .output_tensors = {1, (const int32_t[1]){41}} }, /* getitem_78 */
    {.id = 202, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){37, 41}}, .output_tensors = {1, (const int32_t[1]){42}} }, /* add_4 */
    {.id = 205, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){42}}, .output_tensors = {1, (const int32_t[1]){43}} }, /* hardtanh_18 */
    {.id = 208, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){43}}, .output_tensors = {1, (const int32_t[1]){44}} }, /* hardtanh_19_pad_before */
    {.id = 208, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){44}}, .output_tensors = {1, (const int32_t[1]){45}} }, /* hardtanh_19 */
    {.id = 211, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){45}}, .output_tensors = {1, (const int32_t[1]){46}} }, /* getitem_87 */
    {.id = 214, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){42, 46}}, .output_tensors = {1, (const int32_t[1]){47}} }, /* add_5 */
    {.id = 217, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){47}}, .output_tensors = {1, (const int32_t[1]){48}} }, /* hardtanh_20 */
    {.id = 220, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){48}}, .output_tensors = {1, (const int32_t[1]){49}} }, /* hardtanh_21_pad_before */
    {.id = 220, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){49}}, .output_tensors = {1, (const int32_t[1]){50}} }, /* hardtanh_21 */
    {.id = 223, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){50}}, .output_tensors = {1, (const int32_t[1]){51}} }, /* getitem_96 */
    {.id = 226, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){51}}, .output_tensors = {1, (const int32_t[1]){52}} }, /* hardtanh_22 */
    {.id = 229, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){52}}, .output_tensors = {1, (const int32_t[1]){53}} }, /* hardtanh_23_pad_before */
    {.id = 229, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){53}}, .output_tensors = {1, (const int32_t[1]){54}} }, /* hardtanh_23 */
    {.id = 232, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){54}}, .output_tensors = {1, (const int32_t[1]){55}} }, /* getitem_105 */
    {.id = 235, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){51, 55}}, .output_tensors = {1, (const int32_t[1]){56}} }, /* add_6 */
    {.id = 238, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){56}}, .output_tensors = {1, (const int32_t[1]){57}} }, /* hardtanh_24 */
    {.id = 241, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){57}}, .output_tensors = {1, (const int32_t[1]){58}} }, /* hardtanh_25_pad_before */
    {.id = 241, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){58}}, .output_tensors = {1, (const int32_t[1]){59}} }, /* hardtanh_25 */
    {.id = 244, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){59}}, .output_tensors = {1, (const int32_t[1]){60}} }, /* getitem_114 */
    {.id = 247, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){56, 60}}, .output_tensors = {1, (const int32_t[1]){61}} }, /* add_7 */
    {.id = 250, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){61}}, .output_tensors = {1, (const int32_t[1]){62}} }, /* hardtanh_26 */
    {.id = 253, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){62}}, .output_tensors = {1, (const int32_t[1]){63}} }, /* hardtanh_27_pad_before */
    {.id = 253, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){63}}, .output_tensors = {1, (const int32_t[1]){64}} }, /* hardtanh_27 */
    {.id = 256, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){64}}, .output_tensors = {1, (const int32_t[1]){65}} }, /* getitem_123 */
    {.id = 259, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){65}}, .output_tensors = {1, (const int32_t[1]){66}} }, /* hardtanh_28 */
    {.id = 262, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){66}}, .output_tensors = {1, (const int32_t[1]){67}} }, /* hardtanh_29_pad_before */
    {.id = 262, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){67}}, .output_tensors = {1, (const int32_t[1]){68}} }, /* hardtanh_29 */
    {.id = 265, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){68}}, .output_tensors = {1, (const int32_t[1]){69}} }, /* getitem_132 */
    {.id = 268, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){65, 69}}, .output_tensors = {1, (const int32_t[1]){70}} }, /* add_8 */
    {.id = 271, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){70}}, .output_tensors = {1, (const int32_t[1]){71}} }, /* hardtanh_30 */
    {.id = 274, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){71}}, .output_tensors = {1, (const int32_t[1]){72}} }, /* hardtanh_31_pad_before */
    {.id = 274, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){72}}, .output_tensors = {1, (const int32_t[1]){73}} }, /* hardtanh_31 */
    {.id = 277, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){73}}, .output_tensors = {1, (const int32_t[1]){74}} }, /* getitem_141 */
    {.id = 280, .type = AI_LAYER_ELTWISE_INTEGER_TYPE, .input_tensors = {2, (const int32_t[2]){70, 74}}, .output_tensors = {1, (const int32_t[1]){75}} }, /* add_9 */
    {.id = 283, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){75}}, .output_tensors = {1, (const int32_t[1]){76}} }, /* hardtanh_32 */
    {.id = 286, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){76}}, .output_tensors = {1, (const int32_t[1]){77}} }, /* hardtanh_33_pad_before */
    {.id = 286, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){77}}, .output_tensors = {1, (const int32_t[1]){78}} }, /* hardtanh_33 */
    {.id = 289, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){78}}, .output_tensors = {1, (const int32_t[1]){79}} }, /* getitem_150 */
    {.id = 292, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){79}}, .output_tensors = {1, (const int32_t[1]){80}} }, /* hardtanh_34 */
    {.id = 292, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){80}}, .output_tensors = {1, (const int32_t[1]){81}} }, /* hardtanh_34_0_0_mean_conversion */
    {.id = 295, .type = AI_LAYER_REDUCE_TYPE, .input_tensors = {1, (const int32_t[1]){81}}, .output_tensors = {1, (const int32_t[1]){82}} }, /* mean */
    {.id = 295, .type = AI_LAYER_BN_TYPE, .input_tensors = {1, (const int32_t[1]){82}}, .output_tensors = {1, (const int32_t[1]){83}} }, /* mean_Mul */
    {.id = 295, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){83}}, .output_tensors = {1, (const int32_t[1]){84}} }, /* mean_Mul_0_0_view_conversion */
    {.id = 301, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){84}}, .output_tensors = {1, (const int32_t[1]){85}} } /* output_QuantizeLinear_Input */
  },
  .n_nodes = 85
};
#endif

