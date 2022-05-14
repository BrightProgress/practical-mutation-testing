#include "boost/log/trivial.hpp"

// Design by Contract utility functions
//
///////////////////////
// assertions: Simple assertions which have to hold at the
// point at which they are declared. Invariants take
// as argument a boolean expressions.
//
// Typical usage:
// DBC_ASSERT(...);
//
//////////////////////
// invariants: Invariants are checked at the point at which
// they are declared and at the end of the scope in which they
// are declared.
//
// Typical usage:
// {
//   DBC_INVARIANT(some_label) << [&]()->bool{...};
//   ...
// }
//
///////////////////////
// class invariant: A class invariant is implemented by a method
// named _classInvariant in the class which returns a boolean
// value.
//
// Typical usage:
// class SomeClass {
// private:
//   bool _classInvariant();
//
// public:
//   someType aFunction(...) {
//      DBC_CLASS_INVARIANT(aFunction);
//      ...
//   }
// };
//
///////////////////////
// Assume-Guarantee: This contract checks a specified pre-condition
// at the point of declaration and and a post-condition at the end
// of the scope of its declaration. The Assume-Guarantee contract 
// requires a function that "captures" the state at a point of
// execution. The predicates default to trivially true predicates
// if they are not specified.
//
// Typical usage:
//
// {
//    DBC_ASSUME_GUARANTEE(label) 
//       << dbc::assume << [&]()->bool{...}
//       << dbc::guarantee << [&]()->bool{...};
//    ...
// }
// 
///////////////////////
// Pre-Post: This contract checks a specified pre-condition
// at the point of declaration and and a post-condition at the end
// of the scope of its declaration. The Pre-Post contract 
// requires a function that "captures" the state at a point of
// execution. This capture of state is the main difference between
// the Assume-Guarantee and Pre-Post contracts.
//
// Typical usage:
//
// {
//    DBC_PRE_POST(label, [&]()->T{...}) 
//       << dbc::pre << [&](T)->bool{...}
//       << dbc::post << [&](T,T)->bool{...};
//    ...
// }
// 
/////////////////////////
// While-loop: invariant and variants. The invariant is a boolean
// expression which has to hold at the beginning of the loop. The
// variant is an function that returns a type that has a well-founded
// ordering relation. The variant should strictly decrease at every
// invocation. Note that the variant type should have operator<
// which is used to check the variant. It is also assumed to have
// a default constructor.
//
// NOTE: Loop contracts are characterized by loop-variants. If
// a simple loop-invariant is needed it is simpler to use DBC_ASSERT
// to simply assert the loop invariant.
//
// Typical usage:
//
// while(...) {
// DBC_LOOP(label, variant)
//    << dbc::loop_invariant << [&]() -> bool{...}
// 
//    ...;
// }
//
// Note on Capture of variables:
// The pre-post and while-loop contracts depend on capturing the state at
// different points in time. It is important to ensure that the
// capture function specified for the contract returns value types
// and not references. Note that the lambda expression capturing
// the state should capture variables by reference as it is required
// to capture the state at different points in the execution.
//
// In general, the lambxa expressions shows in the typical examples above
// use reference based capture of variables. Apart from the 
// capture function of a pre-post invariant and the variant which should
// necessarily capture variables by reference, the other lambdas may use
// a combination of copy and reference based capture. However it is safer
// to use reference based capture.
// 
// Behaviour on violation of contracts:
// If a contract is violated the program is terminated using an assert.
// 
// Enabling and disabling contracts:
// Contracts can be enabled or disabled using the DBC_ENABLE pre-processor
// flag. If the DBC_ENABLE flag is not present the predicates/capture
// functions are not evaluated.
//
// Logging behaviour:
// In addition to asserting, the dbc functions use boost::log to generate
// log entries at the "error" level whenever any of the contracts are
// violated.


#ifndef DBC_HPP_INCLUDED
#define DBC_HPP_INCLUDED

namespace dbc{
  
  class invariant{
  private:
    std::string m_name;
    std::function<bool()> m_check;

  public:
    invariant(std::string name):
      m_name(name),
      m_check([]{return true;})
    { }

    ~invariant(){
      if(!m_check()){
        BOOST_LOG_TRIVIAL(error) << "invariant conract " 
                                 << m_name
                                 << " violated on exit" 
                                 << std::endl;

        assert(false);
      }
    }

    invariant& operator<< (std::function<bool()> check) {
      m_check = check;

      if(!m_check()){
        BOOST_LOG_TRIVIAL(error) << "invariant contract " 
                                 << m_name
                                 << " violated on entry" 
                                 << std::endl;

        assert(false);
      }

      return *this;
    }
  };
  
  
  struct assume_struct {};
  struct guarantee_struct {};
  struct pre_struct {};
  struct post_struct {};
  struct variant_struct {};
  struct loop_invariant_struct {};

  extern assume_struct assume;
  extern guarantee_struct guarantee;
  extern pre_struct pre;
  extern post_struct post;
  extern loop_invariant_struct loop_invariant;

  // Pre-Post contract
  template <typename T> class pre_post{
  private:
    std::string m_name;
    std::function<T()> m_capture;
    std::function<bool(T)> m_precheck;
    std::function<bool(T,T)> m_postcheck;
    T m_prevalue;

  public:
    pre_post(std::string name,
             std::function<T()> capture) :
      m_name(name),
      m_capture(capture),
      m_precheck([](T){return true;}),
      m_postcheck([](T,T){return true;}),
      m_prevalue(capture())
    { }

    pre_post& operator << (pre_struct) { return *this; };
    pre_post& operator << (post_struct) { return *this; };

    pre_post& operator << (std::function<bool(T)> precheck) {
      m_precheck = precheck;
      
      if (!m_precheck(m_prevalue)) {
        BOOST_LOG_TRIVIAL(error) << "pre-post contract " 
                                 << m_name
                                 << " violated on entry" 
                                 << std::endl;
        
        assert(false);
      }
      return *this ;
    };

    pre_post& operator << (std::function<bool(T,T)> postcheck) {
      m_postcheck = postcheck;
      return *this ;
    };

    ~pre_post(){
      if (!m_postcheck(m_prevalue, m_capture())) {
        BOOST_LOG_TRIVIAL(error) << "pre-post contract " 
                                 << m_name
                                 << " violated on exit" 
                                 << std::endl;

        assert(false);
      }
    }
  };

  // Assume-Guarantee contract
  class assume_guarantee{
  private:
    std::string m_name;
    std::function<bool()> m_precheck;
    std::function<bool()> m_postcheck;
    bool m_acceptPost = false;
  public:
    assume_guarantee(std::string name) :
      m_name(name),
      m_precheck([](){return true;}),
      m_postcheck([](){return true;})
    { }

    assume_guarantee& operator << (assume_struct) { m_acceptPost = false; return *this; };
    assume_guarantee& operator << (guarantee_struct) { m_acceptPost = true; return *this; };

    assume_guarantee& operator << (std::function<bool()> check) {
      if (!m_acceptPost) {
        m_precheck = check;
        
        if (!m_precheck()) {
          BOOST_LOG_TRIVIAL(error) << "assume-guarantee contract " 
                                   << m_name
                                   << " violated on entry" 
                                   << std::endl;
          
          assert(false);
        }
      }
      else {
        m_postcheck = check;
      }
      return *this ;
    };

    ~assume_guarantee(){
      if (!m_postcheck()) {
        BOOST_LOG_TRIVIAL(error) << "assume-guarantee contract " 
                                 << m_name
                                 << " violated on exit" 
                                 << std::endl;

        assert(false);
      }
    }
  };

  // Loop contract
  template <typename T> class loop{
  private:
    std::string m_name;
    std::function<T()> m_variant;
    std::function<bool()> m_invariant;
    T m_prevalue;

  public:
    loop(std::string name,
         std::function<T()> variant) :
      m_name(name),
      m_variant(variant),
      m_invariant([]{return true;}),
      m_prevalue(variant())
    { }

    loop& operator << (loop_invariant_struct) { return *this; };

    loop& operator << (std::function<bool()> invariant) {
      m_invariant = invariant;
      
      if (!m_invariant()) {
        BOOST_LOG_TRIVIAL(error) << "loop invariant contract for " 
                                 << m_name
                                 << " violated" 
                                 << std::endl;
        
        assert(false);
      }
      return *this ;
    };

    ~loop(){
      if (!(m_variant() < m_prevalue)) {
        BOOST_LOG_TRIVIAL(error) << "loop variant contract " 
                                 << m_name
                                 << " violated" 
                                 << std::endl;

        assert(false);
      }
    }
  };

  // 
  // Trivial implementations to be used when DBC_ENABLED
  // is not defined
  //
  struct pre_post_true {};
  inline pre_post_true& operator<<(pre_post_true& a, pre_struct) {
    return a;
  }
  inline pre_post_true& operator<<(pre_post_true& a, post_struct) {
    return a;
  }
  template <typename T> inline pre_post_true& operator<<(pre_post_true& a, T) {
    return a;
  }

  struct assume_guarantee_true {};
  inline assume_guarantee_true& operator<<(assume_guarantee_true& a, assume_struct) {
    return a;
  }
  inline assume_guarantee_true& operator<<(assume_guarantee_true& a, guarantee_struct) {
    return a;
  }
  inline assume_guarantee_true& operator<<(assume_guarantee_true& a, std::function<bool()>) {
    return a;
  }

  struct loop_true {};
  inline loop_true& operator<<(loop_true& a, loop_invariant_struct) {
    return a;
  }
  template <typename T> inline loop_true& operator<<(loop_true& a, T) {
    return a;
  }

  extern invariant dbc_invariant_true;
  extern pre_post_true dbc_pre_post_true;
  extern assume_guarantee_true dbc_assume_guarantee_true;
  extern loop_true dbc_loop_true;
}

//
// Macro definitions
//

#ifdef DBC_ENABLED
#define DBC_INVARIANT(label)                    \
  dbc::invariant __dbc_invariant( #label );     \
  __dbc_invariant
#else
#define DBC_INVARIANT(label)                    \
  if (1)                                        \
    ;                                           \
  else                                          \
    dbc::dbc_invariant_true
#endif


#define DBC_CLASS_INVARIANT(label) DBC_INVARIANT(label) << [this]{return _classInvariant();}

#ifdef DBC_ENABLED
#define DBC_PRE_POST(label,capture)                                     \
  auto __dbc_capture(capture());                                        \
  dbc::pre_post<decltype(__dbc_capture)>  __dbc_pre_post(#label,capture); \
  __dbc_pre_post << dbc::pre
#else
#define DBC_PRE_POST(label,capture)             \
  if (1)                                        \
    ;                                           \
    else                                        \
    dbc::dbc_pre_post_true << dbc::pre
#endif

#ifdef DBC_ENABLED
#define DBC_ASSUME_GUARANTEE(label)                      \
  dbc::assume_guarantee  __dbc_assume_guarantee(#label); \
  __dbc_assume_guarantee << dbc::assume
#else
#define DBC_ASSUME_GUARANTEE(label)                 \
  if (1)                                            \
    ;                                               \
  else                                              \
    dbc::dbc_assume_guarantee_true << dbc::assume
#endif


#ifdef DBC_ENABLED
#define DBC_LOOP(label,variant)                                         \
  auto __dbc_variant(variant());                                        \
  dbc::loop<decltype(__dbc_variant)>  __dbc_loop(#label,variant);       \
  __dbc_loop << dbc::loop_invariant
#else
#define DBC_LOOP(label,variant)                 \
  if (1)                                        \
    ;                                           \
  else                                          \
    dbc::dbc_loop_true << dbc::loop_invariant
#endif

#ifdef DBC_ENABLED
#define DBC_ASSERT(label,expr)                      \
  if (!(expr)){                                     \
  BOOST_LOG_TRIVIAL(error) << "assertion contract " \
  << #label                                         \
  << " violated"                                    \
  << std::endl;                                     \
  assert(false);                                    \
  }
#else
#define DBC_ASSERT(label,expr)
#endif

#endif //DBC_HPP_INCLUDED
