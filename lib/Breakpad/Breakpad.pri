isEmpty(BREAKPAD_PATH) {
    error(You have to set BREAKPAD_PATH to the path of QPersistence relative to your project)
}

BREAKPAD_INCLUDEPATH     = $$PWD/Breakpad.framework/Headers
BREAKPAD_LIBS            = -framework Breakpad
BREAKPAD_LFLAGS          = -F$$PWD
BREAKPAD_FRAMEWORK.files = $$PWD/Breakpad.framework
BREAKPAD_FRAMEWORK.path  = Contents/Frameworks

BREAKPAD_SYMBOLPATH      = $$PWD/symbols
BREAKPAD_DUMP_SYM        = $$PWD/bin/dump_syms_osx
