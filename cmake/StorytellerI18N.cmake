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


# params:
# [STRTLR_TARGET_NAME] - name of the target (e.g. StorytellerEditor)
# [STRTLR_PO_PREFIX] - name of the .po/.pot file (e.g. Storyteller)
# [STRTLR_TARGET_PREFIX_DIR] - prefix directory (e.g. ${CMAKE_CURRENT_SOURCE_DIR}
# [STRTLR_FOLDER] - folder to place created targets (e.g. Storyteller/Editor)
# [STRTLR_SOURCE_FILES] - files provided to xgettext (e.g. ${SOURCE_FILES})
function(CreateTranslationHelperTargets STRTLR_TARGET_NAME STRTLR_PO_PREFIX STRTLR_TARGET_PREFIX_DIR STRTLR_FOLDER STRTLR_SOURCE_FILES)
	if(StorytellerI18N_OK)
		# Update stage
		add_custom_target(${STRTLR_TARGET_NAME}_pot_update
			COMMENT "${STRTLR_PO_PREFIX}.pot file update: Done!"
			DEPENDS ${STRTLR_TARGET_PREFIX_DIR}/locale/${STRTLR_PO_PREFIX}.pot
		)
		set_property(TARGET ${STRTLR_TARGET_NAME}_pot_update APPEND PROPERTY FOLDER ${STRTLR_FOLDER})
		
		add_custom_command(TARGET ${STRTLR_TARGET_NAME}_pot_update
			PRE_BUILD
			COMMAND
				${GETTEXT_XGETTEXT_EXECUTABLE}
				--from-code=utf-8
				--c++
				--force-po
				--output=${STRTLR_TARGET_PREFIX_DIR}/locale/${STRTLR_PO_PREFIX}.pot
				--keyword="Translate:2,2t"
				--keyword="Translate:2,3,4t"
				--keyword="TranslateCtx:2,3c,3t"
				--keyword="TranslateCtx:2,3,5t,5t"
				--keyword="TranslateDefer:2,2t"
				--keyword="TranslateDefer:2,3,4t"
				--keyword="TranslateCtxDefer:2,3c,3t"
				--keyword="TranslateCtxDefer:2,3,5t,5t"
				${STRTLR_SOURCE_FILES}
			WORKING_DIRECTORY ${STRTLR_TARGET_PREFIX_DIR}
			COMMENT "${STRTLR_PO_PREFIX}.pot file generated: ${STRTLR_TARGET_PREFIX_DIR}/locale/${STRTLR_PO_PREFIX}.pot"
		)
		
		# Merge stage
		add_custom_target(${STRTLR_TARGET_NAME}_pot_merge
			COMMENT "${STRTLR_PO_PREFIX}.pot file merge: Done!"
			DEPENDS ${STRTLR_TARGET_PREFIX_DIR}/locale/${STRTLR_PO_PREFIX}.pot
		)
		set_property(TARGET ${STRTLR_TARGET_NAME}_pot_merge APPEND PROPERTY FOLDER ${STRTLR_FOLDER})
		
		file(GLOB_RECURSE PO_FILES ${STRTLR_TARGET_PREFIX_DIR}/locale/*/${STRTLR_PO_PREFIX}.po)
		message(STATUS " ${STRTLR_TARGET_NAME} PO_FILES: ${PO_FILES}")
		
		foreach(PO_FILE IN ITEMS ${PO_FILES})
			message(STATUS " Adding msgmerge for: ${PO_FILE}")
			add_custom_command(TARGET ${STRTLR_TARGET_NAME}_pot_merge
				PRE_BUILD
				COMMAND
					${GETTEXT_MSGMERGE_EXECUTABLE} ${PO_FILE}
					${STRTLR_TARGET_PREFIX_DIR}/locale/${STRTLR_PO_PREFIX}.pot
				COMMENT "${STRTLR_PO_PREFIX}.pot file merge: ${PO_FILE}"
			)
		endforeach()
		
		
		# Initialization
		add_custom_target(${STRTLR_TARGET_NAME}_po_init
			COMMENT "${STRTLR_PO_PREFIX}.po files initialization: Done! RERUN CMAKE AGAIN!"
			DEPENDS ${STRTLR_TARGET_PREFIX_DIR}/locale/${STRTLR_PO_PREFIX}.pot
		)
		set_property(TARGET ${STRTLR_TARGET_NAME}_po_init APPEND PROPERTY FOLDER ${STRTLR_FOLDER})
		
		file(GLOB PO_LANGS LIST DIRECTORIES TRUE ${STRTLR_TARGET_PREFIX_DIR}/locale/*)
		foreach(PO_LANG_DIR IN ITEMS ${PO_LANGS})
			if(IS_DIRECTORY ${PO_LANG_DIR})
				cmake_path(GET PO_LANG_DIR STEM LANG_NAME)
				list(APPEND LANG_NAMES ${LANG_NAME})
			endif()
		endforeach()
		
		foreach(LANG_NAME IN LISTS LANG_NAMES)
			if(NOT EXISTS ${STRTLR_TARGET_PREFIX_DIR}/locale/${LANG_NAME}/LC_MESSAGES/${STRTLR_PO_PREFIX}.po)
				add_custom_command(TARGET ${STRTLR_TARGET_NAME}_po_init
					PRE_BUILD
					COMMAND
						${GETTEXT_MSGINIT_EXECUTABLE}
						--input=${STRTLR_TARGET_PREFIX_DIR}/locale/${STRTLR_PO_PREFIX}.pot
						--output-file=${STRTLR_TARGET_PREFIX_DIR}/locale/${LANG_NAME}/LC_MESSAGES/${STRTLR_PO_PREFIX}.po
						--locale=${LANG_NAME}
					WORKING_DIRECTORY ${STRTLR_TARGET_PREFIX_DIR}
				)
			endif()
		endforeach()
		
		
		# Msgfmt stage
		file(GLOB PO_LANGS LIST DIRECTORIES TRUE ${STRTLR_TARGET_PREFIX_DIR}/locale/*/*)
		message(STATUS " ${STRTLR_TARGET_NAME} PO_LANGS: ${PO_LANGS}")
		
		add_custom_target(${STRTLR_TARGET_NAME}_po_compile
			COMMENT "${STRTLR_PO_PREFIX}.po compilation: Done!"
		)
		set_property(TARGET ${STRTLR_TARGET_NAME}_po_compile APPEND PROPERTY FOLDER ${STRTLR_FOLDER})
		
		foreach(PO_LANG IN ITEMS ${PO_LANGS})
			if(IS_DIRECTORY ${PO_LANG})
				message(STATUS " Adding msgfmt for: ${PO_LANG}")
				add_custom_command(TARGET ${STRTLR_TARGET_NAME}_po_compile
					PRE_BUILD
					COMMAND
						${GETTEXT_MSGFMT_EXECUTABLE}
						--output-file=${STRTLR_PO_PREFIX}.mo
						${STRTLR_PO_PREFIX}.po
					WORKING_DIRECTORY ${PO_LANG}
					COMMENT "${STRTLR_PO_PREFIX}.po compilation: ${PO_LANG}"
				)
				endif()
		endforeach()
	endif()
endfunction()