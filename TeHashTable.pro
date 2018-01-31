TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    hash_functions.cpp

HEADERS += \
    hashtable.hpp \
    hash_functions.hpp \
    array_list.hpp \
    singly_linked_list.hpp \
    point.hpp
