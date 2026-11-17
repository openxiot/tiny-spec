/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_str_split.h>
#include <tinycore/string_array.h>
#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/typedef/device/urn/urn.h>

TinyRet Urn_Construct(Urn *thiz)
{
    memset(thiz, 0, sizeof(Urn));

    return TINY_RET_OK;
}

void Urn_Dispose(Urn *thiz)
{
    if (thiz->ns != NULL)
    {
        tiny_free(thiz->ns);
    }

    if (thiz->name != NULL)
    {
        tiny_free(thiz->name);
    }

    if (thiz->vendor != NULL)
    {
        tiny_free(thiz->vendor);
    }

    memset(thiz, 0, sizeof(Urn));
}

TinyRet Urn_Set(Urn *thiz, const char *ns, UrnType type, const char *name, uint32_t value, const char *vendor)
{
    TinyRet ret = TINY_RET_OK;

    do
    {
        size_t length = 0;

        /**
         * ns
         */
        if (thiz->ns != NULL)
        {
            tiny_free(thiz->ns);
        }
        length = strlen(ns) + 1;
        thiz->ns = tiny_malloc(length);
        if (thiz->ns == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }
        memset(thiz->ns, 0, length);
        strncpy(thiz->ns, ns, length);

        /**
         * name
         */
        if (thiz->name != NULL)
        {
            tiny_free(thiz->name);
        }
        length = strlen(name) + 1;
        thiz->name = tiny_malloc(length);
        if (thiz->name == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }
        memset(thiz->name, 0, length);
        strncpy(thiz->name, name, length);

        /**
         * type
         */
        thiz->type = type;

        /**
         * value
         */
        thiz->value = value;

        /**
         * vendor
         */
        if (thiz->vendor != NULL)
        {
            tiny_free(thiz->vendor);
            thiz->vendor = NULL;
        }
        if (vendor != NULL)
        {
            length = strlen(vendor) + 1;
            thiz->vendor = tiny_malloc(length);
            if (thiz->vendor == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }
            memset(thiz->vendor, 0, length);
            strncpy(thiz->vendor, vendor, length);
        }
    } while (false);

    return ret;
}

TinyRet Urn_SetString(Urn *thiz, const char *string)
{
    TinyRet ret = TINY_RET_OK;
    StringArray *array = NULL;

    do
    {
        const char *urn = NULL;
        const char *ns = NULL;
        const char *type = NULL;
        const char *name = NULL;
        const char *value = NULL;
        const char *vendor = NULL;
        uint32_t v = 0;
        char *p = NULL;

        array = StringArray_NewString(string, ":");
        if (array == NULL)
        {
            TINY_LOG_ERROR("StringArray_NewString FAILED");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (! (array->values.size == 5 || array->values.size == 6))
        {
            TINY_LOG_ERROR("array.values.size INVALID: %d", (int) (array->values.size));
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        urn = (const char *) TinyList_GetAt(&array->values, 0);
        if (! STR_EQUAL(urn, "urn"))
        {
            TINY_LOG_ERROR("urn FAILED: %s", urn);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ns = (const char *) TinyList_GetAt(&array->values, 1);
        type = (const char *) TinyList_GetAt(&array->values, 2);
        name = (const char *) TinyList_GetAt(&array->values, 3);
        value = (const char *) TinyList_GetAt(&array->values, 4);
        vendor = (const char *) TinyList_GetAt(&array->values, 5);

        v = (uint32_t) strtol(value, &p, 16);

        ret = Urn_Set(thiz, ns, UrnType_Retrieve(type), name, v, vendor);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("Urn_Set FAILED");
            break;
        }
    } while (false);

    if (array != NULL)
    {
        StringArray_Delete(array);
    }

    return ret;
}
