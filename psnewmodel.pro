TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS = QPersistence src

QPersistence.subdir = lib/QPersistence
src.subdir = src
src.depends = QPersistence

OTHER_FILES = README.md
