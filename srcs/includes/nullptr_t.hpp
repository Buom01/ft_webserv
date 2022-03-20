#ifndef __FT_NULLPTR_T
# define __FT_NULLPTR_T

const class nullptr_t
{
	public:
		template<class T>
		operator T*() const { return 0; }

		template<class C, class T>
		operator T C::*() const { return 0; }   
	private:
		void operator&() const;
} nullptr_t = {};

#endif
