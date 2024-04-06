#include <Geode/Geode.hpp>
using namespace geode::prelude;

void SaveMethods() {
    //save game?
    AppDelegate::sharedApplication()->trySaveGame(false);
	//save log why no
    log::info("{}", "Game saved via trySaveGame()");
    //level editor tries
    if (LevelEditorLayer* LevelEditorLayer_ = GameManager::sharedState()->getEditorLayer()) {
        EditorPauseLayer::create(LevelEditorLayer_)->saveLevel();
        log::info("{}", "Current level saved");
    }
}

BOOL WINAPI ConsoleHandler(DWORD CEvent) {
    //save game at some events
    switch(CEvent) {
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
    log::error("{} with {}", __FUNCTION__, sExcCode);
    //save game
    SaveMethods();
    //error msg
    if (ExcCode == EXCEPTION_NONCONTINUABLE_EXCEPTION) {
        CCMessageBox(fmt::format("{}", sExcCode), "EXCEPTION_NONCONTINUABLE_EXCEPTION");
        }
    if (ExcCode != EXCEPTION_NONCONTINUABLE_EXCEPTION) return EXCEPTION_CONTINUE_EXECUTION;
    LastExcCode = ExcCode;
    return EXCEPTION_CONTINUE_SEARCH;
}

$on_mod(Loaded) {
    //VectoredExceptionHandler
	AddVectoredExceptionHandler(1, VectoredExceptionHandler);
    //ConsoleHandler for idk
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE) {//c cast???!!((
        log::error("{}", "Can't install console handler!");
    }
}