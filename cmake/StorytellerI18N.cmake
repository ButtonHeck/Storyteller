file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/locale/en_EN/LC_MESSAGES)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/locale/ru_RU/LC_MESSAGES)

set(StorytellerI18N_OK TRUE)

find_package(Gettext REQUIRED)

find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
find_program(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)
find_program(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
find_program(GETTEXT_MSGINIT_EXECUTABLE msginit)

if(Gettext_NOTFOUND)
	set(StorytellerI18N_OK FALSE)
	message(WARNING "Gettext package not found!")
endif()

if(GETTEXT_XGETTEXT_EXECUTABLE)
	message(STATUS "Found xgettext: ${GETTEXT_XGETTEXT_EXECUTABLE}")
else()
	set(StorytellerI18N_OK FALSE)
	message(WARNING "xgettext not found!")
endif()

if(GETTEXT_MSGMERGE_EXECUTABLE)
	message(STATUS "Found msgmerge: ${GETTEXT_MSGMERGE_EXECUTABLE}")
else()
	set(StorytellerI18N_OK FALSE)
	message(WARNING "msgmerge not found!")
endif()

if(GETTEXT_MSGFMT_EXECUTABLE)
	message(STATUS "Found msgfmt: ${GETTEXT_MSGFMT_EXECUTABLE}")
else()
	set(StorytellerI18N_OK FALSE)
	message(WARNING "msgfmt not found!")
endif()

if(GETTEXT_MSGINIT_EXECUTABLE)
	message(STATUS "Found msginit: ${GETTEXT_MSGINIT_EXECUTABLE}")
else()
	set(StorytellerI18N_OK FALSE)
	message(WARNING "msginit not found!")
endif()