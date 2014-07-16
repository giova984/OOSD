/***************************************************************************
    copyright            : by Jim Hyslop, modified by Giuseppe Lipari
    email                : g.lipari@sssup.it
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::string defaultIDKeyType;

/**
    The abstract factory.
    Implemented using the Singleton pattern
*/
template <class manufacturedObj, typename classIDKey=defaultIDKeyType>
class genericFactory
{
    /// a BASE_CREATE_FN is a static factory method: a function that
	/// returns an unique_ptr to a manufactuedObj.  Note that you can customise
    /// the type of parameters to allow overloaded ctors
    typedef std::unique_ptr<manufacturedObj> (*BASE_CREATE_FN)(const std::string &par);

    /// FN_REGISTRY is the registry of all the BASE_CREATE_FN
    /// pointers registered.  Functions are registered using the
    /// regCreateFn member function (see below).
    typedef std::map<classIDKey, BASE_CREATE_FN> FN_REGISTRY;
    FN_REGISTRY registry;

    /// Singleton implementation - private ctor & copying, with
    /// no implementation on the copying.
    genericFactory();
    genericFactory(const genericFactory&); // Not implemented
    genericFactory &operator=(const genericFactory&); // Not implemented
public:

    /// Singleton access.
    static genericFactory &instance();

    /// Classes derived from manufacturedObj call this function once
    /// per program to register the class ID key, and a pointer to
    /// the function that creates the class.
    void regCreateFn(const classIDKey &, BASE_CREATE_FN);

    /// Create a new class of the type specified by className.
    std::unique_ptr<manufacturedObj> create(const classIDKey &className,
						const std::string &param) const;
};

////////////////////////////////////////////////////////////////////////
// Implementation details.

template <class manufacturedObj, typename classIDKey>
genericFactory<manufacturedObj, classIDKey>::genericFactory()
{
}

template <class manufacturedObj, typename classIDKey>
genericFactory<manufacturedObj, classIDKey> &genericFactory<manufacturedObj, classIDKey>::instance()
{
    // Note that this is not thread-safe!
    static genericFactory theInstance;
    return theInstance;
}

/// Register the creation function.  This simply associates the classIDKey
/// with the function used to create the class.  The return value is a dummy
/// value, which is used to allow static initialization of the registry.
/// See example implementations in base.cpp and derived.cpp
template <class manufacturedObj, typename classIDKey>
void genericFactory<manufacturedObj, classIDKey>::regCreateFn(const classIDKey &clName, BASE_CREATE_FN func)
{
    registry[clName]=func;
}

/// The create function simple looks up the class ID, and if it's in the list,
/// the statement "(*i).second();" calls the function.
template <class manufacturedObj, typename classIDKey>
std::unique_ptr<manufacturedObj> genericFactory<manufacturedObj, classIDKey>::create(const classIDKey &className, const std::string &parms) const
{
    std::unique_ptr<manufacturedObj> ret(nullptr);

    typename FN_REGISTRY::const_iterator regEntry=registry.find(className);
    if (regEntry != registry.end()) {
    	return (*regEntry).second(parms);
    }
    return ret;
}

/// Helper template to make registration painless and simple.
template <class ancestorType,
		  class manufacturedObj,
          typename classIDKey=defaultIDKeyType>
class registerInFactory {
public:
    static std::unique_ptr<ancestorType> createInstance(const std::string &par)
    {
    	return std::unique_ptr<ancestorType>(manufacturedObj::createInstance(par));
    }
    registerInFactory(const classIDKey &id)
    {
    	genericFactory<ancestorType>::instance().regCreateFn(id, createInstance);
    }
};

#endif
