#ifndef PYMIXT_PYGRAPH_H
#define PYMIXT_PYGRAPH_H

#include <list>
#include <map>

#include <Python.h>
#include <boost/python.hpp>

#include <IO/IOFunctions.h>
#include <IO/NamedAlgebra.h>
#include <LinAlg/mixt_LinAlg.h>

using namespace boost::python;
using namespace std;

namespace mixt {

class PyGraph {
 public:
  /* Constructors */
  PyGraph(){};
  PyGraph(const dict &d);

  /* Setters */
  void set(const dict &d);

  /* Getters */
  const dict &getD() const;

  /* Graph Functions */
  void addSubGraph(const vector<string> &path, const string &name,
                   const PyGraph &p);
  void getSubGraph(const vector<string> &path, PyGraph &p) const;

  /* Payload Functions */
  template <typename Type>
  void add_payload(const vector<string> &path, const string &name,
                   const Type &p);

  template <typename Type>
  Type get_payload(const vector<string> &path, const string &name) const;

  bool exist_payload(const vector<string> &path, const string &name) const;

  void name_payload(const vector<string> &path, std::list<string> &l) const;

 private:
  /* Go To Functions */
  void go_to(const vector<string> &path, dict &d) const;
  void go_to(const vector<string> &path, Index currDepth, const dict &currLevel,
             dict &d) const;

  /* Payload functions */
  template <typename Type>
  void add_payload(const vector<string> &path, Index currDepth, dict &currLevel,
                   const string &name, const Type &p);

  template <typename Type>
  void get_payload(const vector<string> &path, const string &name,
                   Type &p) const;

  /* Graph Functions */
  void addSubGraph(const vector<string> &path, Index currDepth, dict &currLevel,
                   const string &name, const PyGraph &p);

  dict d_;
};
}  // namespace mixt

#endif
