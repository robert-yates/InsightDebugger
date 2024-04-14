#include "plugin.h"

void WidgetControls::slot_controls_debuggerStateChanged(DebuggerState state) {
    switch (state) {
        case DebuggerState::kRunning: {
            m_debuggerState = DebuggerState::kRunning;
            m_actionRun->setIcon(getColoredIcon(":/debugger/start", QColor(180, 180, 180)));
            break;
        }
        case DebuggerState::kPaused: {
            m_debuggerState = DebuggerState::kPaused;
            m_actionRun->setIcon(getColoredIcon(":/debugger/resume", getThemeColor(YellowStandardHighlightColor)));
            break;
        }
        case DebuggerState::kStopped: {
            m_debuggerState = DebuggerState::kStopped;
            m_actionRun->setIcon(getColoredIcon(":/debugger/start", getThemeColor(GreenStandardHighlightColor)));
        }
    }
}

WidgetControls::WidgetControls(QWidget* parent) : QToolBar(parent) {

    auto cyan = getThemeColor(CyanStandardHighlightColor);
    auto green = getThemeColor(GreenStandardHighlightColor);
    auto red = getThemeColor(RedStandardHighlightColor);
    auto white = getThemeColor(WhiteStandardHighlightColor);
    auto orange = getThemeColor(OrangeStandardHighlightColor);

    m_actionRun = addAction(getColoredIcon(":/debugger/start", green), "Launch", [=]() {
        if (m_debuggerState == DebuggerState::kStopped)
            emit signal_controls_startDebugger();
        if (m_debuggerState == DebuggerState::kPaused)
            emit signal_controls_resumeDebugger();
    });
    m_actionRun->setToolTip(getToolTip("Launch"));

    m_actionStopDebugging = addAction(getColoredIcon(":/debugger/stop", red), "Launch", [=]() {});
    m_actionStopDebugging->setToolTip(getToolTip("Remove"));

    m_actionClearConsole = addAction(getColoredIcon(":/debugger/cancel", orange), "Pause", [this]() {});
    m_actionClearConsole->setToolTip(getToolTip("Clear Console"));

    m_actionDisconnect = addAction(getColoredIcon(":/debugger/disconnect", red), "Disconnect", [this]() {});

    addSeparator();
    m_actionStepInto = addAction(getColoredIcon(":/debugger/step-into", cyan), "Step Into", [this]() {});
    m_actionStepInto->setToolTip(getToolTip("Step Into"));

    m_actionStepOver = addAction(getColoredIcon(":/debugger/step-over", cyan), "Step Over", [this]() {
        if (m_debuggerState == DebuggerState::kPaused)
            emit signal_controls_stepOver();
    });
    m_actionStepOver->setToolTip(getToolTip("Step Over"));

    m_actionStepReturn = addAction(getColoredIcon(":/debugger/step-out", cyan), "Step Return", [this]() {});
    m_actionStepReturn->setToolTip(getToolTip("Step Return"));

    addSeparator();
    m_actionSettings = addAction(getColoredIcon(":/debugger/settings", cyan), "Settings", [this]() {});
    m_actionSettings->setToolTip(getToolTip("Debug Adapter Settings"));

    this->setIconSize(QSize(20, 20));
}
WidgetControls::~WidgetControls() {}

QIcon WidgetControls::getColoredIcon(const QString& iconPath, const QColor& color) {
    auto pixmap = QPixmap(iconPath);
    auto mask = pixmap.createMaskFromColor(QColor(0, 0, 0), Qt::MaskInColor);
    pixmap.fill(color);
    pixmap.setMask(mask);
    return QIcon(pixmap);
}

QString WidgetControls::getToolTip(const QString& name) {
    QString result = name;
    auto keyBinding = UIAction::getKeyBinding(name);
    if (!keyBinding.isEmpty())
        result += (QString(" (") + keyBinding[0].toString() + ")");

    return result;
}