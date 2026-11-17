#include <tinycore/json_array.h>
#include <tinycore/json_object.h>
#include <tinycore/json_string.h>
#include <tinyspec/codec/iq/device/manager/get_children_codec.h>

bool GetChildrenCodec_EncodeQuery(QueryGetChildren *query, JsonObject *content)
{
    RETURN_VAL_IF_FAIL(query, false);
    RETURN_VAL_IF_FAIL(content, false);
    if (query->did != NULL)
    {
        JsonObject_PutString(content, "did", query->did->value);
    }
    return true;
}

TINYCORE_RET GetChildrenCodec_DecodeQuery(JsonObject *content, QueryGetChildren *query)
{
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_PARAM_INVALID);
    RETURN_VAL_IF_FAIL(query, TINY_RET_E_PARAM_INVALID);
    if (RET_FAILED(QueryGetChildren_Construct(query)))
    {
        return TINY_RET_E_NULL_PTR;
    }
    JsonString *did = JsonObject_GetString(content, "did");
    if (did != NULL)
    {
        query->did = JsonString_New(did->value);
    }
    return TINY_RET_OK;
}

/* 单个孩子 -> {"did":..,"summary":{"type":..,"online":..}}，写进 obj */
bool GetChildrenCodec_EncodeChild(const Child *child, JsonObject *obj)
{
    RETURN_VAL_IF_FAIL(child, false);
    RETURN_VAL_IF_FAIL(obj, false);

    JsonObject_PutString(obj, "did", child->did);

    JsonObject *summary = JsonObject_New();
    JsonObject_PutString(summary, "type", child->type);
    JsonObject_PutBoolean(summary, "online", child->online);
    JsonObject_PutObject(obj, "summary", summary);
    return true;
}

bool GetChildrenCodec_EncodeResult(ResultGetChildren *result, JsonObject *content)
{
    RETURN_VAL_IF_FAIL(result, false);
    RETURN_VAL_IF_FAIL(content, false);

    JsonArray *children = JsonArray_New();
    RETURN_VAL_IF_FAIL(children, false);

    for (uint32_t i = 0; i < TinyList_GetSize(&result->children); ++i)
    {
        Child *child = (Child *)TinyList_GetAt(&result->children, i);
        JsonObject *co = JsonObject_New();
        if (child != NULL && co != NULL && GetChildrenCodec_EncodeChild(child, co))
        {
            JsonArray_AddObject(children, co);
        }
        else if (co != NULL)
        {
            JsonObject_Delete(co);
        }
    }
    JsonObject_PutArray(content, "children", children);
    return true;
}

TINYCORE_RET GetChildrenCodec_DecodeResult(JsonObject *content, ResultGetChildren *result)
{
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_PARAM_INVALID);
    RETURN_VAL_IF_FAIL(result, TINY_RET_E_PARAM_INVALID);
    if (RET_FAILED(ResultGetChildren_Construct(result)))
    {
        return TINY_RET_E_NULL_PTR;
    }
    JsonArray *children = JsonObject_GetArray(content, "children");
    if (children != NULL)
    {
        for (uint32_t i = 0; i < JsonArray_GetSize(children); ++i)
        {
            JsonObject *co = JsonArray_GetObject(children, i);
            Child *child = Child_New();
            if (child == NULL || co == NULL)
            {
                continue;
            }
            JsonString *did = JsonObject_GetString(co, "did");
            if (did != NULL)
            {
                snprintf(child->did, sizeof(child->did), "%s", did->value);
            }
            JsonObject *summary = JsonObject_GetObject(co, "summary");
            if (summary != NULL)
            {
                const char *type = JsonObject_GetStringValue(summary, "type");
                if (type != NULL)
                {
                    snprintf(child->type, sizeof(child->type), "%s", type);
                }
                { JsonBoolean *onlineB = JsonObject_GetBoolean(summary, "online"); child->online = (onlineB != NULL) ? onlineB->value : false; }
            }
            TinyList_AddTail(&result->children, child);
        }
    }
    return TINY_RET_OK;
}
