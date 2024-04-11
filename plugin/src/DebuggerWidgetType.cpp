#include "plugin.h"

DebuggerWidgetType::DebuggerWidgetType(const QImage& icon, const QString& name) : SidebarWidgetType(icon, name) {
    plugin::LogInfo("DebuggerWidgetType::DebuggerWidgetType");
}

SidebarWidget* DebuggerWidgetType::createWidget(ViewFrame* frame, BinaryViewRef data) {
    plugin::LogInfo("DebuggerWidgetType::createWidget");

    return ServiceLocator::getInstance().getService<DebuggerWidgetManager>()->addDebuggerWidget(
        new DebuggerWidget("InsightDebugger", frame, data));
}