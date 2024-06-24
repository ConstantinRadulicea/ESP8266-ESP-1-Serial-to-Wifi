#include "rxtxbuffer.h"
#include <string.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

// Utility function to initialize the rxtxbuffer
int rxtxbuffer_init(rxtxbuffer* _Cvector, void* buf, size_t _recved_size, size_t _sent_size, size_t _capacity) {
	if (_Cvector == NULL) {
		return RXTXBUFFER_ERROR_INVALID_PARAMETERS;
	}

	if (buf == NULL && _capacity != (size_t)0) {
		return RXTXBUFFER_ERROR_INVALID_PARAMETERS;
	}

	if (_recved_size > _sent_size || _recved_size > _capacity)
	{
		return RXTXBUFFER_ERROR_INVALID_PARAMETERS;
	}

	_Cvector->data = buf;
	_Cvector->recved_size = _recved_size;
	_Cvector->sent_size = _sent_size;
	_Cvector->capacity = _capacity;

	return RXTXBUFFER_SUCCESS;
}

void* rxtxbuffer_data(rxtxbuffer* pt) {
	return pt->data;
}

size_t rxtxbuffer_sent_size(rxtxbuffer* pt) {
	return pt->sent_size;
}

size_t rxtxbuffer_recved_size(rxtxbuffer* pt) {
	return pt->recved_size;
}

size_t rxtxbuffer_capacity(rxtxbuffer* pt) {
	return pt->capacity;
}

size_t rxtxbuffer_rx_remaining(rxtxbuffer* sendbuf) {
	return sendbuf->capacity - sendbuf->recved_size;
}

size_t rxtxbuffer_tx_remaining(rxtxbuffer* sendbuf) {
	return sendbuf->recved_size - sendbuf->sent_size;
}

void rxtxbuffer_tx_decrease_size(rxtxbuffer* pt, size_t sent_data_size) {
	pt->sent_size = MIN(MIN(pt->recved_size, pt->sent_size + sent_data_size), pt->capacity);
}

void rxtxbuffer_rx_increase_size(rxtxbuffer* pt, size_t recved_data_size) {
	pt->recved_size = MIN(pt->recved_size + recved_data_size, pt->capacity);
}

void* rxtxbuffer_tx_buf(rxtxbuffer* pt) {
	return ((char*)(pt->data)) + pt->sent_size;
}

void* rxtxbuffer_rx_buf(rxtxbuffer* pt) {
	return ((char*)(pt->data)) + pt->recved_size;
}

void rxtxbuffer_shift_tx_buf(rxtxbuffer* pt) {
	memmove(rxtxbuffer_data(pt), rxtxbuffer_tx_buf(pt), rxtxbuffer_tx_remaining(pt));
	pt->recved_size = rxtxbuffer_tx_remaining(pt);
	pt->sent_size = (size_t)0;
}
