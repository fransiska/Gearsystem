#ifndef _TRACKEDOBJECT_H_
#define _TRACKEDOBJECT_H_


#include<algorithm>
#include <vector>

template<typename Object> class Tracked;

template<typename Object> class ObjectTracker{
public:
  static std::vector<Object*> objs;
};

template<typename Object>
std::vector<Object*> ObjectTracker<Object>::objs;




template<typename Object> class Tracked{
public:
  Tracked(){
    ObjectTracker<Object>::objs.push_back(static_cast<Object*>(this));
  }
  
  ~Tracked(){
    std::vector<Object*>& objs( ObjectTracker<Object>::objs);
    objs.erase(std::remove(objs.begin(),objs.end(),static_cast<Object*>(this)),objs.end());
  }
};


#endif
