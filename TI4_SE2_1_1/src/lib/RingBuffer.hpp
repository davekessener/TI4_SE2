#ifndef HAW_LIB_RINGBUFFER_H
#define HAW_LIB_RINGBUFFER_H

#include <Array.hpp>
#include <concurrent/Mutex.h>
#include <concurrent/Semaphore.h>

namespace lib
{
	namespace RingBufferConcurrency
	{
		template<typename T>
		class SingleThreaded
		{
			public:
				std::size_t size( ) const { return size_; }

				struct FillLock { FillLock(SingleThreaded<T> *) { ++size_; } };
				struct EmptyLock { EmptyLock(SingleThreaded<T> *) { --size_; } };
			private:
				std::size_t size_;
		};

		template<typename T>
		class MultiThreaded
		{
			public:
				MultiThreaded( ) : fill_(T::capacity), empty_(0) { }
				std::size_t size( ) const { return T::capacity - fill_.get(); }

				class FillLock
				{
					public:
						FillLock(MultiThreaded<T> *t) : t_(t)
						{
							t_->fill_.down();
							t_->mtx_.lock();
						}
						~FillLock(void)
						{
							t_->empty_.up();
							t_->mtx_.unlock();
						}
					private:
						MultiThreaded<T> *t_;
				};
				class EmptyLock
				{
					public:
						EmptyLock(MultiThreaded<T> *t) : t_(t)
						{
							t_->empty_.down();
							t_->mtx_.lock();
						}
						~EmptyLock(void)
						{
							t_->fill_.up();
							t_->mtx_.unlock();
						}
					private:
						MultiThreaded<T> *t_;
				};
			private:
				Mutex mtx_;
				Semaphore fill_, empty_;
		};
	}

	template
	<
		typename T, 
		std::size_t N,
		template <class> class ThreadingPolicy = RingBufferConcurrency::SingleThreaded
	>
	class RingBuffer : public ThreadingPolicy<RingBuffer<T, N, ThreadingPolicy> >
	{
		typedef ThreadingPolicy<RingBuffer<T, N, ThreadingPolicy> > Super;
		typedef Array<T, N> arr_t;
		typedef typename arr_t::size_type idx_t;

		typedef typename Super::FillLock FillLock;
		typedef typename Super::EmptyLock EmptyLock;

		public:
			static const std::size_t capacity = N;

		public:
			RingBuffer( ) : iRead_(0), iWrite_(0) { }
			T& front( ) { if(empty()) throw std::out_of_range("RingBuffer::front"); return data_[iRead_]; }
			const T& front( ) const { if(empty()) throw std::out_of_range("RingBuffer::front const"); return data_[iRead_]; }
			void enqueue(const T&);
			T dequeue( );
			bool empty( ) const { return size() == 0; }
			size_t max_size( ) const { return N; }

			using Super::size;
		private:
			arr_t data_;
			idx_t iRead_, iWrite_;
	};

	template<typename T, std::size_t N, template <class> class P>
	void RingBuffer<T, N, P>::enqueue(const T& e)
	{
		FillLock lock(this);

		if(size() >= max_size())
			throw std::out_of_range("RingBuffer:enqueue");
		
		data_[iWrite_] = e; 
		iWrite_ = (iWrite_ + 1) % N;
	}

	template<typename T, std::size_t N, template <class> class P>
	T RingBuffer<T, N, P>::dequeue(void)
	{
		EmptyLock lock(this);

		if(empty())
			throw std::out_of_range("RingBuffer:dequeue");

		T *t = &data_[iRead_];
		iRead_ = (iRead_ + 1) % N;
		return *t;
	}
}

#endif

