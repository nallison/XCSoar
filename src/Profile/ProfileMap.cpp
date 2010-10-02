/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009

	M Roberts (original release)
	Robin Birch <robinb@ruffnready.co.uk>
	Samuel Gisiger <samuel.gisiger@triadis.ch>
	Jeff Goodenough <jeff@enborne.f2s.com>
	Alastair Harrison <aharrison@magic.force9.co.uk>
	Scott Penrose <scottp@dd.com.au>
	John Wharington <jwharington@gmail.com>
	Lars H <lars_hn@hotmail.com>
	Rob Dunning <rob@raspberryridgesheepfarm.com>
	Russell King <rmk@arm.linux.org.uk>
	Paolo Ventafridda <coolwind@email.it>
	Tobias Lohner <tobias@lohner-net.de>
	Mirek Jezek <mjezek@ipplc.cz>
	Max Kellermann <max@duempel.org>
	Tobias Bieniek <tobias.bieniek@gmx.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Profile/ProfileMap.hpp"
#include "Profile/ProfileKeys.hpp"
#include "StringUtil.hpp"
#include "LogFile.hpp"
#include "IO/FileLineReader.hpp"
#include "IO/TextWriter.hpp"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

std::map<tstring, tstring> ProfileMap::map_str;
std::map<tstring, DWORD> ProfileMap::map_num;

bool
ProfileMap::Get(const TCHAR *szRegValue, DWORD &pPos)
{
  std::map<tstring, DWORD>::iterator it;
  it = map_num.find(szRegValue);
  if (it == map_num.end())
    return false;

  pPos = it->second;
  return true;
}

bool
ProfileMap::Set(const TCHAR *szRegValue, DWORD Pos)
{
  map_num[szRegValue] = Pos;
  return true;
}

/**
 * Reads a value from the profile map
 * @param szRegValue Name of the value that should be read
 * @param pPos Pointer to the output buffer
 * @param dwSize Maximum size of the output buffer
 */
bool
ProfileMap::Get(const TCHAR *szRegValue, TCHAR *pPos, size_t dwSize)
{
  std::map<tstring, tstring>::iterator it;
  it = map_str.find(szRegValue);
  if (it == map_str.end())
    return false;

  _tcsncpy(pPos, it->second.c_str(), dwSize);
  return true;
}

/**
 * Writes a value to the profile map
 * @param szRegValue Name of the value that should be written
 * @param Pos Value that should be written
 */
bool
ProfileMap::Set(const TCHAR *szRegValue, const TCHAR *Pos)
{
  map_str[szRegValue] = Pos;
  return true;
}

void
ProfileMap::Import(const TCHAR *szFile)
{
  if (string_is_empty(szFile))
    return;

  LogStartUp(_T("Loading profile map from %s"), szFile);
  FileLineReader reader(szFile);
  if (reader.error())
    return;

  TCHAR *line;
  while ((line = reader.read()) != NULL) {
    if (string_is_empty(line) || *line == _T('#'))
      continue;

    TCHAR *p = _tcschr(line, _T('='));
    if (p == line || p == NULL)
      continue;

    *p = _T('\0');
    TCHAR *value = p + 1;

#ifdef PROFILE_KEY_PREFIX
    TCHAR key[sizeof(PROFILE_KEY_PREFIX) + _tcslen(line)];
    _tcscpy(key, PROFILE_KEY_PREFIX);
    _tcscat(key, line);
#else
    const TCHAR *key = line;
#endif

    if (*value == _T('"')) {
      ++value;
      p = _tcschr(value, _T('"'));
      if (p == NULL)
        continue;

      *p = _T('\0');

      Set(key, value);
    } else {
      long l = _tcstol(value, &p, 10);
      if (p > value)
        Set(key, l);
    }
  }
}

void
ProfileMap::Export(const TCHAR *szFile)
{
  // If no file is given -> return
  if (string_is_empty(szFile))
    return;

  // Try to open the file for writing
  TextWriter writer(szFile);
  // ... on error -> return
  if (writer.error())
    return;

  // Iterate through the profile maps
  std::map<tstring, DWORD>::iterator it_num;
  for (it_num = map_num.begin(); it_num != map_num.end(); it_num++)
    writer.printfln(_T("%s=%d"), it_num->first.c_str(), it_num->second);

  std::map<tstring, tstring>::iterator it_str;
  for (it_str = map_str.begin(); it_str != map_str.end(); it_str++) {
    // If the value string is not empty
    if (!string_is_empty(it_str->second.c_str()))
      // -> write the value to the output file
      writer.printfln(_T("%s=\"%s\""), it_str->first.c_str(),
                      it_str->second.c_str());
    else
      // otherwise -> write ="" to the output file
      writer.printfln(_T("%s=\"\""), it_str->first.c_str());
  }
}
