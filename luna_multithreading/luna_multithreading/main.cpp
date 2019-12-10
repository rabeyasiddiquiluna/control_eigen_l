#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <stdio.h>
#include <math.h>
#include <queue>



#include <condition_variable>
#include <mutex>


template <typename T>
class BlockingQueue {
public:
    BlockingQueue(){}
    BlockingQueue(const BlockingQueue& other) = delete; //not copyable
     void push(const T& item) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push(item);
        }
        cv_.notify_one();
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty()) {
            cv_.wait(lock);
        }
        auto result = queue_.front();
        queue_.pop();
        return result;
    }
    
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
   
};


// Loads the queue with Tasks, and marks the last Task as a sentinel
void source_thread_func(BlockingQueue<int>&bq, int num_ints)
{
    for (int i = 0; i <= num_ints; i++)
    {
        bq.push(i);
    }
}

// Processes all Tasks on the queue, breaking when it reaches the sentinel
void SinkThreadFunction( BlockingQueue<int> &bq, std::vector<int>&vec, int threshold,int n)
{
    
    for (int i = 0; i <= n; ++i)
    {
        int val = bq.pop();
        
        if (val>= threshold)
        {
            vec.emplace_back(val);
        }
        
    }
 
    
}


// If parameter is not true, test fails
// This check function would be provided by the test framework
#define IS_TRUE(x) { if (x.empty()) std::cout << __FUNCTION__ << " success on line " << __LINE__ << std::endl; }



std::vector<int> get_multiples(std::vector<int>&vec, int threshold)
{
    
    std::vector<int> vec_multiple;
    int threshold1 = abs(threshold);
    
    if (threshold1>0)
    {
    for (auto i = vec.begin(); i!= vec.end(); ++i)
    {
        if (*i % threshold1 == 0)
        vec_multiple.push_back(*i);
    
    }
    }
    
    return vec_multiple;
}

// Test for function1()
// You would need to write these even when using a framework
void test_function()
{
    std::vector<int> m;
    IS_TRUE(get_multiples(m,0));
    IS_TRUE(get_multiples(m,-5));
    
}


int main(int argc, char **argv)


{
    

    
    // End sum with these values should be 105
    int num_ints ;
    int threshold;
   
    
    
    
    std::cout << "Please enter num_ints "<<std::endl;
    std::cin >> num_ints;
    std::cout << "The value you entered is " << num_ints<<std::endl;
    std::cout << "Please enter threshold "<<std::endl;
    std::cin >> threshold;
    std::cout << "The value you entered is " << threshold<<std::endl;
    
    
    BlockingQueue<int> q;   // TODO
    std::vector<int> t;
    
    
    std::thread source_thread(source_thread_func,std::ref(q), num_ints);
    std::thread sinkthread(SinkThreadFunction,std::ref(q),std::ref(t), threshold,num_ints);
    source_thread.join();
    sinkthread.join();
    
    int sum = 0;
    test_function();
    
    std::vector<int> m = get_multiples(t, threshold);
    
    
    
    for (int i = 0; i < m.size(); i++)
    {
        sum += m[i];
    }
    std::cout << "Sum of multiples greater than or equal to threshold: " << sum << std::endl;
}


