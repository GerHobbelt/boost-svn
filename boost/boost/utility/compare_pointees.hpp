// (C) 2002, Fernando Luis Cacciola Carballal.
//
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
//
// Permission to use or copy this software for any purpose is hereby granted
// without fee, provided the above notices are retained on all copies.
// Permission to modify the code and to distribute modified code is granted,
// provided the above notices are retained, and a notice that the code was
// modified is included with the above copyright notice.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef BOOST_UTILITY_COMPARE_POINTEES_25AGO2003_HPP
#define BOOST_UTILITY_COMPARE_POINTEES_25AGO2003_HPP

#include<functional>

namespace boost {

// template<class OP> bool equal_pointees(OP const& x, OP const& y);
// template<class OP> struct equal_pointees_t;
//
// Being OP a model of OptionalPointee (either a pointer or an optional):
//
// If both x and y have valid pointees, returns the result of (*x == *y)
// If only one has a valid pointee, returns false.
// If none have valid pointees, returns true.
// No-throw
template<class OptionalPointee>
inline
bool equal_pointees ( OptionalPointee const& x, OptionalPointee const& y )
{
  return (!x) != (!y) ? false : ( !x ? true : (*x) == (*y) ) ;
}

template<class OptionalPointee>
struct equal_pointees_t : std::binary_function<bool,OptionalPointee,OptionalPointee>
{
  bool operator() ( OptionalPointee const& x, OptionalPointee const& y ) const
    { return equal_pointees(x,y) ; }
} ;

// template<class OP> bool less_pointees(OP const& x, OP const& y);
// template<class OP> struct less_pointees_t;
//
// Being OP a model of OptionalPointee (either a pointer or an optional):
//
// If y has not a valid pointee, returns false.
// ElseIf x has not a valid pointee, returns true.
// ElseIf both x and y have valid pointees, returns the result of (*x < *y)
// No-throw
template<class OptionalPointee>
inline
bool less_pointees ( OptionalPointee const& x, OptionalPointee const& y )
{
  return !y ? false : ( !x ? true : (*x) < (*y) ) ;
}

template<class OptionalPointee>
struct less_pointees_t : std::binary_function<bool,OptionalPointee,OptionalPointee>
{
  bool operator() ( OptionalPointee const& x, OptionalPointee const& y ) const
    { return less_pointees(x,y) ; }
} ;

} // namespace boost

#endif

