#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <string>
#include <vector>

/**
 * @brief Simple class that can read and manage CLI arguments
 */

class InputParser
{
public:
  typedef std::vector< std::string > TokenList;

public:
  InputParser(int& argc, char** argv);
  virtual ~InputParser();

  int getCmdOption(const std::string& option, TokenList& value) const;
  bool cmdOptionExists(const std::string& option) const;

private:
  TokenList m_tokens;
};

#endif // INPUTPARSER_H
