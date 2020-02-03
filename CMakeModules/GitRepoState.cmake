function(git_get_sha _shavar)
	execute_process(COMMAND
		"git"
		"rev-parse"
		"--short"
		"HEAD"
		WORKING_DIRECTORY
		"${CMAKE_CURRENT_SOURCE_DIR}"
		RESULT_VARIABLE
		res
		OUTPUT_VARIABLE
		out
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	if(NOT res EQUAL 0)
		set(out "UNKNOWN")
	endif()

	set(${_shavar} "${out}" PARENT_SCOPE)
endfunction()

function(git_get_state _localstatevar)
	execute_process(COMMAND
		"git"
		"diff-state"
		"--quiet"
		"HEAD"
		WORKING_DIRECTORY
		"${CMAKE_CURRENT_SOURCE_DIR}"
		RESULT_VARIABLE
		res
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	if(res EQUAL 0)
		set(${_localstatevar} "" PARENT_SCOPE)
	else()
		set(${_localstatevar} "-dirty" PARENT_SCOPE)
	endif()
endfunction()
