QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    lib/abstracthighlighter.cpp \
    lib/ansihighlighter.cpp \
    lib/context.cpp \
    lib/contextswitch.cpp \
    lib/definition.cpp \
    lib/definitiondownloader.cpp \
    lib/foldingregion.cpp \
    lib/format.cpp \
    lib/highlightingdata.cpp \
    lib/htmlhighlighter.cpp \
    lib/keywordlist.cpp \
    lib/ksyntaxhighlighting_logging.cpp \
    lib/repository.cpp \
    lib/rule.cpp \
    lib/state.cpp \
    lib/syntaxhighlighter.cpp \
    lib/theme.cpp \
    lib/themedata.cpp \
    lib/wildcardmatcher.cpp \
    lib/worddelimiters.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    codeeditor.h \
    lib/abstracthighlighter.h \
    lib/abstracthighlighter_p.h \
    lib/ansihighlighter.h \
    lib/context_p.h \
    lib/contextswitch_p.h \
    lib/definition.h \
    lib/definition_p.h \
    lib/definitiondownloader.h \
    lib/definitionref_p.h \
    lib/foldingregion.h \
    lib/format.h \
    lib/format_p.h \
    lib/highlightingdata_p.hpp \
    lib/htmlhighlighter.h \
    lib/keywordlist_p.h \
    lib/ksyntaxhighlighting_export.h \
    lib/ksyntaxhighlighting_logging.h \
    lib/ksyntaxhighlighting_version.h \
    lib/matchresult_p.h \
    lib/repository.h \
    lib/repository_p.h \
    lib/rule_p.h \
    lib/state.h \
    lib/state_p.h \
    lib/syntaxhighlighter.h \
    lib/textstyledata_p.h \
    lib/theme.h \
    lib/themedata_p.h \
    lib/wildcardmatcher.h \
    lib/worddelimiters_p.h \
    lib/xml_p.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data/themes/theme-data.qrc
