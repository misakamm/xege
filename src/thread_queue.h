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
		_begin = _end = 0;
	}
	~thread_queue(void) {
		::DeleteCriticalSection(&_section);
	}

	void push(const T& d_) {
		Lock lock(&_section);
		int w = (_end + 1) % QUEUE_LEN;
		_queue[_end] = d_;
		if (w == _begin)
			_begin = (_begin + 1) % QUEUE_LEN;
		_end = w;
	}
	int pop(T& d_) {
		Lock lock(&_section);
		if (_end == _begin)
			return 0;
		d_ = _queue[_begin];
		_last = d_;
		_begin = (_begin + 1) % QUEUE_LEN;
		return 1;
	}
	int unpop() {
		Lock lock(&_section);
		if (_begin == (_end + 1) % QUEUE_LEN)
			return 0;
		_begin = (_begin + QUEUE_LEN - 1) % QUEUE_LEN;
		return 1;
	}
	T last() {
		return _last;
	}
	void process(void (*process_func)(T&)) {
		Lock lock(&_section);
		int r = _begin;
		int w = _end;
		if (r != w) {
			if (w < r) w += QUEUE_LEN;
			for (; r < w; r++) {
				int pos = r % QUEUE_LEN;
				process_func(_queue[pos]);
			}
		}
	}
	bool empty() {
		Lock lock(&_section);
		return _begin == _end;
	}
private:
	CRITICAL_SECTION _section;
	T _queue[QUEUE_LEN];
	T _last;
	int _begin, _end;
};

}
