#include "input_parser.h"

#include <algorithm>
#include <iostream>

InputParser::
InputParser(int& argc, char** argv)
{
  for(int i=1; i<argc; ++i)
    m_tokens.push_back(std::string(argv[i]));
}

InputParser::~InputParser()
{}

int
InputParser::getCmdOption(const std::string &option, std::vector<std::string> &value) const
{
  std::vector<std::string>::const_iterator itr;
  itr =  std::find(m_tokens.begin(), m_tokens.end(), option);

  if(itr == m_tokens.end() || ++itr== m_tokens.end())
    return 0;

  value.clear();

  do{

    std::string next(*itr);
    if(next.find_first_of("-") == 0)
      break;
    else
      value.push_back(next);

  } while(++itr != m_tokens.end());

  return value.size();
}

bool
InputParser::
cmdOptionExists(const std::string &option) const
{
  return std::find(m_tokens.begin(), m_tokens.end(), option) != m_tokens.end();
}


