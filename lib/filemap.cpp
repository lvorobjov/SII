/**
 * @Author: Lev Vorobjev
 * @Date:   12.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: filemap.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 13.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include "filemap.h"
#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <codecvt>
#include <sstream>
using namespace sii;
using boost::iostreams::mapped_file_source;
using boost::iostreams::code_converter;
using wmapped_file_source = code_converter<mapped_file_source, codecvt_utf8_utf16<wchar_t>>;

wstring read_file_multi_byte(const path &p) {
    mapped_file_source source(p);
    wmapped_file_source wsource(source);
    wstringstream out;
    int const size = 512;
    wchar_t buf[size];
    while (int count = wsource.read(buf, size))
        out << wstring(buf, buf+count);
    return out.str();
}
