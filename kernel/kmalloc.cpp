#include <kmalloc.hpp>
#include <page_alloc.hpp>

static SLOBHeader base;
static SLOBHeader* freep;

void kfree(void* ap)
{
    SLOBHeader *bp, *p;

    bp = (SLOBHeader*)ap - 1;
    for (p = freep; !(bp > p && bp < p->ptr); p = p->ptr)
        if (p >= p->ptr && (bp > p || bp < p->ptr))
            break;
    if (bp + bp->size == p->ptr) {
        bp->size += p->ptr->size;
        bp->ptr = p->ptr->ptr;
    } else
        bp->ptr = p->ptr;
    if (p + p->size == bp) {
        p->size += bp->size;
        p->ptr = bp->ptr;
    } else
        p->ptr = bp;
    freep = p;
}

static SLOBHeader* morecore()
{
    SLOBHeader* hp;

    hp = (SLOBHeader*)alloc_page();
    hp->size = PGSIZE;
    kfree((void*)(hp + 1));
    return freep;
}

void* kmalloc(u32 nbytes)
{
    SLOBHeader *p, *prevp;
    u32 nunits;

    nunits = (nbytes + sizeof(SLOBHeader) - 1) / sizeof(SLOBHeader) + 1;
    if ((prevp = freep) == 0) {
        base.ptr = freep = prevp = &base;
        base.size = 0;
    }
    for (p = prevp->ptr;; prevp = p, p = p->ptr) {
        if (p->size >= nunits) {
            if (p->size == nunits)
                prevp->ptr = p->ptr;
            else {
                p->size -= nunits;
                p += p->size;
                p->size = nunits;
            }
            freep = prevp;
            return (void*)(p + 1);
        }
        if (p == freep)
            p = morecore();
    }
}