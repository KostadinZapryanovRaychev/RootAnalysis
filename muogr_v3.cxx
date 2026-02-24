TDirectory *openDir(const char *fileName, const char *histoPath)
{
    TFile *f = TFile::Open(fileName);
    if (!f || f->IsZombie())
    {
        std::cerr << "ERROR: cannot open file: " << fileName << std::endl;
        return nullptr;
    }
    f->cd();
    if (!gDirectory->cd(histoPath))
    {
        std::cerr << "ERROR: directory '" << histoPath << "' not found in: " << fileName << std::endl;
        return nullptr;
    }
    return gDirectory;
}