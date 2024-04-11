
#include "plugin.h"

namespace plugin {

    void Initialise() {
        ServiceLocator::getInstance().provideService(
            std::make_unique<BinaryNinja::Logger>(BNLogCreateLogger("InsightDebugger", 0)));
        ServiceLocator::getInstance().provideService(std::make_unique<SessionManager>());
        ServiceLocator::getInstance().provideService(std::make_unique<DebuggerWidgetManager>());

        plugin::LogInfo("Initalise Plugin");
        Sidebar::addSidebarWidgetType(new DebuggerWidgetType(QImage(":/debugger/debugger-yates"), "InsightDebugger"));
        UIContext::registerNotification(new UIObserver());
    }

} // namespace plugin

extern "C" {
BN_DECLARE_UI_ABI_VERSION

BINARYNINJAPLUGIN bool UIPluginInit() {
    plugin::Initialise();
    return true;
}
}