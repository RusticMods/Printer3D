class Printer3DConfig {
    private string EnableDebugMode;
    private string DropItemsIfNoSpace;
    private string RequireGeneratorForStatic;
    private string RequireGeneratorForNonStatic;
    private string DegradeFolders;
    private string AutoResumeQueueAfterStop;
    private string DisablePrinterPickup;
    //private string EnableUSB;

    // Assign defaults
    void Printer3DConfig() 
    {
        EnableDebugMode = "false";
        DropItemsIfNoSpace = "false";
        RequireGeneratorForStatic = "false";
        RequireGeneratorForNonStatic = "true";
        DegradeFolders = "false";
        AutoResumeQueueAfterStop = "false";
        DisablePrinterPickup = "false";
        //EnableUSB = "true";
    }

    bool DebugMode() 
    {
        string lowerString = EnableDebugMode;
        lowerString.ToLower();

        return lowerString == "true";
    }

    bool DropItemsIfNoSpace() 
    {
        string lowerString = DropItemsIfNoSpace;
        lowerString.ToLower();

        return lowerString == "true";
    }

    bool RGFS() 
    {
        string lowerString = RequireGeneratorForStatic;
        lowerString.ToLower();

        return lowerString == "true";
    }

    bool RGFNS() 
    {
        string lowerString = RequireGeneratorForNonStatic;
        lowerString.ToLower();

        return lowerString == "true";
    }

    bool DegradeFolders()
    {
        string lowerString = DegradeFolders;
        lowerString.ToLower();

        return lowerString == "true";
    }

    bool AutoResumeQueueAfterStop()
    {
        string lowerString = AutoResumeQueueAfterStop;
        lowerString.ToLower();

        return lowerString == "true";
    }

    bool DisablePrinterPickup()
    {
        string lowerString = DisablePrinterPickup;
        lowerString.ToLower();

        return lowerString == "true";
    }

    /*
    bool EnableUSB()
    {
        string lowerString = EnableUSB;
        lowerString.ToLower();

        return lowerString == "true";
    }
    */

    void Validate() 
    {
        if (EnableDebugMode == string.Empty) {EnableDebugMode = "false";}
        if (DropItemsIfNoSpace == string.Empty) {DropItemsIfNoSpace = "false";}
        if (RequireGeneratorForStatic == string.Empty) {RequireGeneratorForStatic = "false";}
        if (RequireGeneratorForNonStatic == string.Empty) {RequireGeneratorForNonStatic = "true";}
        if (DegradeFolders == string.Empty) {DegradeFolders = "false";}
        if (AutoResumeQueueAfterStop == string.Empty) {AutoResumeQueueAfterStop = "false";}
        if (DisablePrinterPickup == string.Empty) {DisablePrinterPickup = "false";}
        //if (EnableUSB == string.Empty) {EnableUSB = "true";}
    }
}