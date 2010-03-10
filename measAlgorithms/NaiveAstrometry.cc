// -*- lsst-c++ -*-
#include "Astrometry.h"

namespace {
/**
 * Implement Naive astrometry.  The astronomical details are left to the reader
 */
class NaiveAstrometry : public Astrometry
{
public:
    typedef boost::shared_ptr<NaiveAstrometry> Ptr;
    typedef boost::shared_ptr<NaiveAstrometry const> ConstPtr;

    /// Ctor
    NaiveAstrometry(double x, float xErr, double y, float yErr) {
        init();                         // This allocates space for fields added by defineSchema
        set<X>(x);                      // ... if you don't, these set calls will fail an assertion
        set<X_ERR>(xErr);               // the type of the value must match the schema
        set<Y>(y);
        set<Y_ERR>(yErr);
    }

    /// Add desired fields to the schema
    virtual void defineSchema(Schema::Ptr schema ///< our schema; == _mySchema
                             ) {
        Astrometry::defineSchema(schema);
    }

    static Astrometry::Ptr doMeasure(Image const& im, Peak const&);
};

/**
 * Process the image; calculate values
 */
Astrometry::Ptr NaiveAstrometry::doMeasure(Image const&, Peak const& peak) {
    // Here is the real work, hiding in a comment
    return boost::make_shared<NaiveAstrometry>(peak.getX(), 0.0, peak.getY(), 0.0);
}

/************************************************************************************************************/
/**
 * Declare the existence of a "naive" algorithm
 */
volatile bool isInstance[] = {
    MeasureAstrometry::declare("naive", &NaiveAstrometry::doMeasure)
};
}
