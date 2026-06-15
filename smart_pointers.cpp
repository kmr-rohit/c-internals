// We will solve HFT Interview Question 
// Implement your own unique_ptr in c++ 


// to free up allocated heap memory , earlier it was responsibilty of programer, but sometime deleting can be missed or done twice which is a undefined behaviour
// so c++ introduced 3 smart pointer in c++11 , std::unique_ptr , std::shared_ptr , std::weak_ptr 


// From cppref 
// std::unique_ptr - is a smart pointer that owns and manages another object through a pointer and disposes of that object when the unqiue_ptr goes out of scope

// it goes oos when : a) the managing unique_ptr object is destroyed b) the managing unique_ptr is assigned another pointer via operator = or reset()

// int main(){
// 	int* i = new int{5};
// 	std::unique_ptr<int> uptr{i};

// 	 // unique_ptr is move only function 
// 	// std::unique_ptr<int> uptr2 = uptr; // compile error
// 	std::unique_ptr<int> uptr2 = std::move(uptr); // we have to use move function

// }


// Implementation of uniqu_ptr 

#include<iostream>
using namespace std;

/*
==============================================================================
 UniquePtr<T> — a from-scratch std::unique_ptr
==============================================================================
 Mental model: a "box" that holds EXACTLY ONE raw pointer and promises to
 `delete` it when the box is destroyed.

        UniquePtr<A> uptr
        +----------------+
        |  _ptr  --------+----->  [ A object on heap ]
        +----------------+

 MEMBER
   T* _ptr;            The single owned resource. Empty box == nullptr.

 CONSTRUCTORS
   UniquePtr()                 -> empty box (_ptr = nullptr)
   explicit UniquePtr(T* ptr)  -> wrap a raw pointer and take ownership.
                                  `explicit` blocks accidental silent grabs
                                  like `UniquePtr<A> u = somePointer;`

 COPYING IS DELETED (= delete)
   Two boxes pointing at the same object would both delete it -> double free.
   So copy ctor and copy-assignment are forbidden at compile time.
   This is the entire meaning of "unique": at most one owner.

       FORBIDDEN:  u1._ptr --->[obj]<--- u2._ptr   // both delete -> crash

 MOVING IS ALLOWED (UniquePtr&&)
   Ownership TRANSFERS. release() nulls the source; the destination adopts.
       before:  src --->[obj]    dst ---> null
       after:   src ---> null    dst --->[obj]
   The templated move ctor/assignment allow UniquePtr<Derived> -> UniquePtr<Base>.

 ACCESSORS (make it feel like a real pointer)
   get()        -> hand out raw pointer, no ownership change
   operator->   -> uptr->member
   operator*    -> *uptr

 release() vs reset()  (the key pair)
   release(): "give up ownership, RETURN the pointer, delete NOTHING."
       before: box --->[obj]   after: box ---> null, caller now owns [obj]

   reset(newPtr): "drop what I hold, then adopt newPtr."
       Correct order is: remember old -> adopt new -> delete old.
       NOTE: the reset() below sets `_ptr = nullptr` instead of `_ptr = newPtr`,
       so the pointer passed to reset() is silently dropped (leaked) and its
       destructor never runs. That is why `uptr.reset(a2)` leaks object 2.

 DESTRUCTOR
   ~UniquePtr() calls reset() -> deletes whatever it holds. This automatic
   cleanup on scope-exit is the whole reason smart pointers exist.

 HELPERS
   swap()          -> exchange the two raw pointers
   operator bool   -> `if (uptr)` tests whether the box is non-empty.
                      `explicit` allows `if(uptr)` but blocks `int x = uptr;`
==============================================================================
*/
template<class T>
class UniquePtr{
private:
	T* _ptr = nullptr;

public:
	UniquePtr() : _ptr(nullptr) {}

	explicit UniquePtr(T* ptr = nullptr) : _ptr(ptr){
	}

	UniquePtr(const UniquePtr& other) = delete;


	UniquePtr(UniquePtr&& right) : _ptr(right.release()){}

	template<class U>
	UniquePtr(UniquePtr<U>&& right) : _ptr(right.release()){}

	~UniquePtr(){
		reset();
	}

	UniquePtr& operator= (const UniquePtr& other) = delete;

	template<class U>
	UniquePtr& operator=(UniquePtr<U>&& right){
		reset(right.release());
		return *this;
	}

	T* get() const { return _ptr;}
	T* operator-> () const {return get();}
	T& operator* () const{return *get();}

	T* release(){
		auto oldPtr = _ptr;
		_ptr = nullptr;
		return oldPtr;
	}

	void reset(T* newPtr = nullptr){
		auto oldPtr = _ptr;
		_ptr = newPtr;
		if(oldPtr != nullptr){
			delete oldPtr;
		}
	}

	void swap(UniquePtr& other){
		std::swap(this->_ptr , other
			._ptr);
	}

	explicit operator bool(){
		return get() != nullptr;
	}

};


class A {
	inline static int object{};
	int myNum{};

public:
	A(){
		myNum = ++object;
		cout<< "Constructing object number: "<<myNum<<endl;
	}

	~A(){
		cout<< "Deconstructing object number: "<<myNum<<endl;
	}
};

int main()
{
	auto a1 = new A;
	UniquePtr<A> uptr{a1};

	auto a2 = new A;
	uptr.reset(a2);

	cout<< "Exiting main"<<endl;

}