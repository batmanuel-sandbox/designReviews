#if !defined(IMAGE_H)
#define IMAGE_H 1

#include "boost/shared_ptr.hpp"

template<typename T>
class Image {
public:
    typedef boost::shared_ptr<Image> Ptr;
    typedef boost::shared_ptr<Image const> ConstPtr;
    
    Image(T val) : _val(val) {}
    operator T() const { return _val; }
private:
    T _val;
};

#endif
