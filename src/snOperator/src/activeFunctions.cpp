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
#include "stdafx.h"

using namespace std;
using namespace SN_Base;

// fv - функция значения, df - производная функции


void fv_sigmoid(snFloat* ioVal, size_t sz){
    
    for (size_t i = 0; i < sz; ++i){
    
        ioVal[i] = 1.F / (1.F + std::exp(-ioVal[i]));
        
        if (ioVal[i] >  0.99999) ioVal[i] = 0.99999;
        if (ioVal[i] < 0.00001) ioVal[i] = 0.00001;
    }
}    
void df_sigmoid(snFloat* ioSigm, size_t sz){
    
    for (size_t i = 0; i < sz; ++i){

        ioSigm[i] = ioSigm[i] * (1.F - ioSigm[i]);
    }
}

void fv_relu(snFloat* ioVal, size_t sz){
    
    for (size_t i = 0; i < sz; ++i){

        ioVal[i] = ioVal[i] >= 0 ? ioVal[i] : 0;
    }
};
void df_relu(snFloat* ioRelu, size_t sz){
    
    for (size_t i = 0; i < sz; ++i){

        ioRelu[i] = ioRelu[i] >= 0 ? 1.F : 0.F;
    }
};

void fv_leakyRelu(snFloat* ioVal, size_t sz, snFloat minv){
    
    for (size_t i = 0; i < sz; ++i){

        ioVal[i] = ioVal[i] >= 0 ? ioVal[i] : minv * ioVal[i];
    }
}
void df_leakyRelu(snFloat* ioRelu, size_t sz, snFloat minv){
    
    for (size_t i = 0; i < sz; ++i){

        ioRelu[i] = ioRelu[i] >= 0 ? 1 : minv;
    }
}

void fv_elu(snFloat* ioVal, size_t sz, snFloat minv){
    
    for (size_t i = 0; i < sz; ++i){

        ioVal[i] = ioVal[i] >= 0 ? ioVal[i] : minv * (exp(ioVal[i]) - 1.F);
    }
}
void df_elu(snFloat* ioElu, size_t sz, snFloat minv){
    
    for (size_t i = 0; i < sz; ++i){

        ioElu[i] = ioElu[i] >= 0 ? 1 : ioElu[i] + minv;
    }
}
