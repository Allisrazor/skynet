
//
// SkyNet Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/skynet>
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#pragma once

#include "snBase/snBase.h"
#include <algorithm>
#include <immintrin.h>

namespace SN_SIMD{

    const int LAYER_MAX_WIDTH = 800;
    const int LAYER_MAX_HEIGHT = 600;
    const int REG_CNT = 16;                   // registr count
    const int REG_BYTE_SZ = 32;               // registr byte size  (256 bit = 32 B = 8 float)
    const int L1_BYTE_SZ = 32 * 1024;         // L1 cache byte size (32 kB)
    const int L2_BYTE_SZ = 256 * 1024;        // L2 cache byte size (256 kB)
    const int L3_BYTE_SZ = 2 * 1024 * 1024;   // L3 cache byte size (2 MB/core)

#define LOAD_REG(in, reg)  __m256 reg = _mm256_loadu_ps(in);
#define LOAD_REG_FROM_BUFF_3x3_1DIL(in, buff, w, reg) buff[0] = *(in);               \
                                                      buff[1] = *(in + 1);           \
                                                      buff[2] = *(in + 2);           \
                                                      buff[3] = *(in + (w));         \
                                                      buff[4] = *(in + (w) + 1);     \
                                                      buff[5] = *(in + (w) + 2);     \
                                                      buff[6] = *(in + 2 * (w));     \
                                                      buff[7] = *(in + 2 * (w) + 1); \
                                                   __m256 reg = _mm256_loadu_ps(buff);

#define LOAD_1REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0);

#define LOAD_2REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1);

#define LOAD_3REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2);

#define LOAD_4REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3);

#define LOAD_5REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4);

#define LOAD_6REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5);

#define LOAD_7REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6);

#define LOAD_8REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 7);

#define LOAD_9REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 7); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 8);

#define LOAD_10REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 7); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 8); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 9);

#define LOAD_11REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 7); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 8); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 9); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 10);

#define LOAD_12REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 7); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 8); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 9); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 10); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 11);

#define LOAD_13REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 7); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 8); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 9); in += (w) * (h);  \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 10); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 11); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 12); 

#define LOAD_14REG_FROM_BUFF(m, d, in, buff, w, h, reg) \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 0); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 1); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 2); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 3); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 4); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 5); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 6); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 7); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 8); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 9); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 10); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 11); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 12); in += (w) * (h); \
         LOAD_REG_FROM_BUFF_ ## m ## x ## m ## _ ## d ## DIL(in, buff, w, reg ## 13);

#define SUMM_1REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); 

#define SUMM_2REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut);

#define SUMM_3REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut);

#define SUMM_4REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut);


#define SUMM_5REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut);

#define SUMM_6REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut);

#define SUMM_7REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut);

#define SUMM_8REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 7, arW, arOut);

#define SUMM_9REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 7, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 8, arW, arOut);

#define SUMM_10REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 7, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 8, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 9, arW, arOut);

#define SUMM_11REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 7, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 8, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 9, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 10, arW, arOut);

#define SUMM_12REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 7, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 8, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 9, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 10, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 11, arW, arOut);

#define SUMM_13REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 7, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 8, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 9, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 10, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 11, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 12, arW, arOut);

#define SUMM_14REG(m, weight, arIn, arW, arOut) \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 0, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 1, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 2, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 3, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 4, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 5, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 6, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 7, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 8, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 9, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 10, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 11, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 12, arW, arOut); weight += (m) * (m); \
          arW = _mm256_loadu_ps(weight); arOut = _mm256_fmadd_ps(arIn ## 13, arW, arOut);


    struct buf_t{

        SN_Base::snFloat* p;
        size_t n;

        buf_t(size_t size) : n(size), p((SN_Base::snFloat*)_mm_malloc(size * 4, 64)) {}
        ~buf_t() { _mm_free(p); }
    };
    
    template<typename T>
    float horSummReg(T a);

    template<>
    float horSummReg<__m256>(__m256 a){

        __m128 hi = _mm256_extractf128_ps(a, 1);
        __m128 lo = _mm256_extractf128_ps(a, 0);
        lo = _mm_add_ps(hi, lo);
        hi = _mm_movehl_ps(hi, lo);
        lo = _mm_add_ps(hi, lo);
        hi = _mm_shuffle_ps(lo, lo, 1);
        lo = _mm_add_ss(hi, lo);
        return _mm_cvtss_f32(lo);
    };
};