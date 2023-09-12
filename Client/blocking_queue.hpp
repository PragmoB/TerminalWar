#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

#include "blocking_queue.h"

template <typename T>
void BlockingQueue<T>::Enqueue(const T& item) {
	std::unique_lock<std::mutex> lock(mutex_);
	// 큐가 가득 찼으면 대기
	not_full_.wait(lock, [this] { return queue_.size() < max_size_; });
	queue_.push(item);
	not_empty_.notify_one();
}

template <typename T>
T BlockingQueue<T>::Dequeue() {
	std::unique_lock<std::mutex> lock(mutex_);
	// 큐가 비어있으면 대기
	not_empty_.wait(lock, [this] { return !queue_.empty(); });
	T item = queue_.front();
	queue_.pop();
	not_full_.notify_one();
	return item;
}

#endif