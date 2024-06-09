#include <Geode/Geode.hpp>
using namespace geode::prelude;

void SaveMethods() {
    AppDelegate::sharedApplication()->trySaveGame(false);
    log::info("{}", "Game saved via AppDelegate::trySaveGame().");
    if (LevelEditorLayer* pLevelEditorLayer = GameManager::sharedState()->getEditorLayer()) {
        pLevelEditorLayer->m_editorUI->undoLastAction(pLevelEditorLayer->m_editorUI);
        pLevelEditorLayer->m_editorUI->undoLastAction(pLevelEditorLayer->m_editorUI);
        pLevelEditorLayer->m_editorUI->undoLastAction(pLevelEditorLayer->m_editorUI);
        log::info("{}", "Was performed 3 undo actions in current level editor layer.");
        EditorPauseLayer::create(LevelEditorLayer_)->saveLevel();
        log::info("{}", "Current level saved.");
    }
}

BOOL WINAPI ConsoleHandler(DWORD CEvent) {
    switch (CEvent) {
    case CTRL_CLOSE_EVENT:
        SaveMethods();
        break;
    case CTRL_SHUTDOWN_EVENT:
        SaveMethods();
        break;
    }
    return TRUE;
}

DWORD LastExcCode;

LONG WINAPI VectoredExceptionHandler(_EXCEPTION_POINTERS* pExceptInfo) {
    DWORD ExcCode = pExceptInfo->ExceptionRecord->ExceptionCode;
    std::string sExcCode = (std::stringstream() << "0x" << std::hex << ExcCode).str();
    log::error("{} with {}!", __FUNCTION__, sExcCode);
    SaveMethods();
    if (ExcCode == DBG_CONTROL_C) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_DATATYPE_MISALIGNMENT) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_BREAKPOINT) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_SINGLE_STEP) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_ACCESS_VIOLATION) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_IN_PAGE_ERROR) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_INVALID_HANDLE) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_ILLEGAL_INSTRUCTION) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_NONCONTINUABLE_EXCEPTION) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_INVALID_DISPOSITION) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_ARRAY_BOUNDS_EXCEEDED) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == EXCEPTION_STACK_OVERFLOW) return EXCEPTION_EXECUTE_HANDLER;
    if (ExcCode == LastExcCode) return EXCEPTION_CONTINUE_EXECUTION;
    LastExcCode = ExcCode;
    return EXCEPTION_CONTINUE_SEARCH;
}

$on_mod(Loaded) {
    AddVectoredExceptionHandler(0, VectoredExceptionHandler);
    log::debug("{}", "Added VectoredExceptionHandler.");
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE) {
        log::error("{}", "Can't install ConsoleHandler!");
    }
}
