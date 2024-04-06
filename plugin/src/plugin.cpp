
#include "plugin.h"

namespace plugin {

    void Initialise() {
        BinaryNinja::LogInfo("-----> [InsightDebugger]: Initalise Plugin");
    }

} // namespace plugin

extern "C" {
BN_DECLARE_UI_ABI_VERSION

BINARYNINJAPLUGIN bool UIPluginInit() {
    plugin::Initialise();
    return true;
}
}