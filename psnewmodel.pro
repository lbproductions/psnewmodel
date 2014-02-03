cache()

TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS = QPersistence src

QPersistence.subdir = lib/QPersistence
src.subdir = src
src.depends = QPersistence

OTHER_FILES = README.md

DEPLOY_TARGET.target    = deploy
DEPLOY_TARGET.commands  = cd src && make deploy
QMAKE_EXTRA_TARGETS += DEPLOY_TARGET
