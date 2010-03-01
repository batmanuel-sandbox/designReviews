// -*- lsst-c++ -*-
#if !defined(SCHEMA_DATA_H)
#define SCHEMA_DATA_H 1

template<typename T, typename BaseT>
class MeasurementData : public BaseT {
public:
    /// Return our Measurement's schema
    virtual Schema::Ptr getSchema() const {
        return _mySchema;
    }
    /// Resize the list of individual measurements of type T
    void resize(int n) { _data.resize(n); }

    /// Allocate enough space in _data to hold all values declared in the schema
    void init() {
        resize(getSchema()->size());    // getSchema() is virtual, but this is called from most-derived ctor
    }
    /**
     * Return some Measurement's value as a double given its Schema
     *
     * \sa getAsLong() to return as a long
     */
    double get(Schema const& se        ///< The schema entry for the value you want
              ) const {
        return getAsType<double>(se);
    }
    /**
     * Return some Measurement's value as a double given its Schema
     *
     * \sa getAsLong() to return as a long
     */
    double get(unsigned int i,                 ///< Index to set
               Schema const& se        ///< The schema entry for the value you want
              ) const {
        return getAsType<double>(i, se);
    }
    /**
     * Return some Measurement's value as a long given its Schema
     *
     * \sa get() to return as a double
     */
    long getAsLong(Schema const& se   ///< The schema entry for the value you want
                  ) const {
        return getAsType<long>(se);
    }
    /**
     * Return some Measurement's value as a long given its Schema
     *
     * \sa get() to return as a double
     */
    long getAsLong(unsigned int i,      ///< Index to set
                   Schema const& se     ///< The schema entry for the value you want
                  ) const {
        return getAsType<long>(i, se);
    }
    /**
     * Return a T by name and component
     *
     * Note that all values are returned as double; use the (protected) templated form of get if you need more
     * control
     */
    double get(std::string const& name,        ///< the name within T
               std::string const& component="" ///< the name within the set of measurements
              ) const {
        return get(getSchema()->find(name, component));
    }

    /**
     * Return an element T of an array by index name and component
     *
     * Note that all values are returned as double; use the (protected) templated form of get if you need more
     * control
     */
    double get(unsigned int i,                 ///< Index to set
               std::string const& name,        ///< the name within T
               std::string const& component="" ///< the name within the set of measurements
              ) const {
        return get(i, getSchema()->find(name, component));
    }             
protected:
    /// ctor
    MeasurementData() {
        if (!_mySchema) {
            _mySchema.reset(new Schema);
            T::defineSchema(_mySchema);
        }
    }

    /// Fast compile-time-computed access to set the values of _data
    template<unsigned int INDEX, typename U>
    void set(U value                    ///< Desired value
            ) {
        assert(INDEX < _data.size());
        _data[INDEX] = value;
    }

    /// Fast compile-time-computed access to set the values of _data
    template<unsigned int INDEX, typename U>
    void set(unsigned int i,            ///< Index to set
             U value                    ///< Desired value
            ) {
        assert(INDEX + i < _data.size());
        _data[INDEX + i] = value;
    }

    /// Fast compile-time-computed access to retrieve the values of _data
    template<unsigned int INDEX, typename U>
    U get() const {
        assert(INDEX < _data.size());
        return boost::any_cast<U>(_data[INDEX]);
    }

    /// Fast compile-time-computed access to retrieve the values of _data as an array
    template<unsigned int INDEX, typename U>
    U get(unsigned int i                ///< Desired index
         ) const {
        assert(INDEX + i < _data.size());
        return boost::any_cast<U>(_data[INDEX + i]);
    }
private:
    /// Return a value as the specified type
    template<typename U>
    U getAsType(Schema const& se        ///< The schema entry for the value you want
               ) const {
        return getAsType<U>(0, se);
    }

    /// Return a value as the specified type
    template<typename U>
    U getAsType(unsigned int i,         ///< Index into array (if se is an array)
                Schema const& se        ///< The schema entry for the value you want
               ) const {
        unsigned int const index = se.getIndex() + i;
        if (index >= _data.size()) {
            std::ostringstream msg;
            if (index - i < _data.size()) { // the problem is that i takes us out of range
                msg << "Index " << i << " is out of range for " << se.getName() <<
                    "[0," << se.getDimen() - 1 << "]";
            } else {
                msg << "Index " << index << " out of range [0," << _data.size() << "] for " << se.getName();
            }
            throw std::runtime_error(msg.str());
        }
        boost::any const& val = _data[index];

        switch (se.getType()) {
          case Schema::CHAR:
            return static_cast<U>(boost::any_cast<char>(val));
          case Schema::SHORT:
            return static_cast<U>(boost::any_cast<short>(val));
          case Schema::INT:
            return static_cast<U>(boost::any_cast<int>(val));
          case Schema::LONG:
            return static_cast<U>(boost::any_cast<long>(val));
          case Schema::FLOAT:
            return static_cast<U>(boost::any_cast<float>(val));
          case Schema::DOUBLE:
            return static_cast<U>(boost::any_cast<double>(val));
          default:
            break;
        }
        
        std::ostringstream msg;
        msg << "Unable to retrieve value of type " << se.getType() << " for " << se.getName();
        throw std::runtime_error(msg.str());
    }

    typedef std::vector<boost::any> DataStore;
    // The elements of T (if a leaf)
    DataStore _data;
    // The T's schema
    static Schema::Ptr _mySchema;
};

template<typename T, typename BaseT>
Schema::Ptr MeasurementData<T, BaseT>::_mySchema = Schema::Ptr();

#endif
