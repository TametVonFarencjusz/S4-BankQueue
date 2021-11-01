#include <exception>

struct EmptyQueueException : public std::exception{
	const char* what () const throw ()
	{
		return "Empty Queue";
	}
};
