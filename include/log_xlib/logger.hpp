#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "definations.hpp"

#include "core/logger_impl.hpp"

#ifdef NAMESPACE_DEFINITION
using namespace xlib::logger;
#endif // NAMESPACE_DEFINITION

#ifdef EXTENSIONS_ALLOWED
#include "sink/sink_dataStructure.hpp"
#include "sink/sink.hpp"
#include "sink/sink_factory.hpp"
#endif // EXTENSIONS_ALLOWED


#endif // LOGGER_HPP