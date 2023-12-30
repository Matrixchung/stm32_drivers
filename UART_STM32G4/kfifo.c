#include "kfifo.h"
kfifo_t *kfifo_alloc(uint32_t size)
{
    char *buffer = NULL;
    kfifo_t *ret = NULL;
    ret = (kfifo_t *)MEMALLOC(sizeof(kfifo_t));
    if(size & (size - 1)) size = roundup_pow2(size);
    buffer = (char *)MEMALLOC(size);
    if(!buffer) return 0; // fail to allocate buffer memory
    ret->buffer = buffer;
    ret->size = size;
    ret->in = 0;
    ret->out = 0;
    if(!ret) MEMFREE(buffer);
    return ret;
}
// void kfifo_free(kfifo_t *fifo)
// {
//     MEMFREE(fifo->buffer);
//     fifo->in = 0;
//     fifo->out = 0;
//     fifo->size = 0;
//     fifo->buffer = NULL;
// }
uint32_t kfifo_put(kfifo_t *fifo, char *buffer, uint32_t len)
{
    uint32_t l;
    len = uint32_min(len, (fifo->size - fifo->in + fifo->out));
    l = uint32_min(len, (fifo->size - (fifo->in & (fifo->size - 1))));
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
    memcpy(fifo->buffer, buffer + l, len - l);
    fifo->in += len;
    return len;
}
uint32_t kfifo_peek(kfifo_t *fifo, char *buffer, uint32_t len)
{
    uint32_t l;
    len = uint32_min(len, (fifo->in - fifo->out));
    l = uint32_min(len, (fifo->size - (fifo->out & (fifo->size - 1))));
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
    memcpy(buffer + l, fifo->buffer, len - l);
    return len;
}
uint32_t kfifo_get(kfifo_t *fifo, char *buffer, uint32_t len)
{
    len = kfifo_peek(fifo, buffer, len);
    fifo->out += len;
    return len;
}