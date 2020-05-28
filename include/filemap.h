/**
 * @Author: Lev Vorobjev
 * @Date:   12.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: filemap.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 12.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#ifndef FILEMAP_H
#define FILEMAP_H

#include <string>
#include <boost/filesystem.hpp>

namespace sii {
    using namespace std;
    using boost::filesystem::path;

    template <typename T>
    wstring read_file_multi_byte(const T &filename);

    wstring read_file_multi_byte(const path &p);

    template <typename T>
    wstring read_file_multi_byte(const T &filename) {
        path p(filename);
        return read_file_multi_byte(p);
    }
}

#endif
