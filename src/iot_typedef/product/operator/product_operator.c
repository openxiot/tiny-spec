/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/device/service.h>
#include <tinyspec/iot_typedef/status/iot_status.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/product/operator/product_operator.h>

static Product * Product_GetChild(Product *thiz, const char * did)
{
    for (uint32_t i = 0; i < thiz->children.size; ++i)
    {
        Product * child = (Product *)TinyList_GetAt(&thiz->children, i);
        if (STR_EQUAL(child->config.did, did))
        {
            return child;
        }
    }

    return NULL;
}

static Service * Product_GetService(Product *thiz, uint16_t siid)
{
    for (uint32_t i = 0; i < thiz->device.services.size; ++i)
    {
        Service *s = (Service *) TinyList_GetAt(&thiz->device.services, i);
        if (s->iid == siid)
        {
            return s;
        }
    }

    return NULL;
}

static void Product_TryRead(Product *thiz, PropertyOperation *o)
{
    Service * service = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);
    service = Product_GetService(thiz, o->pid.siid);
    if (service != NULL)
    {
        Service_TryRead(service, o);
        if (o->status == IOT_STATUS_COMPLETED)
        {
            if (thiz->onGet != NULL)
            {
                thiz->onGet(o, thiz);
                Service_CheckValue(service, o);
            }
            else
            {
                o->status = IOT_STATUS_INTERNAL_ERROR;
            }
        }
    }
    else
    {
        o->status = IOT_STATUS_SERVICE_NOT_FOUND;
    }
}

static void Product_TryReadChild(Product *thiz, PropertyOperation *o)
{
    Product *child = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);
    child = Product_GetChild(thiz, o->pid.did);
    if (child != NULL)
    {
        Product_TryRead(child, o);
    }
    else
    {
        o->status = IOT_STATUS_CHILD_NOT_FOUND;
    }
}

static void Product_TryWrite(Product *thiz, PropertyOperation *o)
{
    Service * service = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);
    service = Product_GetService(thiz, o->pid.siid);
    if (service != NULL)
    {
        Service_TryWrite(service, o);
        if (o->status == IOT_STATUS_COMPLETED)
        {
            if (thiz->onSet != NULL)
            {
                thiz->onSet(o, thiz);
            }
            else
            {
                o->status = IOT_STATUS_INTERNAL_ERROR;
            }
        }
    }
    else
    {
        o->status = IOT_STATUS_SERVICE_NOT_FOUND;
    }
}

#if 0
static void Product_TrySubscribe(Product *thiz, PropertyOperation *o)
{
    Service * service = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    service = Product_GetService(thiz, o->pid.siid);
    if (service != NULL)
    {
        Service_TrySubscribe(service, o);
    }
    else
    {
        o->status = IOT_STATUS_NOT_EXIST;
    }
}

static void Product_TryUnsubscribe(Product *thiz, PropertyOperation *o)
{
    Service * service = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    service = Product_GetService(thiz, o->pid.siid);
    if (service != NULL)
    {
        Service_TryUnsubscribe(service, o);
    }
    else
    {
        o->status = IOT_STATUS_NOT_EXIST;
    }
}
#endif

static void Product_TryWriteChild(Product *thiz, PropertyOperation *o)
{
    Product *child = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);
    child = Product_GetChild(thiz, o->pid.did);
    if (child != NULL)
    {
        Product_TryWrite(child, o);
    }
    else
    {
        o->status = IOT_STATUS_CHILD_NOT_FOUND;
    }
}

#if 0
static void Product_TrySubscribeChild(Product *thiz, PropertyOperation *o)
{
    Product *child = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    child = Product_GetChild(thiz, o->pid.did);
    if (child != NULL)
    {
        Product_TrySubscribe(child, o);
    }
    else
    {
        o->status = IOT_STATUS_NOT_EXIST;
    }
}

static void Product_TryUnsubscribeChild(Product *thiz, PropertyOperation *o)
{
    Product *child = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    child = Product_GetChild(thiz, o->pid.did);
    if (child != NULL)
    {
        Product_TryUnsubscribe(child, o);
    }
    else
    {
        o->status = IOT_STATUS_NOT_EXIST;
    }
}
#endif

static void Product_TryInvoke(Product *thiz, ActionOperation *o)
{
    Service * service = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);
    service = Product_GetService(thiz, o->aid.siid);
    if (service != NULL)
    {
        Service_TryInvoke(service, o);
        if (o->status == IOT_STATUS_COMPLETED)
        {
            if (thiz->onInvoke != NULL)
            {
                thiz->onInvoke(o, thiz);
                Service_CheckResult(service, o);
            }
            else
            {
                o->status = IOT_STATUS_INTERNAL_ERROR;
            }
        }
    }
    else
    {
        o->status = IOT_STATUS_SERVICE_NOT_FOUND;
    }
}

static void Product_TryInvokeChild(Product *thiz, ActionOperation *o)
{
    Product *child = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);
    child = Product_GetChild(thiz, o->aid.did);
    if (child != NULL)
    {
        Product_TryInvoke(child, o);
    }
    else
    {
        o->status = IOT_STATUS_CHILD_NOT_FOUND;
    }
}

PropertyOperations * Product_GetAllProperties(Product *thiz)
{
    PropertyOperations * operations = NULL;

    RETURN_VAL_IF_FAIL(thiz, NULL);
    do
    {
        operations = PropertyOperations_New();
        if (operations == NULL)
        {
            TINY_LOG_ERROR("PropertyOperations_New");
            break;
        }

        for (uint32_t i = 0; i < thiz->device.services.size; ++i)
        {
            Service *service = (Service *)TinyList_GetAt(&thiz->device.services, i);
            for (uint32_t k = 0; k < service->properties.size; ++k)
            {
                Property *property = (Property *)TinyList_GetAt(&service->properties, k);
                if (Access_IsReadable(property->access))
                {
                    PropertyOperation * operation = PropertyOperation_NewFrom(thiz->config.did, service->iid, property->iid);
                    if (operation != NULL)
                    {
                        Product_TryRead(thiz, operation);

                        if (TinyList_AddTail(&operations->properties, operation) == NULL)
                        {
                            TINY_LOG_ERROR("TinyList_AddTail failed");
                        }
                    }
                    else
                    {
                        TINY_LOG_ERROR("PropertyOperation_New");
                    }
                }
            }
        }

    } while (false);

    return operations;
}

void Product_TryReadProperties(Product *thiz, PropertyOperations *operations)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(operations);
    for (uint32_t i = 0; i < operations->properties.size; ++i)
    {
        PropertyOperation *o = (PropertyOperation *)TinyList_GetAt(&operations->properties, i);
        if (STR_EQUAL(thiz->config.did, o->pid.did))
        {
            Product_TryRead(thiz, o);
        }
        else
        {
            Product_TryReadChild(thiz, o);
        }
    }
}

void Product_TryWriteProperties(Product *thiz, PropertyOperations *operations)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(operations);
    for (uint32_t i = 0; i < operations->properties.size; ++i)
    {
        PropertyOperation *o = (PropertyOperation *)TinyList_GetAt(&operations->properties, i);
        if (STR_EQUAL(thiz->config.did, o->pid.did))
        {
            Product_TryWrite(thiz, o);
        }
        else
        {
            Product_TryWriteChild(thiz, o);
        }
    }
}

#if 0
void Product_TrySubscribeProperties(Product *thiz, PropertyOperations *operations)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(operations);

    for (uint32_t i = 0; i < operations->operations.size; ++i)
    {
        PropertyOperation *o = (PropertyOperation *)TinyList_GetAt(&operations->operations, i);
        if (STR_EQUAL(thiz->config.did, o->pid.did))
        {
            Product_TrySubscribe(thiz, o);
        }
        else
        {
            Product_TrySubscribeChild(thiz, o);
        }
    }
}

void Product_TryUnsubscribeProperties(Product *thiz, PropertyOperations *operations)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(operations);

    for (uint32_t i = 0; i < operations->operations.size; ++i)
    {
        PropertyOperation *o = (PropertyOperation *)TinyList_GetAt(&operations->operations, i);
        if (STR_EQUAL(thiz->config.did, o->pid.did))
        {
            Product_TryUnsubscribe(thiz, o);
        }
        else
        {
            Product_TryUnsubscribeChild(thiz, o);
        }
    }
}
#endif

void Product_TryInvokeAction(Product *thiz, ActionOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);
    if (STR_EQUAL(thiz->config.did, o->aid.did))
    {
        Product_TryInvoke(thiz, o);
    }
    else
    {
        Product_TryInvokeChild(thiz, o);
    }
}

TINYCORE_RET Product_TryChangePropertyValue(Product *thiz, PropertyOperation *o, bool save)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(o, TINY_RET_E_NULL_PTR);
    do
    {
        Service * service = Product_GetService(thiz, o->pid.siid);
        if (service == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        Service_TryChange(service, o, save);
    } while (false);

    return ret;
}

TINYCORE_RET Product_TryProduceEvent(Product *thiz, EventOperation *o)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(o, TINY_RET_E_NULL_PTR);
    do
    {
        Service * service = Product_GetService(thiz, o->eid.siid);
        if (service == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        Service_TryProduce(service, o);
    } while (false);

    return ret;
}

TINYCORE_RET Product_DoChangePropertyValue(Product *thiz, PropertyOperation *o)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(o, TINY_RET_E_NULL_PTR);
    return Product_TryChangePropertyValue(thiz, o, true);
}

TINYSPEC_API
PropertyOperations * Product_GetChangedProperties(Product *thiz)
{
    PropertyOperations *changed = NULL;

    RETURN_VAL_IF_FAIL(thiz, NULL);
    changed = PropertyOperations_New();
    if (changed != NULL)
    {
        for (uint32_t i = 0; i < thiz->device.services.size; ++i)
        {
            Service *s = (Service *) TinyList_GetAt(&thiz->device.services, i);
            for (uint32_t j = 0; j < s->properties.size; ++j)
            {
                Property *p = (Property *) TinyList_GetAt(&s->properties, j);
                if (p->value != NULL)
                {
                    PropertyOperation *o = PropertyOperation_New();
                    if (o == NULL)
                    {
                        TINY_LOG_ERROR("PropertyOperation_New failed!");
                        break;
                    }

                    strncpy(o->pid.did, thiz->config.did, IOT_DEVICE_ID_LENGTH);
                    o->pid.siid = s->iid;
                    o->pid.iid = p->iid;

                    o->value = JsonValue_NewFrom(p->value);
                    if (o->value == NULL)
                    {
                        TINY_LOG_ERROR("JsonValue_NewFrom failed!");
                        PropertyOperation_Delete(o);
                        break;
                    }

                    if (TinyList_AddTail(&changed->properties, o) == NULL)
                    {
                        TINY_LOG_ERROR("TinyList_AddTail failed!");
                        PropertyOperation_Delete(o);
                        break;
                    }
                }
            }
        }
    }

    return changed;
}
