foreach(srcDir IN LISTS SRC_DIRS)
	message(STATUS "Copying \"${srcDir}\" to \"${DEST_DIR}\"")
	
	file(COPY ${srcDir} DESTINATION ${DEST_DIR}
		PATTERN "*.po" EXCLUDE
		PATTERN "*.pot" EXCLUDE
	)
endforeach()
