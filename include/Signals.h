//Code made by Xavier Blay. https://github.com/BlayXavi

#pragma once
#include <map>
#include <functional>

//USAGE:
//Signal signal;
//signal.AddListener(HelloWorld); //To Add Global/Static Function
//int i = signal.AddListener(&classAInstance, &ClassA::DoSomething); // To add member instance method
template<class... Args>
class Signal
{
public:

	typedef std::function<void(Args...)> Listener;
	typedef std::function<void(Args..., void*)> ListenerContext;

	virtual ~Signal() { m_Listeners.clear(); }

	//add member method as listener. Store returned integer (handler) to remove the same listener
	uint32_t AddListener(const Listener& func)
	{
		m_Listeners.insert(std::make_pair(++m_HandlerCounter, func));
		return m_HandlerCounter;
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(T* inst, void (T::* func)(Args...))
	{
		return AddListener([=](Args...args) -> void
			{
				(inst->*func)(args...);
			});
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(T* inst, void (T::* func)(Args...) const)
	{
		return AddListener([=](Args...args) -> void
			{
				(inst->*func)(args...);
			});
	}

	uint32_t AddListener(const ListenerContext& func, void* Context)
	{
		m_ListenersContext.insert(std::make_pair(++m_HandlerCounter, std::make_pair(func, Context)));
		return m_HandlerCounter;
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(T* inst, void (T::* func)(Args..., void*), void* Context)
	{
		return AddListener([=](Args...args, void* C) -> void
			{
				(inst->*func)(args..., C);
			}, Context);
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(T* inst, void (T::* func)(Args..., void*) const, void* Context)
	{
		return AddListener([=](Args...args, void* C) -> void
			{
				(inst->*func)(args..., C);
			}, Context);
	}

	void RemoveListener(const int& i)
	{
		m_Listeners.erase(i);
		m_ListenersContext.erase(i);
	}

	void RemoveAllListeners()
	{
		m_Listeners.clear();
		m_ListenersContext.clear();
	}

	void Dispatch(const Args& ... args)
	{
		for (auto const& it : m_Listeners) {
			it.second(std::forward<decltype(args)>(args)...);
		}

		for (auto const& it : m_ListenersContext) {
			it.second.first(std::forward<decltype(args)>(args)..., it.second.second);
		}
	}

	Listener GetListener(const int& i)
	{
		return m_Listeners[i];
	}

	ListenerContext GetListenerContext(const int& i)
	{
		return m_ListenersContext[i];
	}

private:
	std::map<int, Listener> m_Listeners;
	std::map<uint32_t, std::pair<ListenerContext, void*>> m_ListenersContext;

	uint32_t m_HandlerCounter = 0;
};
