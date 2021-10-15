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
	int lambdaHandler = classA.m_OnValueChanged.AddListener(([&](int a, float b) -> void {std::cout << "Lambda Example " << a << "  " << b << std::endl; }));

	Signal<> jSignal;
	int emptySignalHandler = jSignal.AddListener(&classA, &ClassA::Print); //no method parameters example
	int staticHandler = jSignal.AddListener(&ClassA::PrintParamsStatic); //static method example
	jSignal.AddListener(HelloWorld); //global function example
	jSignal.Dispatch(); //dispatch with no parameters

	//Signals with parameters
	Signal<int, int, float> jSignal2;
	int parameterSignalHandler = jSignal2.AddListener(&classA, &ClassA::Print); //method parameters example
	jSignal2.Dispatch(1, 2, 3.4f); //parameters dispacth
	classA.SetInt(12345);//modify Instance
	jSignal2.Dispatch(1, 2, 3.4f); //se changes on instance

	auto handler = jSignal2.GetListener(parameterSignalHandler); //how to retrieve function pointer whit handler

	//remove listeners
	jSignal.RemoveListener(emptySignalHandler);
	jSignal.RemoveListener(staticHandler);
	classA.m_OnValueChanged.RemoveListener(lambdaHandler);
	jSignal2.RemoveListener(parameterSignalHandler);

	return 0;
}
