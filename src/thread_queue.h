#pragma once

#include <windows.h>

#define QUEUE_LEN 1024

namespace ege {

class Lock
{
public:
	Lock(LPCRITICAL_SECTION p_) : _psection(p_) {
		::EnterCriticalSection(_psection);
	}
	~Lock() {
		::LeaveCriticalSection(_psection);
	}
private:
	LPCRITICAL_SECTION _psection;
};

template<typename T>
class thread_queue
{
public:
	thread_queue(void) {
		::InitializeCriticalSection(&_section);
		_r = _w = 0;
	}
	~thread_queue(void) {
		::DeleteCriticalSection(&_section);
	}

	void push(const T& d_) {
		Lock lock(&_section);
		int w = (_w + 1) % QUEUE_LEN;
		_queue[_w] = d_;
		if (w == _r)
			_r = (_r + 1) % QUEUE_LEN;
		_w = w;
	}
	int pop(T& d_) {
		Lock lock(&_section);
		if (_w == _r)
			return 0;
		d_ = _queue[_r];
		_last = d_;
		_r = (_r + 1) % QUEUE_LEN;
		return 1;
	}
	int unpop() {
		Lock lock(&_section);
		if (_r == (_w + 1) % QUEUE_LEN)
			return 0;
		_r = (_r + QUEUE_LEN - 1) % QUEUE_LEN;
		return 1;
	}
	T last() {
		return _last;
	}
	void process(void (*process_func)(T&)) {
		Lock lock(&_section);
		int r = _r;
		int w = _w;
		if (r != w) {
			if (w < r) w += QUEUE_LEN;
			for (; r <= w; r++) {
				int pos = r % QUEUE_LEN;
				process_func(_queue[pos]);
			}
		}
	}
	bool empty() {
		Lock lock(&_section);
		return _r == _w;
	}
private:
	CRITICAL_SECTION _section;
	T _queue[QUEUE_LEN];
	T _last;
	int _r, _w;
};

}
