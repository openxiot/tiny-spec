#include <tinyspec/typedef/product/child/child.h>
#include <tinycore/tiny_alloc.h>

Child * Child_New(void)
{
    return (Child *)tiny_calloc(1, sizeof(Child));
}

void Child_Delete(Child *thiz)
{
    if (thiz == NULL)
    {
        return;
    }
    tiny_free(thiz);
}
