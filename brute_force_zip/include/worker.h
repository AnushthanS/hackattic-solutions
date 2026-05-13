#pragma once
#include "password_generator.h"
#include <atomic>
#include <string>
#include <zip.h>

class Worker
{
private:
  long start, end, threadId, passwordLength;
  PasswordConverter passwordHelper;
  std::string &secret;
  std::atomic<bool> &foundFlag;

  zip_t* archive;

  const std::string FILE_NAME = "secret.txt";

  bool test_password(const std::string& password);

public:
  Worker(
      long start,
      long end,
      const int threadId,
      int passwordLength,
      const std::string &charset,
      std::string &secret,
      std::atomic<bool> &foundFlag,
      const std::string& archivePath);
  void run();
  ~Worker();
};
