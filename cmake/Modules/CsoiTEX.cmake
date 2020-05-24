find_package(LATEX COMPONENTS PDFLATEX)
include(CsoiDoc)
function(add_tex _target)
    get_filename_component(BASENAME ${ARGV1} NAME_WE)
    set(SOURCES ${ARGN})
    list(FILTER SOURCES INCLUDE REGEX ".+\\.[am]d$")
    list(TRANSFORM ARGN REPLACE "\\.[am]d$" ".tex")
    string(REGEX MATCH "\\.[am]d$" _match ${ARGV1})
	if (DEFINED _match)
		set(_main TRUE)
        set(BIBFILES ${ARGN})
        list(FILTER BIBFILES INCLUDE REGEX "\\.bib$")
        list(LENGTH BIBFILES BIBCOUNT)
        if(BIBCOUNT GREATER 0)
            list(GET BIBFILES 0 BIBFILE)
            set(_main ${BIBFILE})
        endif()
	else()
		set(_main FALSE)
	endif()
    foreach (_source IN LISTS SOURCES)
        add_doc(${_source} ${_main})
        set(_main FALSE)
    endforeach ()
    list(GET ARGN 0 TEX_MAIN)
    set(TEX_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}.pdf)
    set(TEX_PREAMBLE ${CMAKE_CURRENT_LIST_DIR}/preamble.tex)
    set(TEX_FORMAT ${CMAKE_CURRENT_BINARY_DIR}/preamble.fmt)
    set(TEX_FLAGS -interaction=nonstopmode -shell-escape -job-name="${_target}")
    add_custom_command(OUTPUT ${TEX_FORMAT}
            COMMAND pdftex -shell-escape -ini -output-directory=${CMAKE_CURRENT_BINARY_DIR}
            -job-name="preamble" "&pdflatex ${TEX_PREAMBLE}\\dump" ${TEX_PREAMBLE}
            DEPENDS ${TEX_PREAMBLE} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} VERBATIM)
    set(AUX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${_target}.aux)
    set(LOG_FILE ${CMAKE_CURRENT_BINARY_DIR}/${_target}.log)
    add_custom_command(OUTPUT ${TEX_OUTPUT}
            COMMAND ${PDFLATEX_COMPILER} ${TEX_FLAGS}
            -aux-directory=${CMAKE_CURRENT_BINARY_DIR}
            -output-directory=${CMAKE_CURRENT_BINARY_DIR}
            "&preamble ${TEX_MAIN}"
            COMMAND find "undefined references" < ${LOG_FILE} && 
            ${PDFLATEX_COMPILER} ${TEX_FLAGS}
            -aux-directory=${CMAKE_CURRENT_BINARY_DIR}
            -output-directory=${CMAKE_CURRENT_BINARY_DIR}
            "&preamble ${TEX_MAIN}" || set ERRORLEVEL=0
            DEPENDS ${TEX_FORMAT} ${ARGN} BYPRODUCTS ${AUX_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} VERBATIM)
    if (BIBCOUNT GREATER 0)
        set(BIB_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_target}.bbl)
        string(REGEX REPLACE "(\\/)" "\\\\\\1" BIB_RELATIVE ${CMAKE_CURRENT_LIST_DIR})
        add_custom_command(OUTPUT ${BIB_OUTPUT} DEPENDS ${TEX_FORMAT} ${ARGN}
                COMMAND ${PDFLATEX_COMPILER} ${TEX_FLAGS}
                -include-directory=${CMAKE_CURRENT_LIST_DIR}
                "&preamble ${CMAKE_CURRENT_LIST_DIR}/${TEX_MAIN}"
                COMMAND perl -i.bak -pe "s/(\\\\bibdata\\{)(\\w+\\})/$1${BIB_RELATIVE}\\/$2/" ${_target}.aux &&
                bibtex8 -B -c utf8cyrillic.csf ${_target}
                COMMAND del ${BASENAME}.pdf
                BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${_target}.blg
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} VERBATIM)
    endif ()
    add_custom_target(${_target} DEPENDS ${BIB_OUTPUT} ${TEX_OUTPUT} SOURCES ${ARGN})
endfunction()