/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_COMBINATION_VALUE_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_COMBINATION_VALUE_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_value.h>

TINYSPEC_BEGIN_DECLS

/**
 * 组合属性值的 wire 编解码（语义对齐 Java CombinationValueCodec）。
 *
 * 内部规范形态：JSON_OBJECT，键为成员属性裸 iid（如 "8"），成员值递归规范化。
 * wire 形态有两种：
 *   - 标准：JSON_ARRAY [{"piid": 8, "value": ...}, ...]
 *   - 紧凑：JSON_OBJECT {"#8": ...}
 *
 * 返回的新值由调用方持有（JsonValue_Delete）。
 */

/** wire -> 规范形态。非组合形态（普通数组属性值）返回 NULL。 */
TINYSPEC_API
JsonValue * CombinationValue_Decode(const JsonValue *wire, bool *compact);

/** 规范形态 -> wire（按 compact 选择紧凑对象或标准数组）。 */
TINYSPEC_API
JsonValue * CombinationValue_Encode(const JsonValue *canonical, bool compact);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_COMBINATION_VALUE_H */
