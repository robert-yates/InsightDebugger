#ifndef INSIGHTDEBUGGER_SERVICELOCATOR_H
#define INSIGHTDEBUGGER_SERVICELOCATOR_H

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

// Generic service locator wrapper for our plugin global services
class ServiceLocator {
  public:
    template <typename T>
    void provideService(std::unique_ptr<T> service) {
        std::type_index typeIndex = typeid(T);
        // custom deleter needed due to the type erasure needed by the std::unordered_map
        services[typeIndex] = std::unique_ptr<void, std::function<void(void*)>>(
            service.release(), [](void* ptr) { delete static_cast<T*>(ptr); });
    }

    template <typename T>
    T* getService() {
        std::type_index typeIndex = typeid(T);
        auto it = services.find(typeIndex);
        if (it != services.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    static ServiceLocator& getInstance() {
        static ServiceLocator instance;
        return instance;
    }

  private:
    std::unordered_map<std::type_index, std::unique_ptr<void, std::function<void(void*)>>> services;
};

#endif // INSIGHTDEBUGGER_SERVICELOCATOR_H