isEmpty(SPARKLE_PATH) {
    error(You have to set SPARKLE_PATH to the path of QPersistence relative to your project)
}

SPARKLE_INCLUDEPATH     = $$PWD/Sparkle.framework/Headers
SPARKLE_LIBS            = -framework AppKit -framework Sparkle
SPARKLE_LFLAGS          = -F$$PWD
SPARKLE_FRAMEWORK.files = $$PWD/Sparkle.framework
SPARKLE_FRAMEWORK.path  = Contents/Frameworks
