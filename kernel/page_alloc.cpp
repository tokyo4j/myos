#include <int.hpp>
#include <page_alloc.hpp>
#include <screen.hpp>
#include <utils.hpp>

struct RunNode {
    RunNode* next;
};

class Run {
private:
    RunNode* head;

public:
    void push(RunNode* new_node)
    {
        new_node->next = head;
        head = new_node;
    }

    RunNode* pop()
    {
        RunNode* old_head = head;
        head = head->next;
        return old_head;
    }
};

static Run freelist;

void free_page(void* va)
{
    freelist.push((RunNode*)va);
}

void free_pages(void* va_start, void* va_end)
{
    ASSERT((u32)va_start % PGSIZE == 0)
    ASSERT((u32)va_end % PGSIZE == 0)

    for (u8* p = (u8*)va_start; p < (u8*)va_end; p += PGSIZE)
        free_page(p);
}

void* alloc_page()
{
    return freelist.pop();
}