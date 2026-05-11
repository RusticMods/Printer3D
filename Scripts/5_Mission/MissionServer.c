#ifdef SERVER
modded class MissionServer 
{
    ref Printer3DConfig PrinterConfig;
    string configFolder;

    void MissionServer() 
    {
        GetRusticModsCoreLogger().Log("Printer3D", "SERVER", "Server Mod Loaded Correctly");

//CONFIG
        configFolder = CONSTRusticModsCore.RM_MOD_FOLDER + CONSTPrinter3DConfig.PRINTERConfigName;

        if (!FileExist(configFolder))
        {
            if (!PrinterConfig)
            {
                PrinterConfig = new Printer3DConfig();
            }

            GetDayZGame().SetPrinterConfig(PrinterConfig);
            MakeDirectory(CONSTRusticModsCore.RM_MOD_FOLDER);
            SavePrinterConfig();
        } else {
            LoadPrinterConfig();
            GetDayZGame().SetPrinterConfig(PrinterConfig);
            SavePrinterConfig();
        }

    }
    
    private void SavePrinterConfig()
    {
        JsonFileLoader<Printer3DConfig>.JsonSaveFile(configFolder, PrinterConfig);
    }

    private void LoadPrinterConfig()
    {
        JsonFileLoader<Printer3DConfig>.JsonLoadFile(configFolder, PrinterConfig);
        PrinterConfig.Validate();
    }
}
#endif
