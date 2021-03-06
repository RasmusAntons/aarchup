#include "CliWrapper.hh"

using namespace std;

CliWrapper::CliWrapper(const char *cliCommand) {
  this->_cliCommand = cliCommand;
}

CliWrapper::~CliWrapper() = default;

string CliWrapper::execute() {
  shared_ptr<FILE> pipe(popen(CliWrapper::_cliCommand, "r"), pclose);
  if (!pipe) {
    std::stringstream ss;
    ss << "Failed to execute command " << _cliCommand;
    throw std::runtime_error(ss.str());
  }
  return parseOutput(pipe);
}

string CliWrapper::parseOutput(const shared_ptr<FILE> &pipe) const {
  array<char, 128> buffer = {0};
  string result;
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
      result += buffer.data();
    }
  }
  return result;
}
