lite_cc_library(fuse_fc
        SRCS fc_fuser.cc
        DEPS pattern_matcher_high_api)
lite_cc_library(fuse_conv_elementwise
        SRCS conv_elementwise_fuser.cc
        DEPS pattern_matcher_high_api)
lite_cc_library(fuse_conv_activation
        SRCS conv_activation_fuser.cc
        DEPS pattern_matcher_high_api)
lite_cc_library(fuse_conv_bn
        SRCS conv_bn_fuser.cc
        DEPS pattern_matcher_high_api)
lite_cc_library(fuse_elementwise_add_activation
        SRCS elementwise_add_activation_fuser.cc
        DEPS pattern_matcher_high_api)
lite_cc_library(fuse_quant_dequant
        SRCS quant_dequant_op_fuser.cc
        DEPS pattern_matcher_high_api)

set(mir_fusers
    fuse_fc
    fuse_conv_elementwise
    fuse_conv_activation
    fuse_conv_bn
    fuse_quant_dequant
    fuse_elementwise_add_activation
    CACHE INTERNAL "fusers")

if (LITE_WITH_LIGHT_WEIGHT_FRAMEWORK)
    return()
endif()

# TODO(Superjomn) Enable it latter
# NOTE disabled for the proto_desc is not valid yet.
# lite_cc_test(test_lite_conv_bn_fuse SRCS conv_bn_fuse_pass_test.cc
#    DEPS elementwise_ops batch_norm_op conv_op proto_desc compatible_pb program mir_pass mir_pass_manager pattern_matcher_high_api)
