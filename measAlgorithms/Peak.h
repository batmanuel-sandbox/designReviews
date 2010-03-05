#if !defined(PEAK_H)
#define PEAK_H 1

class Peak {
public:
    typedef 
    boost::shared_ptr<Peak> Ptr;
    boost::shared_ptr<Peak const> ConstPtr;

    Peak(float x, float y) : _x(x), _y(y) {}
    float getX() const { return _x; }
    float getY() const { return _y; }
private:
    float _x, _y;
};

#endif
