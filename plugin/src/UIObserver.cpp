#include "plugin.h"

void UIObserver::OnContextOpen(UIContext* context) {
    plugin::LogInfo("UIObserver::OnContextOpen c:%p", context);
}

void UIObserver::OnContextClose(UIContext* context) {
    plugin::LogInfo("UIObserver::OnContextClose c:%p", context);
}

void UIObserver::OnViewChange(UIContext* context, ViewFrame* frame, const QString& type) {
    BinaryNinja::BinaryView* bv = nullptr;
    if (frame) {
        auto bvRef = frame->getCurrentBinaryView();
        if (bvRef)
            bv = bvRef.GetPtr();
    }
    plugin::LogInfo("UIObserver::OnViewChange c:%p v:%p n:%s bv:%p", context, frame, type.toStdString().c_str(), bv);
}

bool UIObserver::OnBeforeCloseFile(UIContext* context, FileContext* file, ViewFrame* frame) {
    BinaryNinja::BinaryView* bv = nullptr;
    if (frame) {
        auto bvRef = frame->getCurrentBinaryView();
        if (bvRef)
            bv = bvRef.GetPtr();
    }
    plugin::LogInfo("UIObserver::OnBeforeCloseFile c:%p f:%p v:%p bv:%p", context, file, frame, bv);

    return true;
}