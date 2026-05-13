#include "worker.h"
#include "password_generator.h"
#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <zip.h>

#define BUFFER_SIZE 2048

Worker::Worker(long start, long end, const int threadId, int passwordLength,
               const std::string &charset, std::string &secret,
               std::atomic<bool> &foundFlag, const std::string &archivePath)
    : start(start), end(end), threadId(threadId),
      passwordLength(passwordLength), passwordHelper(charset),
      secret(secret), foundFlag(foundFlag)
{

    int err = 0;
    archive = zip_open(archivePath.c_str(), 0, &err);
    if (!archive)
    {
        zip_error_t zerr;
        zip_error_init_with_code(&zerr, err);
        std::string error = std::string("Failed to open archive: ") + zip_error_strerror(&zerr);

        std::cerr << error << std::endl;
        throw std::runtime_error(error);
    }
}

void Worker::run()
{
    std::string password = passwordHelper.numToPassword(start, passwordLength);

    // check zip password, store in the private std::string password variable
    long c = start;
    while (c++ < end)
    {
        if (foundFlag.load())
            return;

        if (test_password(password))
        {
            std::cout << "Thread: " << threadId << " found password: " << password << std::endl;
            return;
        }

        passwordHelper.incrementPassword(password);
    }
}

bool Worker::test_password(const std::string& password)
{
    zip_file_t* zf = zip_fopen_encrypted(
        archive,
        FILE_NAME.c_str(),
        0,
        password.c_str()
    );

    if (!zf)
    {
        return false;
    }

    char buffer[BUFFER_SIZE];
    zip_int64_t totalRead = 0;
    while (true)
    {
        zip_int64_t bytesRead =
            zip_fread(zf, buffer, sizeof(buffer));

        if (bytesRead < 0)
        {
            zip_fclose(zf);
            return false;
        }

        if (bytesRead == 0)
        {
            break;
        }
        totalRead += bytesRead;
    }

    zip_fclose(zf);

    if (!foundFlag.exchange(true))
    {
        while (totalRead > 0 && (buffer[totalRead-1] == '\n' || buffer[totalRead-1] == '\r'))
            totalRead--;

        buffer[totalRead] = '\0';
        secret = buffer;
    }
    return true;
}


Worker::~Worker()
{
    zip_close(archive);
}