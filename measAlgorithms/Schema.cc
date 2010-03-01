#include "Schema.h"

/// Return a Schema given its name and component
Schema const& Schema::find(
        std::string const& name,        ///< The name of the desired Schema
        std::string const& component    ///< The component name, if not blank
                          ) const {
    for (std::vector<Schema::Ptr>::const_iterator ptr = begin(); ptr != end(); ++ptr) {
        if ((*ptr)->_component == component) {
            Schema const& val = (*ptr)->find(name);
            if (val) {
                return val;
            }
        } else if ((*ptr)->_component == "") {
            Schema const& val = (*ptr)->find(name, component);
            if (val) {
                return val;
            }
        }
    }
        
    return Schema::unknown();
}

/// Print v to os, using dynamic dispatch
std::ostream &operator<<(std::ostream &os, Schema const& v)
{
    return v.output(os);
}

/// Print all the values to os;  note that this is a virtual function called by operator<<
std::ostream &Schema::output(std::ostream &os) const {
    for (std::vector<Schema::Ptr>::const_iterator ptr = begin(); ptr != end(); ++ptr) {
        if (ptr != begin()) {
            os << " ";
        }
        os << "[";
        if (_component != "") {
            os << _component << ".";
        }
        os << **ptr << "]";
    }

    return os;
}

/**
 * Return the unknown object
 */
Schema const& Schema::unknown() {
    static Schema unknown("unknown", -1, UNKNOWN);

    return unknown;
}
