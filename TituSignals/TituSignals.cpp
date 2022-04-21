#include <iostream>
#include "Signals.h"

void HelloWorld()
{
	std::cout << "Hello World" << std::endl;
}

class ClassA
{
public:

	ClassA() = default;
	virtual ~ClassA() = default;

	void SetInt(int m) { m_Int = m; m_OnValueChanged.Dispatch(m_Int, m_Float); }
	void SetFloat(float c) { m_Float = c; }

	void Print()
	{
		std::cout << m_Int << "  " << m_Float << std::endl;
	}

	void Print(int i, int j, float k) const
	{
		std::cout << "PrintParams: " << i << " " << j << "  " << k << " ---- Instance Values: " << m_Int << " " << m_Float << std::endl;
	}

	void PrintWithContext(int i, int j, float k, void* Context)
	{
		float* reinterpretedContext = (float*)(Context); //retreive context
		std::cout << "PrintParams: " << i << " " << j << "  " << k << " ---- Instance Values: " << m_Int << " " << m_Float << "------ Context: " << *reinterpretedContext << std::endl;
	}

	static void PrintParamsStatic()
	{
		std::cout << "Static Call" << std::endl;
	}

private:
	int m_Int = 2;
	float m_Float = 3;

public:
	Signal<int, float> m_OnValueChanged;
};

int main()
{
	//Empty Signals
	ClassA classA;
	uint32_t lambdaHandler = classA.m_OnValueChanged.AddListener(([&](int a, float b) -> void {std::cout << "Lambda Example " << a << "  " << b << std::endl; }));

	Signal<> jSignal;
	uint32_t emptySignalHandler = jSignal.AddListener(&classA, &ClassA::Print); //no method parameters example
	uint32_t staticHandler = jSignal.AddListener(&ClassA::PrintParamsStatic); //static method example
	jSignal.AddListener(HelloWorld); //global function example
	jSignal.Dispatch(); //dispatch with no parameters

	//Signals with parameters
	Signal<int, int, float> jSignal2;
	uint32_t parameterSignalHandler = jSignal2.AddListener(&classA, &ClassA::Print); //method parameters example
	jSignal2.Dispatch(1, 2, 3.4f); //parameters dispacth
	classA.SetInt(12345);//modify Instance
	jSignal2.Dispatch(1, 2, 3.4f); //see changes on instance

	float Context = 4.5f;
	uint32_t parameterSignalHandlerContext = jSignal2.AddListener(&classA, &ClassA::PrintWithContext, (void*)(&Context)); //Stored context to retreive it later regardless of what Signal is Dispatching 
	jSignal2.Dispatch(1, 2, 3.0f); //Example how Context will be reitreived independently of Signal parameters

	auto handler = jSignal2.GetListener(parameterSignalHandler); //how to retrieve function pointer whit handler

	//remove listeners
	jSignal.RemoveListener(emptySignalHandler);
	jSignal.RemoveListener(staticHandler);
	classA.m_OnValueChanged.RemoveListener(lambdaHandler);
	jSignal2.RemoveListener(parameterSignalHandler);
	jSignal2.RemoveListener(parameterSignalHandlerContext);

	return 0;
}
