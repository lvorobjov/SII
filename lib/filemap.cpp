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
#include <boost/iostreams/device/mapped_file.hpp>
using namespace std;
using boost::filesystem::path;
using boost::iostreams::mapped_file_source;

wstring sii::read_file_multi_byte(const path &p) {
    mapped_file_source source(p);
    auto *src = source.data();
    mbstate_t state = mbstate_t();
    size_t size = 1 + mbsrtowcs(nullptr, &src, 0, &state);
    vector<wchar_t> buf(size, 0);
    mbsrtowcs(buf.data(), &src, buf.size(), &state);
    return wstring(buf.begin(), buf.end());
}
