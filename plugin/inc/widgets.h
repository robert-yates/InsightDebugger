#ifndef INSIGHTDEBUGGER_WIDGETS_H
#define INSIGHTDEBUGGER_WIDGETS_H

#include <QtWidgets>

enum class DebuggerState { kRunning, kPaused, kStopped };

class WidgetControls : public QToolBar {
    Q_OBJECT

  public:
    WidgetControls(QWidget* parent = nullptr);
    virtual ~WidgetControls();
    QAction* getActionRun() { return m_actionRun; };
    QAction* getActionStepOver() { return m_actionStepOver; };

  public slots:
    void slot_controls_debuggerStateChanged(DebuggerState state);

  signals:
    void signal_controls_startDebugger();
    void signal_controls_resumeDebugger();
    void signal_controls_stepOver();

  private:
    QIcon getColoredIcon(const QString& iconPath, const QColor& color);
    QString getToolTip(const QString& name);

    QAction* m_actionRun;
    QAction* m_actionStopDebugging;
    QAction* m_actionClearConsole;
    QAction* m_actionDisconnect;
    QAction* m_actionStepInto;
    QAction* m_actionStepOver;
    QAction* m_actionStepReturn;
    QAction* m_actionSettings;
    DebuggerState m_debuggerState = DebuggerState::kStopped;
};

class WidgetRegisters : public QTableWidget {
    Q_OBJECT

  public:
    WidgetRegisters(QWidget* parent = nullptr);
    ~WidgetRegisters();

  private:
    void updateFonts();
    void updateRegisterValue(int row, uint32_t value);

    QFont m_font;
    int m_baseline;
    int m_charWidth;
    int m_charHeight;
    int m_charOffset;
};

#endif // INSIGHTDEBUGGER_WIDGETS_H