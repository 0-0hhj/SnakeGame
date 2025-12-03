#pragma once

// 简单的单例基类（模板）
// 用法：让某个类继承自 Instance<Derived>，然后通过 Derived::getInstance() 获取单例
// 注意：被继承类必须有公共的默认构造函数

template <typename U>
class Instance
{
public:
    // 返回单例引用，使用局部静态指针保证初始化线程安全且对象在程序结束前存活
    static U& getInstance()
    {
        static U* instance = new U();
        return *instance;
    }

protected:
    Instance() = default;
    ~Instance() = default;

private:
	// Prevent copying and moving
    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;
    Instance(Instance&&) = delete;
    Instance& operator=(Instance&&) = delete;
};