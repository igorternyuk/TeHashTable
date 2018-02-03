TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    hash_utils.cpp

HEADERS += \
    hashtable.hpp \
    array_list.hpp \
    singly_linked_list.hpp \
    point.hpp \
    hash_utils.hpp
