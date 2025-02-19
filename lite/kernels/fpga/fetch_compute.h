// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once
#include "lite/core/op_registry.h"
#include "lite/core/type_system.h"
#include "lite/fpga/KD/pes/output_pe.hpp"

namespace paddle {
namespace lite {
namespace kernels {
namespace fpga {

class FetchCompute
    : public KernelLite<TARGET(kFPGA), PRECISION(kFP16), DATALAYOUT(kNHWC)> {
 public:
  using param_t = operators::FetchParam;

  void PrepareForRun() override;
  void Run() override;

 private:
  zynqmp::OutputPE pe_;
  zynqmp::Tensor input_;
  zynqmp::Tensor output_;
};

}  // namespace fpga
}  // namespace kernels
}  // namespace lite
}  // namespace paddle
