// -*- lsst-c++ -*-
#include "Astrometry.h"
#include "MeasurementData.h"

/**
 * Implement PSF astrometry.  The astronomical details are left to the reader
 */
class NaiveAstrometry : public AstrometryImpl<NaiveAstrometry>
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
    static void defineSchema(Schema::Ptr schema ///< our schema; == _mySchema
                      ) {
        Astrometry::defineSchema(schema);
    }
};

ASTROMETRY_BOILERPLATE("naive", Naive)

/**
 * Process the image; calculate values
 */
Astrometry::Ptr NaiveMeasureAstrometry::doMeasure(Image const&, float x, float y) {
    // Here is the real work, hiding in a comment
    return boost::make_shared<NaiveAstrometry>(x, 0.0, y, 0.0);
}
