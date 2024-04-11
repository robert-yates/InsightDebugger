#include "plugin.h"

SessionManager::SessionManager() {
    plugin::LogInfo("SessionManager::SessionManager");
}

void SessionManager::createSession(const std::string& name, BinaryNinja::Ref<BinaryNinja::BinaryView> bv) {
    std::lock_guard<std::mutex> lock(mtx);
    plugin::LogInfo("SessionManager::createSession n:%s bv:%p", name.c_str(), bv.GetPtr());

    std::unique_ptr<Session> newSession = std::make_unique<Session>(name, bv);
    m_sessions.push_back(std::move(newSession));
}

void SessionManager::removeSession(const std::string& sessionToRemove) {
    std::lock_guard<std::mutex> lock(mtx);
    plugin::LogInfo("SessionManager::removeSession -> request to remove %s", sessionToRemove.c_str());

    auto it = std::remove_if(m_sessions.begin(), m_sessions.end(),
                             [&sessionToRemove](const std::unique_ptr<Session>& session) -> bool {
                                 return session->getName() == sessionToRemove;
                             });

    m_sessions.erase(it, m_sessions.end());
}

tl::expected<std::reference_wrapper<Session>, std::string> SessionManager::getSession(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);
    plugin::LogInfo("SessionManager::getSession -> request to get %s", name.c_str());

    for (const auto& session : m_sessions) {
        if (session->getName() == name) {
            return std::ref(*session);
        }
    }

    return tl::unexpected("Session not found");
}