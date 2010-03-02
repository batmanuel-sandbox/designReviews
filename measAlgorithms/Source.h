#if !defined(SOURCE_H)
#define SOURCE_H 1

#include <iostream>
#include "Astrometry.h"
#include "Photometry.h"

class Source {
public:
    Source() {}

    void setAstrometry(Measurement<Astrometry> const& astrom) { _astrom = astrom; }
    Measurement<Astrometry> const& getAstrometry() const { return _astrom; }
    void setPhotometry(Measurement<Photometry> const& photom) { _photom = photom; }
    Measurement<Photometry> const& getPhotometry() const { return _photom; }
private:
    Measurement<Photometry> _photom;
    Measurement<Astrometry> _astrom;
};

std::ostream &operator<<(std::ostream &os, Source const& s);

#endif
