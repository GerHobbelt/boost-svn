// Copyright (c) 2003 Daniel Wallin

// Permission is hereby granted, free of charge, to any person or organization 
// obtaining a copy of the software covered by this license (the "Software") 
// to use, reproduce, display, distribute, execute, and transmit the Software, 
// and to prepare derivative works of the Software, and to permit others
// to do so, all subject to the following:

// The copyright notice in the Software and this entire statement, including 
// the above license grant, this restriction and the following disclaimer, 
// must be included in all copies of the Software, in whole or in part, and 
// all derivative works of the Software, unless such copies or derivative 
// works are solely in the form of machine-executable object code generated by 
// a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.

#ifndef BOOST_LANGBINDING_REGISTRY_IMPLEMENTATION
#define BOOST_LANGBINDING_REGISTRY_IMPLEMENTATION

#include <boost/langbinding/registry.hpp>
#include <boost/langbinding/registration.hpp>
#include <boost/langbinding/rvalue_data.hpp>
#include <boost/tuple/tuple.hpp>

#include <memory>
#include <map>
#include <iostream>

namespace boost { namespace langbinding {

   namespace {

      using tuples::tie;
      tuples::detail::swallow_assign _; // ignore is too verbose
     
   }
   
   namespace detail {
   
      template<class T>
      struct BOOST_LANGBINDING_DECL registry_impl
      {
         typedef std::map<
              typename registry<T>::type_info_
            , registration<T>
         > registry_t;

         registry_t entries;
      };

   }
   
   template<class T>
   registry_base<T>::registry_base()
      : m_pimpl(new detail::registry_impl<T>)
   {}

   template<class T>
   registry_base<T>::~registry_base() 
   {}
   
   template<class T>
   const registration<T>*
   registry_base<T>::lookup(const type_info_& x)
   {
      std::cout << "lookup for \"" << x << "\"\n";

      typename detail::registry_impl<T>::registry_t::iterator iter;

      tie(iter, _) = m_pimpl->entries.insert(
            typename detail::registry_impl<T>::registry_t::value_type(
               x, registration<T>(x)));

      return &iter->second;
   }

   template<class T>
   const registration<T>* 
      registry_base<T>::query(const type_info_& x)
   {
      std::cout << "query for \"" << x << "\"\n";

      typename detail::registry_impl<T>::registry_t::const_iterator iter
         = m_pimpl->entries.find(x);

      return iter == m_pimpl->entries.end() ? 0 : &iter->second;
   }

   template<class T>
   void registry_base<T>::insert(const type_info_& x, 
      typename registry_base<T>::lvalue_from_function convert)
   {
      std::cout << "lvalue insert for \"" << x << "\"\n";

      registration<T>& r 
         = const_cast<registration<T>&>(*lookup(x));

      typedef lvalue_chain<T> chain_t;

      chain_t* chain = new chain_t;
      chain->convert = convert;
      chain->next = r.lvalue_converters;
      r.lvalue_converters = chain;
   }

   template<class T>
   void registry_base<T>::insert(const type_info_& x,
      typename registry_base<T>::rvalue_from_stage1 convertible,
      typename registry_base<T>::rvalue_from_stage2 convert)
   {
      std::cout << "rvalue insert for \"" << x << "\"\n";

      registration<T>& r 
         = const_cast<registration<T>&>(*lookup(x));

      typedef rvalue_chain<T> chain_t;

      chain_t* chain = new chain_t;
      chain->convertible = convertible;
      chain->convert = convert;
      chain->next = r.rvalue_converters;
      r.rvalue_converters = chain;
   }

   template<class T>
   void registry_base<T>::export_converters(registry_base& to)
   {
      for (typename detail::registry_impl<T>::registry_t::const_iterator iter
            = m_pimpl->entries.begin()
            ; iter != m_pimpl->entries.end()
            ; ++iter)
      {
         const type_info_& type = iter->first;

         for (lvalue_chain<T>* c = iter->second.lvalue_converters; 
               c != 0; c = c->next)
         {
            to.insert(type, c->convert);
         }

         for (rvalue_chain<T>* c2 = iter->second.rvalue_converters; 
               c2 != 0; c2 = c2->next)
         {
            to.insert(type, c2->convertible, c2->convert);
         }
      }
   }

   template<class T>
   void registry_base<T>::export_converters(const type_info_& x, registry_base& to)
   {
      const registration<T>* r = query(x);
      if (!r) return;

      for (lvalue_chain<T>* c = r->lvalue_converters; 
            c != 0; c = c->next)
      {
         to.insert(x, c->convert);
      }

      for (rvalue_chain<T>* c2 = r->rvalue_converters;
            c2 != 0; c2 = c2->next)
      {
         to.insert(x, c2->convertible, c2->convert);
      }
   }

}}

#endif

