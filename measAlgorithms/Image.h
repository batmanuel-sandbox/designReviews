#if !defined(IMAGE_H)
#define IMAGE_H 1

template<typename T>
class Image {
public:
    Image(T val) : _val(val) {}
    operator T() const { return _val; }
private:
    T _val;
};

#endif
