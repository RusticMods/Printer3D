modded class DayZGame 
{
    protected ref Printer3DConfig m_PrinterConfig;

    ref Printer3DConfig GetPrinterConfig()
    {
        return m_PrinterConfig;
    }

    void SetPrinterConfig(ref Printer3DConfig printerConfig)
    {
        m_PrinterConfig = printerConfig;
    }

}