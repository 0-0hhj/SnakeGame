#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <utility>
#include <stdexcept>
#include "Instance.h"

// 通用对象池（基于 shared_ptr 的实现）
// 说明：池内部为每个 slot 分配一个堆对象（std::unique_ptr<T>），
// acquireShared() 返回一个 std::shared_ptr<T>，其自定义 deleter 会在最后一个引用释放时
// 将 slot 标记为空闲并重置对象状态（通过赋值默认构造的 T）。
// 通过继承 Instance<ObjPool<T>>，每个元素类型 T 对应一个全局单例池：
// 使用方式： auto p = ObjPool<MyType>::getInstance().acquireShared();

template<typename T>
class ObjPool : public Instance<ObjPool<T>>
{
public:

	// 构造函数，预分配若干对象
    ObjPool() {
        reserve(preallocate);
        for (size_t i = 0; i < preallocate; ++i) {
            pool_.push_back(std::make_unique<T>());
            inUse_.push_back(false);
			//放入空闲列表，逆序以便后续 pop_back 时按顺序分配
            freeList_.push_back(static_cast<int>(preallocate - 1 - i));
        }
    }

	// 默认析构函数
    ~ObjPool() = default;

    // 预留容量
    void reserve(size_t n) {
        std::lock_guard<std::mutex> lk(mutex_);
        pool_.reserve(n);
        inUse_.reserve(n);
        freeList_.reserve(n);
    }

    // 获取一个 shared_ptr 句柄；如果池为空则会新建一个对象
    std::shared_ptr<T> acquireShared() {
        std::lock_guard<std::mutex> lk(mutex_);
        int id = -1;
        if (!freeList_.empty()) {
            id = freeList_.back();
            freeList_.pop_back();
            inUse_[id] = true;
        } else {
            id = static_cast<int>(pool_.size());
            pool_.push_back(std::make_unique<T>());
            inUse_.push_back(true);
        }
        T* raw = pool_[id].get();
        // 自定义 deleter：不删除对象，只把 slot 标记为空闲并重置内容
        // 捕获 this（单例实例）是安全的，只要池实例在所有 shared_ptr 被释放之前存活
        auto deleter = [this, id](T*) {
            std::lock_guard<std::mutex> lk(mutex_);
            if (id < 0 || id >= static_cast<int>(pool_.size())) return;
            if (!inUse_[id]) return;

            // 重置对象到默认状态（要求 T 可赋值）
            *pool_[id] = T();
            inUse_[id] = false;
            freeList_.push_back(id);
        };
        // 注意：shared_ptr 的 control block 与 pool 中的 unique_ptr 无关联，
        // deleter 负责将 slot 返回池中，池继续保有对象的实际所有权（unique_ptr）
        return std::shared_ptr<T>(raw, deleter);
    }

    // 使用样例对象初始化并返回 shared_ptr
    std::shared_ptr<T> acquireShared(const T& sample) {
        std::lock_guard<std::mutex> lk(mutex_);
        int id = -1;
        if (!freeList_.empty()) {
            id = freeList_.back();
            freeList_.pop_back();
            *pool_[id] = sample;
            inUse_[id] = true;
        } else {
            id = static_cast<int>(pool_.size());
            pool_.push_back(std::make_unique<T>(sample));
            inUse_.push_back(true);
        }
        T* raw = pool_[id].get();
        auto deleter = [this, id](T*) {
            std::lock_guard<std::mutex> lk(mutex_);
            if (id < 0 || id >= static_cast<int>(pool_.size())) return;
            if (!inUse_[id]) return;
            *pool_[id] = T();
            inUse_[id] = false;
            freeList_.push_back(id);
        };
        return std::shared_ptr<T>(raw, deleter);
    }

    // 池容量（包含已用与空闲）
    size_t capacity() const noexcept {
        std::lock_guard<std::mutex> lk(mutex_);
        return pool_.size();
    }

    // 可用（空闲）对象个数
    size_t available() const noexcept {
        std::lock_guard<std::mutex> lk(mutex_);
        return freeList_.size();
    }

    // 清空池（注意：调用时应确保没有活跃的 shared_ptr）
    void clear() {
        std::lock_guard<std::mutex> lk(mutex_);
        pool_.clear();
        freeList_.clear();
        inUse_.clear();
    }

private:
	// 互斥锁保护线程安全
    mutable std::mutex mutex_;

    // 使用vector进行存储，扩容时，std::unique_ptr<T>会触发移动，int、char触发拷贝，因此不会丢失信息

	// 预分配数量
    const size_t preallocate = 10;
	// 对象池存储
    std::vector<std::unique_ptr<T>> pool_;
	// 空闲列表，存储空闲 slot 的索引
    std::vector<int> freeList_;
	// 使用状态标记
    std::vector<char> inUse_;
}; 