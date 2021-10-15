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

	virtual ~Signal() { m_Listeners.clear(); }

	//add member method as listener. Store returned integer (handler) to remove the same listener
	int AddListener(const Listener& func)
	{
		m_Listeners.insert(std::make_pair(++m_HandlerCounter, func));
		return m_HandlerCounter;
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	int AddListener(T* inst, void (T::* func)(Args...))
	{
		return AddListener([=](Args...args) -> void
			{
				(inst->*func)(args...);
			});
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	int AddListener(T* inst, void (T::* func)(Args...) const)
	{
		return AddListener([=](Args...args) -> void
			{
				(inst->*func)(args...);
			});
	}

	void RemoveListener(const int& i)
	{
		m_Listeners.erase(i);
	}

	void Dispatch(const Args& ... args)
	{
		for (auto const& it : m_Listeners) {
			it.second(std::forward<decltype(args)>(args)...);
		}
	}

	Listener GetListener(const int& i)
	{
		return m_Listeners[i];
	}

private:
	std::map<int, Listener> m_Listeners;
	uint32_t m_HandlerCounter = 0;
};
