cache()

TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS = QPersistence lib/libQImageMetaData src

QPersistence.subdir = lib/QPersistence
src.subdir = src
src.depends = QPersistence

OTHER_FILES = README.md

DEPLOY_TARGET.target    = deploy
DEPLOY_TARGET.commands  = cd src && make deploy
QMAKE_EXTRA_TARGETS += DEPLOY_TARGET

BREAKPAD_DUMP_SYMBOLS.target    = dump_syms
BREAKPAD_DUMP_SYMBOLS.commands  = cd src && make dump_syms
QMAKE_EXTRA_TARGETS += BREAKPAD_DUMP_SYMBOLS
