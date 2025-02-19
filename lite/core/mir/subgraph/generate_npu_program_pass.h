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

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "lite/core/mir/pass.h"
#include "lite/core/mir/subgraph/subgraph_program_pass.h"
#include "lite/npu/bridge/registry.h"
#include "lite/npu/npu_helper.h"

namespace paddle {
namespace lite {
namespace mir {
namespace subgraph {

class GenerateNPUProgramPass : public SubgraphProgramPass {
 public:
  using key2nodes_t = std::map<std::string, Node*>;

  void Apply(const std::unique_ptr<SSAGraph>& graph) override;
  std::unique_ptr<RuntimeProgram> GenProgram();

 protected:
  void NPUSortHelper(Node* node,
                     const std::unordered_set<Node*>& nodes_all,
                     std::unordered_set<const Node*>* visited_nodes,
                     std::vector<Node*>* ret);

  // nodes2cvt: op nodes to convert
  // in_vars_name: graph op's inputs var name
  // out_vars_name: graph op's outputs var name
  // vcted_vars:
  // nodes2rm: op nodes and var nodes that need to be removed
  void CvtOpNodes(const std::vector<Node*>& nodes2cvt,
                  std::vector<std::string>* in_vars_name,
                  std::vector<std::string>* out_vars_name,
                  lite::npu::bridge::node_map_type* cvted_vars,
                  std::unordered_set<const Node*>* nodes2rm);

  void GenNPUGraphOpNode(const std::unique_ptr<SSAGraph>& graph,
                         int sub_id,
                         const std::unordered_set<Node*>& nodes_all);

  void ConvertSubgraph(const std::unique_ptr<SSAGraph>& graph, int sub_num);

 private:
  std::vector<Instruction> insts_;
};

}  // namespace subgraph
}  // namespace mir
}  // namespace lite
}  // namespace paddle
