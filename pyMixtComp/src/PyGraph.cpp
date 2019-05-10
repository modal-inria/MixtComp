#include "PyGraph.hpp"

using namespace std;

namespace mixt {

PyGraph::PyGraph(const dict &d) : d_(d) {}

/* Setters */
void PyGraph::set(const dict &d) { d_ = d; }

/* Getters */
const dict &PyGraph::getD() const { return d_; }

/* Graph Functions */
void PyGraph::addSubGraph(const vector<string> &path, const string &name,
                          const PyGraph &p) {
  addSubGraph(path, 0, d_, name, p);
}
void PyGraph::getSubGraph(const vector<string> &path, PyGraph &p) const {
  dict d;
  go_to(path, d);
  p.set(d);
}

/* Payload Functions */
template <typename Type>
void PyGraph::add_payload(const vector<string> &path, const string &name,
                          const Type &p) {
  add_payload(path, 0, d_, name, p);
}

template <typename Type>
Type PyGraph::get_payload(const vector<string> &path,
                          const string &name) const {
  Type val;
  get_payload(path, name, val);
  return val;
}

bool PyGraph::exist_payload(const vector<string> &path,
                            const string &name) const {
  dict d;
  go_to(path, d);
  return d.has_key(name);
}

void PyGraph::name_payload(const vector<string> &path,
                           std::list<string> &l) const {
  dict d;
  go_to(path, d);
  for (Index i = 0; i < len(d); ++i) l.push_back(extract<string>(d[i]));
}

/* Private Definitions */
/* Go To Functions */
void PyGraph::go_to(const vector<string> &path, dict &d) const {
  go_to(path, 0, d_, d);
}

void PyGraph::go_to(const vector<string> &path, Index currDepth,
                    const dict &currLevel, dict &d) const {
  if (currDepth == path.size())
    d = currLevel;
  else {
    if (!currLevel.has_key(path[currDepth])) {
      string askedPath;
      for (Index i = 0; i < currDepth + 1; ++i) askedPath += "/" + path[i];
      throw(askedPath + " path does not exist.");
    }
    const dict &nextLevel = extract<dict>(currLevel[path[currDepth]]);
    go_to(path, currDepth + 1, nextLevel, d);
  }
}

/* Payload functions */
template <typename Type>
void PyGraph::add_payload(const vector<string> &path, Index currDepth,
                          dict &currLevel, const string &name, const Type &p) {
  if (currDepth == path.size())
    tranlateCPPToPython(p, currLevel[name]);
  else {
    extract<dict &> nextLevel(currLevel[path[currDepth]]);
    if (!currLevel.has_key(path[currDepth]))
      nextLevel = dict();
    else if (nextLevel.check()) {
      string askedPath;
      for (Index i = 0; i < currDepth + 1; ++i) askedPath += "/" + path[i];
      throw(askedPath + " path does not exist.");
    }
    add_payload(path, currDepth + 1, nextLevel(), name, p);
  }
}

template <typename Type>
void PyGraph::get_payload(const vector<string> &path, const string &name,
                          Type &p) const {
  dict d;
  go_to(path, d);
  if (!d.has_key(name)) {
    string cPath;
    completePath(path, name, cPath);
    throw(cPath + " object does not exist.");
  }
  translatePythonToCPP(d[name], p);
}

/* Graph Functions */
void PyGraph::addSubGraph(const vector<string> &path, Index currDepth,
                          dict &currLevel, const string &name,
                          const PyGraph &p) {
  if (currDepth == path.size())
    currLevel[name] = p.getD();
  else {
    extract<dict &> nextLevel(currLevel[path[currDepth]]);
    if (!currLevel.has_key(path[currDepth]))
      nextLevel = dict();
    else if (nextLevel.check()) {
      string askedPath;
      for (Index i = 0; i < currDepth + 1; ++i) askedPath += "/" + path[i];
      throw(askedPath + " already exists and is not a python object.");
    }
    addSubGraph(path, currDepth + 1, nextLevel(), name, p);
  }
}
}  // namespace mixt
