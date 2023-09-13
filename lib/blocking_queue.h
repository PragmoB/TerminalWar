#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue {
public:
	BlockingQueue() : max_size_(50) {}
	BlockingQueue(int max_size) : max_size_(max_size) {}
	void Enqueue(const T& item);
	T Dequeue();

private:
	std::queue<T> queue_;
	int max_size_;
	std::mutex mutex_;
	std::condition_variable not_empty_;
	std::condition_variable not_full_;
};

#include "blocking_queue.hpp"

#endif