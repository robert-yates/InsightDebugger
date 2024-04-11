#include "plugin.h"

Session::Session(const std::string& name, BinaryNinja::Ref<BinaryNinja::BinaryView> bv) : m_name(name), m_bv(bv){};
