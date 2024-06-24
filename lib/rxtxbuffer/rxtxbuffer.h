#ifndef __RXTXBUFFER_H__
#define __RXTXBUFFER_H__

#include <stddef.h>

#define RXTXBUFFER_SUCCESS						0
#define RXTXBUFFER_ERROR_INVALID_PARAMETERS		1

#define RXTXBUFFER_DEFAULT(vec) vec.data=NULL; vec.capacity=(size_t)0; vec.sent_size=(size_t)0; vec.recved_size=(size_t)0

#define RXTXBUFFER_CAST(ptr, type) ((type)(ptr))

typedef struct rxtxbuffer {
    void* data;
    size_t capacity;
    size_t sent_size;
    size_t recved_size;
}rxtxbuffer;


void* rxtxbuffer_data(rxtxbuffer* pt);

size_t rxtxbuffer_sent_size(rxtxbuffer* pt);

size_t rxtxbuffer_recved_size(rxtxbuffer* pt);

size_t rxtxbuffer_capacity(rxtxbuffer* pt);

int rxtxbuffer_init(rxtxbuffer* _Cvector, void* buf, size_t _recved_size, size_t _sent_size, size_t _capacity);

// total ammount of data that is free to be received
size_t rxtxbuffer_rx_remaining(rxtxbuffer* sendbuf);

// total ammount of tx data that remains to be sent
size_t rxtxbuffer_tx_remaining(rxtxbuffer* sendbuf);

// decrease the ammount of the tx data that remains to be sent
void rxtxbuffer_tx_decrease_size(rxtxbuffer* pt, size_t sent_data_size);

// increase the ammount of the rx data that was received
void rxtxbuffer_rx_increase_size(rxtxbuffer* pt, size_t recved_data_size);

void* rxtxbuffer_tx_buf(rxtxbuffer* pt);

void* rxtxbuffer_rx_buf(rxtxbuffer* pt);

void rxtxbuffer_shift_tx_buf(rxtxbuffer* pt);




#endif // !__RXTXBUFFER_H__
