function(add_doc _source)
    get_filename_component(BASENAME ${_source} NAME_WE)
    set(TEX_FILE ${CMAKE_CURRENT_LIST_DIR}/${BASENAME}.tex)
    set(_main "")
    if (${ARGC} GREATER 1 AND (${ARGV1} OR ${ARGV1} MATCHES "\\.bib$"))
        # Check is bibtex file presented
        set(_bib "")
        if (${ARGV1} MATCHES "\\.bib$")
            get_filename_component(BIBNAME ${ARGV1} NAME_WE)
            set(_bib "print qq(\\n\\\\bibliographystyle{ugost2003}\\n\\\\bibliography{${BIBNAME}});")
        endif ()
        set(_main "BEGIN{print qq(\\\\begin{document}\\n);} \
                   END{${_bib} print qq(\\n\\\\end{document}\\n);}")
    endif()
    add_custom_command(OUTPUT ${TEX_FILE}
            COMMAND perl -MAsciiDoc -C -lpe "${_main} chomp; to_latex($_, qq(\\n));" < ${_source} > ${TEX_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            DEPENDS ${_source} VERBATIM)
endfunction()