#include "timewheel.h"
#include "channel.h"
#include "eventloop.h"
TimeTask::TimeTask(uint64_t id, uint32_t timeout, TaskFunc cb)
    : id_(id), timeout_(timeout), taskcb_(cb), canceled_(false)
{
}

TimeTask::~TimeTask()
{
    if (!canceled_)
    {
        taskcb_();
    }

    releasecb_();
}

void TimeTask::cancelTask()
{
    canceled_ = true;
}

void TimeTask::setRelesaeCb(ReleseFunc cb)
{
    releasecb_ = cb;
}

uint32_t TimeTask::getTimeout()
{
    return timeout_;
}
// 时间轮
// 构造
TimeWheel::TimeWheel(EventLoop *loop)
    : tick_(0), capacity_(60), wheel_(capacity_), timefd_(CreatTimeFd()), timefd_channel_(new Channel(loop, timefd_)), loop_(loop)
{
    timefd_channel_->SetReadCallBack(std::bind(&TimeWheel::Ontime, this));
    timefd_channel_->EnAbleRead(); // 启动读监控
}

int TimeWheel::ReadTimeFd()
{
    uint64_t ret = 0;
    int n = read(timefd_, &ret, sizeof ret);
    if (n < 0)
    {
        if (errno == EINTR)
            return -1;

        LOG_FATAL("timefd read error");
    }

    return ret;
}

void TimeWheel::Ontime()
{
    int n = ReadTimeFd();

    for (int i = 0; i < n; i++)
    {
        runTimerTask();
    }
}

TimeWheel::~TimeWheel()
{
}

void TimeWheel::timeAdd(uint64_t id, uint32_t timeout, const TaskFunc cb)
{
    // 添加定时任务也得放在eventloop中完成
    loop_->RunInLoop(std::bind(&TimeWheel::timeAddInloop, this, id, timeout, cb));
}
void TimeWheel::timeAddInloop(uint64_t id, uint32_t timeout, const TaskFunc cb)
{
    taskPtr pt(new TimeTask(id, timeout, cb));
    // 设置任务的释放回调
    pt->setRelesaeCb(std::bind(&TimeWheel::timerRemove, this, id));

    // 根据 timeout 计算任务应该放入时间轮的位置
    int pos = (tick_ + timeout) % capacity_;
    wheel_[pos].push_back(pt);

    // 将任务id和任务的weak_ptr放入timers_
    timers_[id] = weakTask(pt);
}

void TimeWheel::reflashTimer(uint64_t id)
{
    loop_->RunInLoop(std::bind(&TimeWheel::reflashTimerInLoop, this, id));
    return;
}
void TimeWheel::reflashTimerInLoop(uint64_t id)
{

    auto it = timers_.find(id);
    if (it == timers_.end())
    {
        // 没有找到任务
        return;
    }

    // 找到了任务，刷新任务
    auto delayTask = it->second.lock();
    int pos = (tick_ + delayTask->getTimeout()) % capacity_;
    wheel_[pos].push_back(delayTask);
    return;
}

void TimeWheel::runTimerTask()
{
    tick_ = (tick_ + 1) % capacity_;
    wheel_[tick_].clear();
}

void TimeWheel::timeCancel(uint64_t id)
{
    loop_->RunInLoop(std::bind(&TimeWheel::timeCancelInLoop, this, id));
}
void TimeWheel::timeCancelInLoop(uint64_t id)
{
    auto it = timers_.find(id);
    if (it == timers_.end())
    {
        // 没有找到任务
        return;
    }

    auto pt = it->second.lock();
    if (pt)
    {
        pt->cancelTask();
    }

    return;
}

bool TimeWheel::timerRemove(uint64_t id)
{
    auto it = timers_.find(id);
    if (it == timers_.end())
    {
        // 没有找到任务
        return false;
    }

    timers_.erase(id);
    return true;
}

int TimeWheel::CreatTimeFd()
{

    int fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (fd < 0)
    {
        LOG_FATAL("time fd error");
    }
    LOG_INFO("time fd create success fd:%d", fd);

    struct itimerspec new_value;
    new_value.it_value.tv_sec = 1;
    new_value.it_value.tv_nsec = 0;
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;
    timerfd_settime(fd, 0, &new_value, nullptr);

    return fd;
}
