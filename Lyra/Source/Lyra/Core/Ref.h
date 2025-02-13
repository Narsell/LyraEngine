#pragma once

#include <memory>

/* TODO: For now this is just an alias but in the future this should be a proper non-atomic ref counting system. */
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;
