//
// Harano Aji Fonts generator
// https://github.com/trueroad/HaranoAjiFonts-generator
//
// conv_cmap_main.cc:
//   convert cmap.ttx
//
// Copyright (C) 2019 Masamichi Hosoda.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//

#include <exception>
#include <iostream>

#include <pugixml.hpp>

#include "walker_cmap.hh"
#include "version.hh"

int main (int argc, char *argv[])
{
  std::cerr
    << "conv_cmap: Harano Aji Fonts generator " << version
    << std::endl
    << "(convert cmap.ttx)"
    << std::endl
    << "Copyright (C) 2019 Masamichi Hosoda" << std::endl
    << "https://github.com/trueroad/HaranoAjiFonts-generator" << std::endl
    << std::endl;

  if (argc != 3)
    {
      std::cerr
        << "usage: conv_cmap TABLE.TBL FONT._c_m_a_p.ttx > cmap.ttx"
        << std::endl
        << std::endl
        << "     TABLE.TBL:" << std::endl
        << "         conversion table." << std::endl
        << "     FONT._c_m_a_p.ttx:" << std::endl
        << "         The ttx file of the cmap table extracted" << std::endl
        << "         from the original font file." << std::endl;
      return 1;
    }

  std::cerr
    << "inputs:" << std::endl
    << "    conversion table file      : \"" << argv[1] << "\""
    << std::endl
    << "    original font cmap ttx file: \"" << argv[2] << "\""
    << std::endl << std::endl;

  walker_cmap wcmap;
  try
    {
      wcmap.load (argv[1]);
    }
  catch (std::exception &e)
    {
      std::cerr << "error: walker_cmap::load (): std::exception: "
                << e.what () << std::endl;
      return 1;
    }

  pugi::xml_document doc;

  auto result = doc.load_file (argv[2]);
  if (!result)
    {
      std::cerr << "error: filename \"" << argv[2]
                << "\": " << result.description () << std::endl;
      return 1;
    }

  try
    {
      auto cmap = doc.child ("ttFont").child ("cmap");
      wcmap.walk (cmap, "map");
    }
  catch (std::exception &e)
    {
      std::cerr << "error: walk: std::exception: " << e.what ()
                << std::endl;
      return 1;
    }

  doc.save (std::cout, "  ", pugi::format_default, pugi::encoding_utf8);

  return 0;
}
