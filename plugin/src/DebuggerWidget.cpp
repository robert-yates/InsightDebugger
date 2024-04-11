#include "plugin.h"
#include <QPlainTextEdit>
#include <vector>

DebuggerWidget::DebuggerWidget(const QString& name, ViewFrame* view, BinaryViewRef data) : SidebarWidget(name), m_bv(data) {
    QFileInfo fileInfo(m_bv->GetFile()->GetOriginalFilename().c_str());
    QString fileName = fileInfo.fileName();
    m_widgetBVName = fileName.toStdString();
    plugin::LogInfo("DebuggerWidget::DebuggerWidget n:%s v:%p bv:%p", m_widgetBVName.c_str(), view, data.GetPtr());

    createLayouts();
    setStyleSheet("QToolBar { background: palette(base); }");

    // Let other widgets know about this one
    ServiceLocator::getInstance().getService<DebuggerWidgetManager>()->refresh();
}

DebuggerWidget::~DebuggerWidget() {
    plugin::LogInfo("DebuggerWidget::~DebuggerWidget");

    if (m_sessionActive)
        ServiceLocator::getInstance().getService<SessionManager>()->removeSession(m_currentSessionName);

    ServiceLocator::getInstance().getService<DebuggerWidgetManager>()->removeDebuggerWidget(this);
    ServiceLocator::getInstance().getService<DebuggerWidgetManager>()->refresh();
}

void DebuggerWidget::createLayouts() {
    m_stackedWidget = new QStackedWidget(this);

    // Create Session Selector Layout in the Stack
    QWidget* sessionSelectorContainer = new QWidget;
    sessionSelectorContainer->setLayout(createDefaultLayout());
    m_stackedWidget->addWidget(sessionSelectorContainer);

    // Create Debugger Layout in the Stack
    QWidget* debuggerContainer = new QWidget;
    debuggerContainer->setLayout(createDebuggerLayout());
    m_stackedWidget->addWidget(debuggerContainer);

    // Set SideBarWidget to the Stack Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_stackedWidget);
    setLayout(mainLayout);

    m_stackedWidget->setCurrentIndex(0);
}

QVBoxLayout* DebuggerWidget::createDefaultLayout() {
    plugin::LogInfo("DebuggerWidget::createDefaultLayout");

    QVBoxLayout* sessionSelectorLayout = new QVBoxLayout(this);

    // Create Session: This BV will be the main debugged binary.
    QHBoxLayout* buttonLayout = new QHBoxLayout(this);
    QPushButton* createSessionButton = new QPushButton("Create Session");
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(createSessionButton);
    buttonLayout->addStretch(1);
    sessionSelectorLayout->addLayout(buttonLayout);

    // Separator
    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    sessionSelectorLayout->addWidget(separator);

    // Join Session: This BV will be attached to an existing session, usually a .dll
    QHBoxLayout* joinSessionButtonLayout = new QHBoxLayout(this);
    m_joinSessionButton = new QPushButton("Join Session");
    joinSessionButtonLayout->addStretch(1);
    joinSessionButtonLayout->addWidget(m_joinSessionButton);
    joinSessionButtonLayout->addStretch(1);
    sessionSelectorLayout->addLayout(joinSessionButtonLayout);

    // Dropdown list box of current sessions
    m_sessionDropdown = new QComboBox(this);
    sessionSelectorLayout->addWidget(m_sessionDropdown);

    // Connect button actions
    sessionSelectorLayout->addStretch(1);

    QObject::connect(createSessionButton, &QPushButton::clicked, this, &DebuggerWidget::slot_debuggerwidget_createSession);
    QObject::connect(m_joinSessionButton, &QPushButton::clicked, this, &DebuggerWidget::slot_debuggerwidget_joinSession);

    refresh(); // populate the dropdown list box
    return sessionSelectorLayout;
}

QVBoxLayout* DebuggerWidget::createDebuggerLayout() {
    plugin::LogInfo("DebuggerWidget::createDebuggerLayout");

    QVBoxLayout* debuggerLayout = new QVBoxLayout(this);

    QString tmpText = QString("Hello Debugger: Session: %1").arg(QString::fromStdString(m_currentSessionName));
    m_insightDebuggerLabel = new QLabel(tmpText, this);
    m_insightDebuggerLabel->setAlignment(Qt::AlignCenter);
    debuggerLayout->addWidget(m_insightDebuggerLabel);

    return debuggerLayout;
}

void DebuggerWidget::refresh() {
    plugin::LogInfo("DebuggerWidget::refresh -> Refreshing Widget %s", m_widgetBVName.c_str());

    // Check the state of the widget. active = in debugger UI / inactive = in session selector UI
    if (m_sessionActive) {
        // Debugger layout is active
        auto session = ServiceLocator::getInstance().getService<SessionManager>()->getSession(m_currentSessionName);
        if (!session) {
            // This means we have joined a session that no longer exists so therefore reset the layout
            plugin::LogInfo("DebuggerWidget::refresh -> Session not found: %s", m_currentSessionName.c_str());
            // Switch to session layout
            m_stackedWidget->setCurrentIndex(static_cast<int>(WidgetState::kSessionSelector));
            m_sessionActive = false;
            refresh(); // recursive call back to refresh the switched session view state
        }

        QString tmpText = QString("Hello Debugger:\nSession: %1").arg(QString::fromStdString(m_currentSessionName));
        m_insightDebuggerLabel->setText(tmpText);
    } else {
        // Session selector layout is active

        // Populate possible sessions to join
        m_joinSessionButton->setEnabled(true);
        m_sessionDropdown->setEnabled(true);
        SessionManager* sm = ServiceLocator::getInstance().getService<SessionManager>();
        m_sessionDropdown->clear();
        for (const auto& session : sm->getSessions()) {
            m_sessionDropdown->addItem(QString::fromStdString(session->getName()));
        }
        if (sm->getSessions().empty()) {
            m_joinSessionButton->setEnabled(false);
            m_sessionDropdown->setEnabled(false);
        }
    }
}

void DebuggerWidget::slot_debuggerwidget_createSession() {

    plugin::LogInfo("DebuggerWidget::slot_debuggerwidget_createSession: %s", m_widgetBVName.c_str());
    SessionManager* sm = ServiceLocator::getInstance().getService<SessionManager>();
    sm->createSession(m_widgetBVName, m_bv);
    m_currentSessionName = m_widgetBVName; // We are the main session, so the session is the name of this binary view

    // Switch to debugger layout
    m_stackedWidget->setCurrentIndex(static_cast<int>(WidgetState::kDebugger));
    m_sessionActive = true;

    // Trigger a refresh, we want all the other widgets to know this one is available to connect to
    ServiceLocator::getInstance().getService<DebuggerWidgetManager>()->refresh();
}

void DebuggerWidget::slot_debuggerwidget_joinSession() {

    QString selectedSession = m_sessionDropdown->currentText();
    plugin::LogInfo("DebuggerWidget::slot_debuggerwidget_joinSession -> %s", selectedSession.toStdString().c_str());
    m_currentSessionName = selectedSession.toStdString(); // The current session name will be the target of the one to join

    // Switch to debugger layout
    m_stackedWidget->setCurrentIndex(static_cast<int>(WidgetState::kDebugger));
    m_sessionActive = true;

    // Trigger a refresh, we want all the other widgets to know this one is available to connect to
    ServiceLocator::getInstance().getService<DebuggerWidgetManager>()->refresh();
}
