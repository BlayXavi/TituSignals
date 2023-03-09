#pragma once
#include <map>
#include <functional>

//USAGE:
//Signal signal;
//signal.AddListener(&Class::DoSomething); //To Add Global/Static Function
//int i = signal.AddListener(&classAInstance, &ClassA::DoSomething); // To add member instance method
template<class... Args>
class Signal
{
public:

	typedef std::function<void(Args...)> Listener;
	typedef std::function<void(Args..., uint8*)> ListenerContext;

	virtual ~Signal() { m_Listeners.clear(); }

	//add member method as listener. Store returned integer (handler) to remove the same listener
	uint32_t AddListener(const Listener& func)
	{
		m_Listeners.insert(std::make_pair(++m_HandlerCounter, func));
		return m_HandlerCounter;
	}

	uint32_t AddListenerOnce(const Listener& func)
	{
		m_ListenersOnce.insert(std::make_pair(++m_HandlerCounter, func));
		return m_HandlerCounter;
	}

	uint32_t AddListener(const ListenerContext& func, uint8* Context)
	{
		m_ListenersContext.insert(std::make_pair(++m_HandlerCounter, std::make_pair(func, Context)));
		return m_HandlerCounter;
	}

	uint32_t AddListenerOnce(const ListenerContext& func, uint8* Context)
	{
		m_ListenersContextOnce.insert(std::make_pair(++m_HandlerCounter, std::make_pair(func, Context)));
		return m_HandlerCounter;
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(const T* inst, void (T::* func)(Args...))
	{
		return AddListener([=](Args...args) -> void
			{
				(const_cast<T*>(inst)->*func)(args...);
			});
	}

	template<typename T>
	uint32_t AddListenerOnce(const T* inst, void (T::* func)(Args...))
	{
		return AddListenerOnce([=](Args...args) -> void
			{
				(const_cast<T*>(inst)->*func)(args...);
			});
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(const T* inst, void (T::* func)(Args...) const)
	{
		return AddListener([=](Args...args) -> void
			{
				(const_cast<T*>(inst)->*func)(args...);
			});
	}

	template<typename T>
	uint32_t AddListenerOnce(const T* inst, void (T::* func)(Args...) const)
	{
		return AddListenerOnce([=](Args...args) -> void
			{
				(const_cast<T*>(inst)->*func)(args...);
			});
	}

	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(const T* inst, void (T::* func)(Args..., uint8*), uint8* Context)
	{
		return AddListener([=](Args...args, uint8* C) -> void
			{
				(const_cast<T*>(inst)->*func)(args..., C);
			}, Context);
	}

	template<typename T>
	uint32_t AddListenerOnce(const T* inst, void (T::* func)(Args..., uint8*), uint8* Context)
	{
		return AddListenerOnce([=](Args...args, uint8* C) -> void
			{
				(const_cast<T*>(inst)->*func)(args..., C);
			}, Context);
	}

	
	//add member method as listener. Store returned integer (handler) to remove the same listener
	template<typename T>
	uint32_t AddListener(const T* inst, void (T::* func)(Args..., uint8*) const, uint8* Context)
	{
		return AddListener([=](Args...args, uint8* C) -> void
			{
				(const_cast<T*>(inst)->*func)(args..., C);
			}, Context);
	}

	template<typename T>
	uint32_t AddListenerOnce(const T* inst, void (T::* func)(Args..., uint8*) const, uint8* Context)
	{
		return AddListenerOnce([=](Args...args, uint8* C) -> void
			{
				(const_cast<T*>(inst)->*func)(args..., C);
			}, Context);
	}
	
	void RemoveListener(const int& i)
	{
		m_Listeners.erase(i);
		m_ListenersContext.erase(i);

		m_ListenersOnce.erase(i);
		m_ListenersContextOnce.erase(i);
	}

	void RemoveAllListeners()
	{
		m_Listeners.clear();
		m_ListenersContext.clear();

		m_ListenersOnce.clear();
		m_ListenersContextOnce.clear();
	}

	void RemoveListeners()
	{
		RemoveAllListeners();
	}

	void Dispatch(const Args& ... args)
	{
		for (auto const& it : m_Listeners) {
			it.second(std::forward<decltype(args)>(args)...);
		}

		for (auto const& it : m_ListenersContext){
			it.second.first(std::forward<decltype(args)>(args)..., it.second.second);	
		}

		for (auto const& it : m_ListenersOnce) {
			it.second(std::forward<decltype(args)>(args)...);
		}
		m_ListenersOnce.clear();

		for (auto const& it : m_ListenersContextOnce){
			it.second.first(std::forward<decltype(args)>(args)..., it.second.second);	
		}
		m_ListenersContextOnce.clear();
	}

	Listener GetListener(const int& i)
	{
		return m_Listeners[i];
	}

private:
	std::map<uint32_t, Listener> m_Listeners;
	std::map<uint32_t, Listener> m_ListenersOnce;
	std::map<uint32_t, std::pair<ListenerContext, uint8_t*>> m_ListenersContext;
	std::map<uint32_t, std::pair<ListenerContext, uint8_t*>> m_ListenersContextOnce;
	
	uint32_t m_HandlerCounter = 0;
};