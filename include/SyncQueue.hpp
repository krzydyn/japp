#include <atomic>
#include <queue>
#include <mutex>
#include<condition_variable>

class SyncQueueError : public std::runtime_error {
public:
	SyncQueueError() : std::runtime_error("The SyncQueue has been stopped.") {}
};

template<typename T>
class SyncQueue {
private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cond;
	bool running = true;
public:
	SyncQueue(const SyncQueue& other) = delete;
    SyncQueue(SyncQueue&& other) = delete;
    SyncQueue& operator=(const SyncQueue& other) = delete;
    SyncQueue& operator=(SyncQueue&& other) = delete;
    virtual ~SyncQueue() = default;

	SyncQueue(){}

	void stop() {
		running = false;
		cond.notify_all();
	}

	bool empty() {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.empty();
	}

	void enqueue(T& o) {
		std::unique_lock<std::mutex> lock(mutex);
		if (!running) throw SyncQueueError();
		queue.push(o);
		lock.unlock(); // unlock before notify
		cond.notify_one();
	}

	T dequeue() {
		std::unique_lock<std::mutex> lock(mutex);
		while (queue.empty() && running) {
			cond.wait(lock);
		}
		if (queue.empty()) {
			// Check for queue empty here, as running could
			// have been changed after we returned but we could still
			// return useful data.
			throw SyncQueueError();
		}
		auto o = std::move(queue.front());
		queue.pop();
		return o;
	}
};
