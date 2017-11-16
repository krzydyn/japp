#ifndef __THREADPOOL_HPP
#define __THREADPOOL_HPP

#include <thread>
#include <functional>
#include <SyncQueue.hpp>
#include <util/ArrayList.hpp>

class ThreadPool {
	public:
	ThreadPool(const ThreadPool& other) = delete;
	ThreadPool(ThreadPool&& other) = delete;
	ThreadPool& operator=(const ThreadPool& other) = delete;
	ThreadPool& operator=(ThreadPool&& other) = delete;
	~ThreadPool() = default;

	ThreadPool(int threads=0){
		if (threads > 0) maxThreads = threads;
		else maxThreads = std::thread::hardware_concurrency();
	}

	void enqueue(std::function<void()> f) {
		queue.enqueue(f);
	}
	private:
	void workerLoop() {
		while (true) {
			std::function<void()> work = queue.dequeue();
			work(); // function<void()> type
		}
	};

	private:
	int maxThreads;
	SyncQueue<std::function<void()>> queue;
	ArrayList<std::thread*> workers;
};

#endif
