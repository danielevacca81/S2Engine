// ValidType.h
//
#ifndef CORE_VALIDTYPE_H
#define CORE_VALIDTYPE_H

// ------------------------------------------------------------------------------------------------
template< typename T>
class valid_t
{
public:
    valid_t()
        : _valid( false )
        , _value()
    {}

    valid_t( const T &value )
        : _valid( true )
        , _value( value )
    {}

    void set( const T &value )
    {
        _value = value;
        _valid = true;
    }

	void invalidate()
	{
		_valid = false;
	}

	valid_t<T> &operator=( const valid_t<T> &v )
	{
		if( v._valid )
		{
			_value = v._value;
			_valid = true;
		}
		return *this;
	}

    valid_t<T> &operator=( const T &value )
    {
        _value = value;
        _valid = true;
        return *this;
    }

	bool operator!=( const valid_t<T> &value ) const
    {
        bool ret = false;
        ret |= _valid != value._valid;
        if (_valid)
        {
            ret |= _value != value._value;
        }

        return ret;
    }

	T    value() const { if( _valid ) return _value; return {}; }
    bool valid() const { return _valid; }

	operator T() const { return value(); }

private:
    bool _valid;
    T    _value;
};

#endif // !COMMON_VALIDTYPE_H
