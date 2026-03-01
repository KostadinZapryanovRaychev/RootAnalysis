
#include <iostream>
#include <string>
#include "TSystem.h"
#include "TMD5.h"
#include "TFile.h"

/**
 * Computes and compares the MD5 checksum of two files.
 * If the checksums match the files are identical byte-for-byte.
 *
 * Execution example:
 * [] .x checksum.C("AnalyzeEfficiency_321475_325172_RPCMon_2018D.root", "AnalyzeEfficiency_367334_368765_RPCMon_2023C.root")
 */

void checksum(const char *file1, const char *file2)
{
    if (gSystem->AccessPathName(file1))
    {
        std::cerr << "ERROR: file not found: " << file1 << std::endl;
        return;
    }
    if (gSystem->AccessPathName(file2))
    {
        std::cerr << "ERROR: file not found: " << file2 << std::endl;
        return;
    }

    // --- compute MD5 for both files ---
    TMD5 *md5_1 = TMD5::FileChecksum(file1);
    TMD5 *md5_2 = TMD5::FileChecksum(file2);

    if (!md5_1 || !md5_2)
    {
        std::cerr << "ERROR: could not compute checksum." << std::endl;
        return;
    }

    std::string sum1 = md5_1->AsString();
    std::string sum2 = md5_2->AsString();

    delete md5_1;
    delete md5_2;

    // --- print results ---
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════════\n";
    std::cout << "  File checksum comparison\n";
    std::cout << "══════════════════════════════════════════════════════════\n";
    std::cout << "  File 1 : " << file1 << "\n";
    std::cout << "  MD5    : " << sum1 << "\n";
    std::cout << "──────────────────────────────────────────────────────────\n";
    std::cout << "  File 2 : " << file2 << "\n";
    std::cout << "  MD5    : " << sum2 << "\n";
    std::cout << "══════════════════════════════════════════════════════════\n";

    if (sum1 == sum2)
    {
        std::cout << "  RESULT : ✅  IDENTICAL — both files are byte-for-byte the same.\n";
    }
    else
    {
        std::cout << "  RESULT : ✅  DIFFERENT — the files have different content.\n";
    }
    std::cout << "══════════════════════════════════════════════════════════\n\n";
}
