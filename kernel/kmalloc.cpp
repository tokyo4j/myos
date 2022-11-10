#include <kmalloc.hpp>
#include <page_alloc.hpp>
#include <screen.hpp>

static slob_header base;
static slob_header* freep = nullptr;

void kfree(void* ap)
{
    slob_header *bp, *p;

    bp = (slob_header*)ap - 1;
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

static slob_header* morecore(u32 nu)
{
    slob_header* hp;

    hp = (slob_header*)alloc_page();
    hp->size = PGSIZE;
    kfree((void*)(hp + 1));
    return freep;
}

void* kmalloc(u32 nbytes)
{
    slob_header *p, *prevp;
    u32 nunits;

    nunits = (nbytes + sizeof(slob_header) - 1) / sizeof(slob_header) + 1;
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
            p = morecore(nunits);
    }
}