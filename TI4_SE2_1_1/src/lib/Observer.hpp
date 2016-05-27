#ifndef HAW_LIB_OBSERVER_H
#define HAW_LIB_OBSERVER_H

#include <vector>
#include <algorithm>

namespace lib
{
	template<typename T>
	class Observer
	{
		public:
			virtual ~Observer( ) { }
			virtual void update(const T&) = 0;
	};

	template<typename T>
	class Observable
	{
		typedef std::vector<Observer<T> *> vec_t;
		typedef vec_t::iterator iter_t;

		public:
			void attach(Observer<T>&);
			void detach(Observer<T>&);
		protected:
			void notify(const T&);
		private:
			vec_t clients_;
	};

	template<typename T>
	void Observable<T>::attach(Observer<T>& o)
	{
		clients_.push_back(&o);
	}
	
	template<typename T>
	void Observable<T>::detach(Observer<T>& o)
	{
		iter_t i = std::find(clients_.begin(), clients_.end(), &o);

		if(i != clients_.end())
			clients_.erase(i);
	}
	
	template<typename T>
	void Observable<T>::notify(const T& e)
	{
		for(iter_t i1 = clients_.begin(), i2 = clients_.end() ; i1 != i2 ; ++i1)
		{
			(*i1)->update(e);
		}
	}
}

#endif

