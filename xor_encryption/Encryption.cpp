// File: Encryption.cpp
//

#include <iostream>
#include <fstream>
#include <ctime>


// Forward Declarations

std::string xorDataStr(const std::string &, const std::string &);
std::string readFile(const std::string &);
std::string getHeaderString(const std::string &);
void saveDataFile(const std::string &, const std::string &, const std::string &, const std::string &);


/**
 * @brief Entry Point for program
 *
 * @param argc    Count of arguments
 * @param argv    Command line arguments
 * @return int    Exit Code
 */
int main(int argc, char *argv[]) {
    std::string xorKey = "default";

    // check if program recieved more than one argument
    if (argc > 2) {
        std::cerr << "Program only takes \"XOR_KEY\" as a argument" << std::endl;
        std::cerr << "\tEnsure the xor key is encapsulated within quotes";
        exit(EXIT_FAILURE);
    }

    if(argc == 2 && sizeof(argv[1]) > 0) {
        xorKey = argv[1];
    }


    const std::string inputFile = "input.txt", encFile = "encrypted.txt", decFile = "decrypted.txt";
    const std::string dataSrc = readFile(inputFile);
    const std::string headerLine = getHeaderString(dataSrc);

    // Call the XOR function to de
    const std::string encData = xorDataStr(dataSrc, xorKey);
    const std::string decData = xorDataStr(encData, xorKey);
    saveDataFile(encFile, headerLine, xorKey, encData);
    saveDataFile(decFile, headerLine, xorKey, decData);

    std::cout << "Read File: " << inputFile
              << "\n\t- Encrypted To: " << encFile
              << "\n\t- Decrypted To: " << decFile
              << std::endl;

    return (EXIT_SUCCESS);
} // end of main



/**
 * @brief XOR data from file using a string/key
 *
 * @param dataSrc         Input string to process
 * @param xorKey          Key to use for the XOR
 * @return std::string    Output string
 */
std::string xorDataStr(const std::string &dataSrc, const std::string &xorKey) {
    static const size_t keyLen = xorKey.length();
    static const size_t dataLen = dataSrc.length();

    // Check if the key length and source length are greater than 0
    //
    if(keyLen <= 0 && dataLen <= 0) {
        std::cerr << "Key length or source data is 0" << std::endl;
        exit(EXIT_FAILURE);
    }


    std::string output = dataSrc;

    static size_t i = 0;
    for(i = 0; i < dataLen; ++i) {
        output.at(i) = (unsigned int)((output.at(i) ^ xorKey.at(i % keyLen)));
    }

    if(output.length() == dataSrc.length()) {
        return output;
    }

    return "[ERROR] output does not equal source length";
}

/**
 * @brief Read all bytes within a file to a string
 *
 * @param inputFile Name of the input file.
 * @return std::string as the input file.
 */
std::string readFile(const std::string &inputFile) {
    std::string tmp = "default_file_data";

    // Declare and open the file
    std::ifstream iFileStream;
    iFileStream.open(inputFile);

    // set tmp to the contents of file (first char... last char)
    tmp.assign(
        (std::istreambuf_iterator(iFileStream)), // fist char in file
        (std::istreambuf_iterator<char>())           // last char in file
    );

    // close stream and return string
    iFileStream.close();
    return tmp;
}

/**
 * @brief Retrieve only the first line of the file.
 *
 * @param fileData       std::string as the input file.
 * @return std::string   First line of the input data file.
 */
std::string getHeaderString(const std::string &fileData) {
    std::string tmp = "default_header";

    // Seek to the first line feed, and copy the name if it exists...
    const size_t pos = fileData.find('\n');
    if(pos != std::string::npos) {
        tmp = fileData.substr(0, pos);
    }

    return (tmp);
} // end of getHeaderString

/**
 * @brief    Writes data from a in-memory string to an external file
 *
 * @param inputFile       File to save data to
 * @param headerLine      Name of the Header
 * @param xorKey          The passkey used when XORing data
 * @param data            The string containing all file data
 */
void saveDataFile(const std::string &inputFile, const std::string &headerLine,
    const std::string &xorKey, const std::string &data) {

    // Declare the file stream
    std::fstream fStreamOut;


    // open and write to file
    fStreamOut.open(inputFile, std::fstream::out);
    fStreamOut <<
        "Header:\t" << headerLine <<
        "\nDate:\t" << __TIMESTAMP__ <<
        "\nKey:\t" << xorKey <<
        "\n========= BEGIN DATA =========" << std::endl <<
        data << std::endl <<
        "=========  END DATA  =========" << std::endl;
    // end of stream output


    // Close file stream
    fStreamOut.close();
} // end of saveDataFile
