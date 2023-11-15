// Callback.h
//
// A callback template class
#ifndef CALLBACK_ONCE
#define CALLBACK_ONCE

namespace s2Fwk
{

template < typename ClassType, typename ReturnType, typename Parameter >
class Callback
{
public:
	typedef ReturnType (ClassType::*Method)(Parameter);

private:
	ClassType	*obj;
	Method		m;

public:
	Callback(ClassType *objectInstance, Method method)
	{
		obj = objectInstance;
		m	  = method;
	};

	ReturnType operator()(Parameter p)
	{
		return (obj->*m)(p);
	};

	ReturnType call(Parameter p)
	{
		return operator()(p);
	};
};

//template <class TSlot>
//class SignalVoid : public Callback <TSlot, void, void>
//{
//private:
//public:
//	SignalVoid(TSlot *slot, void(TSlot::*m)()) : Callback<TSlot,void,void>(slot,m) {}
//};


template <typename SlotType, typename SignalType>
class Signal : public Callback <SlotType, void, SignalType>
{
private:
public:
	Signal(SlotType *slot, void(SlotType::*m)(SignalType)) : Callback<SlotType,void,SignalType>(slot,m) {}
};


}
#endif
