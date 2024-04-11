#include "plugin.h"

DebuggerWidgetManager::DebuggerWidgetManager() {
    plugin::LogInfo("DebuggerWidgetManager::DebuggerWidgetManager");
}

DebuggerWidget* DebuggerWidgetManager::addDebuggerWidget(DebuggerWidget* widget) {
    plugin::LogInfo("DebuggerWidgetManager::addDebuggerWidget");
    m_debuggerWidgets.push_back(widget);
    return m_debuggerWidgets.back();
}

void DebuggerWidgetManager::removeDebuggerWidget(DebuggerWidget* widget) {
    plugin::LogInfo("DebuggerWidgetManager::removeDebuggerWidget");
    m_debuggerWidgets.erase(std::remove(m_debuggerWidgets.begin(), m_debuggerWidgets.end(), widget),
                            m_debuggerWidgets.end());
}

void DebuggerWidgetManager::refresh() {
    for (auto& widget : m_debuggerWidgets) {
        widget->refresh();
    }
}
