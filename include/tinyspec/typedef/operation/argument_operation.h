/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_ARGUMENT_OPERATION_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_ARGUMENT_OPERATION_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_array.h>
#include <tinycore/json_value.h>
#include <tinycore/tiny_list.h>

TINYSPEC_BEGIN_DECLS

/**
 * 动作/事件的一个参数运行值（对齐 Java ArgumentOperation）：
 * piid 标识参数定义，values 保存该参数的一个或多个值（repeat）。
 * 组合属性的每个 value 为裸 iid 键的 JSON_OBJECT。
 */
typedef struct
{
    uint16_t            piid;
    JsonArray           values;
} ArgumentOperation;

TINYSPEC_API
ArgumentOperation * ArgumentOperation_New(uint16_t piid);

TINYSPEC_API
void ArgumentOperation_Delete(ArgumentOperation *thiz);

TINYSPEC_API
TinyRet ArgumentOperation_Construct(ArgumentOperation *thiz, uint16_t piid);

TINYSPEC_API
void ArgumentOperation_Dispose(ArgumentOperation *thiz);

TINYSPEC_API
TinyRet ArgumentOperation_Copy(ArgumentOperation *dst, const ArgumentOperation *src);

TINYSPEC_API
ArgumentOperation * ArgumentOperation_CopyFrom(const ArgumentOperation *other);

/** 深拷贝一个值追加到 values。 */
TINYSPEC_API
TinyRet ArgumentOperation_AddValue(ArgumentOperation *thiz, const JsonValue *value);

/**
 * 解析 wire 参数列表到 TinyList（元素 ArgumentOperation*）：
 *   - 标准形态 [{"piid": N, "values": [...]}, ...]
 *   - 紧凑形态 {"#N": [...]}
 * 也接受 Java 的“仅 piid 整数”元素（无值）。
 * out 必须是已 Construct 的 TinyList（元素所有权交给 out，需自带 delete listener）。
 */
TINYSPEC_API
TinyRet ArgumentOperations_Decode(const JsonValue *wire, TinyList *out, bool *compact);

/** 按 compact 编码参数列表，返回新 wire 值（调用方持有）。 */
TINYSPEC_API
JsonValue * ArgumentOperations_Encode(const TinyList *operations, bool compact);

/** 给 ArgumentOperation* 列表安装删除监听（Dispose/Clear 时释放参数）。 */
TINYSPEC_API
TinyRet ArgumentOperations_SetDeleteListener(TinyList *operations);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_ARGUMENT_OPERATION_H */
