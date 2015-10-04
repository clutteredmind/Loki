// 
// Purpose: Defines the base class for Loki's node addons
//

#include "LokiAddonBase.hpp"

// the node libraries
#include <node.h>

// use the namespace so we don't have to have v8:: everywhere
using namespace v8;

// constructor
LokiAddonBase::LokiAddonBase() { }

// destructor
LokiAddonBase::~LokiAddonBase() { }