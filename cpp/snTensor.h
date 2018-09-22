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


#include <cstdint>
#include <string>
#include <vector>
#include "../src/skynet/skyNet.h"

namespace SN_API{
      
    class Tensor{
        
    public:

        Tensor(const snLSize& lsz, std::vector<snFloat>& data){
        
            lsz_ = lsz;
            data_ = data;
        };

        Tensor(const snLSize& lsz, snFloat* data){

            lsz_ = lsz; 
            size_t sz = lsz.w * lsz.h * lsz.ch * lsz.bsz;
         
            data_ = std::vector<snFloat>(sz);

            memcpy(data_.data(), data, sz * sizeof(snFloat));
        };

        ~Tensor(){};
         
        snFloat* data(){

            return data_.data();
        }

        snLSize size(){

            return lsz_;
        }

    private:
        snLSize lsz_;
        std::vector<snFloat> data_;
     
    };    
}
