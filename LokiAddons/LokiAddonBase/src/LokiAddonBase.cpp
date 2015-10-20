// 
// Purpose: Defines the base class for Loki's node addons
//

#include "LokiAddonBase.hpp"

// v8 constructor
v8::Persistent<v8::Function> LokiAddonBase::constructor;

// constructor
LokiAddonBase::LokiAddonBase() {}

// destructor
LokiAddonBase::~LokiAddonBase() {}
