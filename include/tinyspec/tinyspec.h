/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_TINYSPEC_H
#define TINYSPEC_TINYSPEC_H

/* Convenience umbrella header: includes every public TinySpec module. */

#include <tinyspec/tinyspec_base.h>

/* IotTypedef - device description model */
#include <tinyspec/typedef/constant/typedef_constant.h>
#include <tinyspec/typedef/device/action.h>
#include <tinyspec/typedef/device/argument.h>
#include <tinyspec/typedef/device/device.h>
#include <tinyspec/typedef/device/event.h>
#include <tinyspec/typedef/device/property.h>
#include <tinyspec/typedef/device/property/access/access.h>
#include <tinyspec/typedef/device/property/access/operation_type.h>
#include <tinyspec/typedef/device/property/constraint/value_list.h>
#include <tinyspec/typedef/device/property/constraint/value_range.h>
#include <tinyspec/typedef/device/property/format/format.h>
#include <tinyspec/typedef/device/service.h>
#include <tinyspec/typedef/device/urn/urn.h>
#include <tinyspec/typedef/device/urn/urn_type.h>
#include <tinyspec/typedef/notice/device/device_notice.h>
#include <tinyspec/typedef/notice/device/device_notice_codec.h>
#include <tinyspec/typedef/notice/device/impl/device_event_occurred.h>
#include <tinyspec/typedef/notice/device/impl/device_event_occurred_codec.h>
#include <tinyspec/typedef/notice/device/impl/device_property_changed.h>
#include <tinyspec/typedef/notice/device/impl/device_property_changed_codec.h>
#include <tinyspec/typedef/notice/notice.h>
#include <tinyspec/typedef/operation/action_operation.h>
#include <tinyspec/typedef/operation/argument_operation.h>
#include <tinyspec/typedef/operation/codec/action_operation_codec.h>
#include <tinyspec/typedef/operation/codec/property_operations_codec.h>
#include <tinyspec/typedef/operation/combination_value.h>
#include <tinyspec/typedef/operation/event_operation.h>
#include <tinyspec/typedef/operation/property_operation.h>
#include <tinyspec/typedef/operation/property_operations.h>
#include <tinyspec/typedef/product/config/product_config.h>
#include <tinyspec/typedef/product/handler/action_handler.h>
#include <tinyspec/typedef/product/handler/identify_listener.h>
#include <tinyspec/typedef/product/handler/property_handler.h>
#include <tinyspec/typedef/product/observer/event_observer.h>
#include <tinyspec/typedef/product/observer/property_observer.h>
#include <tinyspec/typedef/product/operator/product_operator.h>
#include <tinyspec/typedef/product/product.h>
#include <tinyspec/typedef/status/iot_status.h>
#include <tinyspec/typedef/xid/action_id.h>
#include <tinyspec/typedef/xid/codec/action_id_codec.h>
#include <tinyspec/typedef/xid/codec/event_id_codec.h>
#include <tinyspec/typedef/xid/codec/property_id_codec.h>
#include <tinyspec/typedef/xid/codec/xid3_codec.h>
#include <tinyspec/typedef/xid/event_id.h>
#include <tinyspec/typedef/xid/property_id.h>
#include <tinyspec/typedef/xid/xid3.h>

/* Stanza - IQ and message model */
#include <tinyspec/stanza/iq/basic/factory/bye_factory.h>
#include <tinyspec/stanza/iq/basic/factory/hello_factory.h>
#include <tinyspec/stanza/iq/basic/factory/ping_factory.h>
#include <tinyspec/stanza/iq/basic/factory/pong_factory.h>
#include <tinyspec/stanza/iq/device/control/factory/get_properties_factory.h>
#include <tinyspec/stanza/iq/device/control/factory/get_property_factory.h>
#include <tinyspec/stanza/iq/device/control/factory/invoke_action_factory.h>
#include <tinyspec/stanza/iq/device/control/factory/invoke_actions_factory.h>
#include <tinyspec/stanza/iq/device/control/factory/set_properties_factory.h>
#include <tinyspec/stanza/iq/device/control/factory/set_property_factory.h>
#include <tinyspec/stanza/iq/device/control/get_properties.h>
#include <tinyspec/stanza/iq/device/control/get_property.h>
#include <tinyspec/stanza/iq/device/control/invoke_action.h>
#include <tinyspec/stanza/iq/device/control/set_properties.h>
#include <tinyspec/stanza/iq/device/control/set_property.h>
#include <tinyspec/stanza/iq/device/key/factory/get_access_key_factory.h>
#include <tinyspec/stanza/iq/device/key/factory/set_access_key_factory.h>
#include <tinyspec/stanza/iq/device/key/get_access_key.h>
#include <tinyspec/stanza/iq/device/key/set_access_key.h>
#include <tinyspec/stanza/iq/device/manager/get_children.h>
#include <tinyspec/stanza/iq/device/manager/factory/get_children_factory.h>
#include <tinyspec/stanza/iq/device/verify/factory/initialize_factory.h>
#include <tinyspec/stanza/iq/device/verify/factory/verify_finish_factory.h>
#include <tinyspec/stanza/iq/device/verify/factory/verify_start_factory.h>
#include <tinyspec/stanza/iq/device/verify/initialize.h>
#include <tinyspec/stanza/iq/device/verify/verify_finish.h>
#include <tinyspec/stanza/iq/device/verify/verify_start.h>
#include <tinyspec/stanza/iq/iq.h>
#include <tinyspec/stanza/iq/iq_error.h>
#include <tinyspec/stanza/iq/iq_error_factory.h>
#include <tinyspec/stanza/iq/iq_method.h>
#include <tinyspec/stanza/iq/iq_query.h>
#include <tinyspec/stanza/iq/iq_result.h>
#include <tinyspec/stanza/message/message.h>
#include <tinyspec/stanza/stanza.h>
#include <tinyspec/stanza/stanza_constant.h>

/* StanzaCodec - JSON codecs */
#include <tinyspec/codec/iq/device/control/get_properties_codec.h>
#include <tinyspec/codec/iq/device/control/get_property_codec.h>
#include <tinyspec/codec/iq/device/control/invoke_action_codec.h>
#include <tinyspec/codec/iq/device/control/set_properties_codec.h>
#include <tinyspec/codec/iq/device/control/set_property_codec.h>
#include <tinyspec/codec/iq/device/key/get_access_key_codec.h>
#include <tinyspec/codec/iq/device/key/set_access_key_codec.h>
#include <tinyspec/codec/iq/device/verify/initialize_codec.h>
#include <tinyspec/codec/iq/device/verify/verify_finish_codec.h>
#include <tinyspec/codec/iq/device/verify/verify_start_codec.h>
#include <tinyspec/codec/iq/iq_codec.h>
#include <tinyspec/codec/iq/iq_error_codec.h>
#include <tinyspec/codec/iq/iq_query_codec.h>
#include <tinyspec/codec/iq/iq_result_codec.h>
#include <tinyspec/codec/message/message_codec.h>
#include <tinyspec/codec/stanza_codec.h>

#endif /* TINYSPEC_TINYSPEC_H */
