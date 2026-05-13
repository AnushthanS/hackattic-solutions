#include <iostream>
#include <vector>
#include <string>
#include <zip.h>

using namespace std; //zip not in std

void test_password(zip_t* archive, const string& fileName, const string& password) {
    cout << "Testing password: [" << password << "]" << endl;

    zip_file_t* zf = zip_fopen_encrypted(archive, fileName.c_str(), 0, password.c_str());
    
    if (!zf) {
        cout << "Result: Error opening file entry." << endl;
        return;
    }


    char buffer[1024];
    zip_int64_t bytesRead = zip_fread(zf, buffer, sizeof(buffer) - 1);

    if (bytesRead < 0) {
        cout << "Result: FAILURE (Wrong Password)." << endl;
    } else {
        buffer[bytesRead] = '\0';
        cout << "Result: SUCCESS!" << endl;
        cout << "--- Content ---" << endl;
        cout << buffer << endl;
        cout << "---------------" << endl;
    }

    zip_fclose(zf);
}

int main() {
    string zipPath = "my_zip.zip";
    string targetFile = "secret.txt";
    int err = 0;

    zip_t* archive = zip_open(zipPath.c_str(), 0, &err);
    if (!archive) {
        zip_error_t zerr;
        zip_error_init_with_code(&zerr, err);
        cerr << "Failed to open archive: " << zip_error_strerror(&zerr) << endl;
        return 1;
    }

    test_password(archive, targetFile, "wrong_password_123");

    cout << endl;

    test_password(archive, targetFile, "test");

    zip_close(archive);

    return 0;
}