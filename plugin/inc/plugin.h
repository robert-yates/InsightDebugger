#ifndef INSIGHTDEBUGGER_PLUGIN_H
#define INSIGHTDEBUGGER_PLUGIN_H

#include "ServiceLocator.h"
#include "binaryninjaapi.h"
#include "sidebar.h"
#include "sidebarwidget.h"
#include "tl/expected.hpp"
#include "uicontext.h"
#include "uitypes.h"
#include "viewframe.h"
#include <QtWidgets>
#include <string>
#include <vector>

// A session is what binds a collection of BVs together
class Session {
  public:
    Session(const std::string& name, BinaryNinja::Ref<BinaryNinja::BinaryView> bv);
    const std::string& getName() const { return m_name; };

  private:
    BinaryNinja::Ref<BinaryNinja::BinaryView> m_bv;
    std::string m_name;
};

// Manages a collection of all the sessions
class SessionManager {
  public:
    SessionManager();
    void createSession(const std::string& name, BinaryNinja::Ref<BinaryNinja::BinaryView> bv);
    void removeSession(const std::string& sessionToRemove);

    tl::expected<std::reference_wrapper<Session>, std::string> getSession(const std::string& name);
    const std::vector<std::unique_ptr<Session>>& getSessions() { return m_sessions; };

  private:
    std::vector<std::unique_ptr<Session>> m_sessions;
    mutable std::mutex mtx;
};

// The sidebar widget that contains either the session selector
// or the active debugger widget.
// Widgets are per BV context sensitive.
class DebuggerWidget : public SidebarWidget {
    Q_OBJECT

  public:
    DebuggerWidget(const QString& name, ViewFrame* view, BinaryViewRef data);
    ~DebuggerWidget();
    DebuggerWidget(const DebuggerWidget&) = delete;
    DebuggerWidget& operator=(const DebuggerWidget&) = delete;
    DebuggerWidget(DebuggerWidget&&) = delete;
    DebuggerWidget& operator=(DebuggerWidget&&) = delete;

    void refresh();

  public slots:
    void slot_debuggerwidget_createSession();
    void slot_debuggerwidget_joinSession();

  private:
    void createLayouts();
    QVBoxLayout* createDefaultLayout();
    QVBoxLayout* createDebuggerLayout();

    std::string m_widgetBVName{};
    QComboBox* m_sessionDropdown;
    QPushButton* m_joinSessionButton;
    QStackedWidget* m_stackedWidget;

    std::string m_currentSessionName{};
    QLabel* m_insightDebuggerLabel;
    bool m_sessionActive = false;

    BinaryNinja::Ref<BinaryNinja::BinaryView> m_bv;
    enum class WidgetState { kSessionSelector = 0, kDebugger = 1 };
};

// Manages the collection of all widgets per BV view.
// here we can requests to refresh their UI
class DebuggerWidgetManager {
  public:
    DebuggerWidgetManager();
    DebuggerWidget* addDebuggerWidget(DebuggerWidget* widget);
    void removeDebuggerWidget(DebuggerWidget* widget);
    void refresh();

  private:
    std::vector<DebuggerWidget*> m_debuggerWidgets;
};

// The binaryninja needed container type for creating widgets
class DebuggerWidgetType : public SidebarWidgetType {
  public:
    DebuggerWidgetType(const QImage& icon, const QString& name);
    SidebarWidget* createWidget(ViewFrame* frame, BinaryViewRef data) override;

    SidebarWidgetLocation defaultLocation() const override { return SidebarWidgetLocation::LeftContent; }
    SidebarContextSensitivity contextSensitivity() const override { return PerViewTypeSidebarContext; }
};

// Observes UI events sent by binary ninja
class UIObserver : public UIContextNotification {
  public:
    void OnContextOpen(UIContext* context) override;
    void OnContextClose(UIContext* context) override;
    void OnViewChange(UIContext* context, ViewFrame* frame, const QString& type) override;
    bool OnBeforeCloseFile(UIContext* context, FileContext* file, ViewFrame* frame) override;
};

namespace plugin {

    void Initialise();

    template <typename... Args>
    void LogInfo(const char* fmt, Args... args) {
        auto logger = ServiceLocator::getInstance().getService<BinaryNinja::Logger>();
        if (logger)
            logger->LogInfo(fmt, args...);
    }
} // namespace plugin

#endif // INSIGHTDEBUGGER_PLUGIN_H
