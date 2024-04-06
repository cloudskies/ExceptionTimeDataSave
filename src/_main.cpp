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
    //our besties
    if (ExcCode == DBG_CONTROL_C) return EXCEPTION_EXECUTE_HANDLER;//=0x40010005, "
	if (ExcCode == EXCEPTION_GUARD_PAGE_VIOLATION) return EXCEPTION_EXECUTE_HANDLER;//=0x80000001, "
	if (ExcCode == EXCEPTION_DATATYPE_MISALIGNMENT) return EXCEPTION_EXECUTE_HANDLER;//=0x80000002, "
	if (ExcCode == EXCEPTION_BREAKPOINT) return EXCEPTION_EXECUTE_HANDLER;//=0x80000003, "
	if (ExcCode == EXCEPTION_SINGLE_STEP) return EXCEPTION_EXECUTE_HANDLER;//=0x80000004, "
	if (ExcCode == EXCEPTION_ACCESS_VIOLATION) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000005, "
	if (ExcCode == EXCEPTION_IN_PAGE_ERROR) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000006, "
	if (ExcCode == EXCEPTION_INVALID_HANDLE) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000008, "
	if (ExcCode == EXCEPTION_ILLEGAL_INSTRUCTION) return EXCEPTION_EXECUTE_HANDLER;//=0xc000001d, "
	if (ExcCode == EXCEPTION_NONCONTINUABLE_EXCEPTION) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000025, "
	if (ExcCode == EXCEPTION_INVALID_DISPOSITION) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000026, "
	if (ExcCode == EXCEPTION_ARRAY_BOUNDS_EXCEEDED) return EXCEPTION_EXECUTE_HANDLER;//=0xc000008c, "
	if (ExcCode == EXCEPTION_FLOAT_DENORMAL_OPERAND) return EXCEPTION_EXECUTE_HANDLER;//=0xc000008d, "
	if (ExcCode == EXCEPTION_FLOAT_DIVIDE_BY_ZERO) return EXCEPTION_EXECUTE_HANDLER;//=0xc000008e, "
	if (ExcCode == EXCEPTION_FLOAT_INEXACT_RESULT) return EXCEPTION_EXECUTE_HANDLER;//=0xc000008f, "
	if (ExcCode == EXCEPTION_FLOAT_INVALID_OPERATION) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000090, "
	if (ExcCode == EXCEPTION_FLOAT_OVERFLOW) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000091, "
	if (ExcCode == EXCEPTION_FLOAT_STACK_CHECK) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000092, "
	if (ExcCode == EXCEPTION_FLOAT_UNDERFLOW) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000093, "
	if (ExcCode == EXCEPTION_INTEGER_DIVIDE_BY_ZERO) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000094, "
	if (ExcCode == EXCEPTION_INTEGER_OVERFLOW) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000095, "
	if (ExcCode == EXCEPTION_PRIVILEGED_INSTRUCTION) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000096, "
	if (ExcCode == EXCEPTION_STACK_OVERFLOW) return EXCEPTION_EXECUTE_HANDLER;//=0xc00000fd, "
	if (ExcCode == EXCEPTION_POSSIBLE_DEADLOCK) return EXCEPTION_EXECUTE_HANDLER;//=0xc0000194
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