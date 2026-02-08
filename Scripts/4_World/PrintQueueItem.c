class PrintQueueItem
{
    string PrintEntityType;
    int PrintTime;
    string PrintMaterial;
    int PrintAmount;
    string PrintEntityName;
    PlayerIdentity NotificationPlayer;

    // Constructor
    void PrintQueueItem(string entityType = "", int time = 0, string material = "", int amount = 0, string entityName = "", PlayerIdentity player = null)
    {
        PrintEntityType = entityType;
        PrintTime = time;
        PrintMaterial = material;
        PrintAmount = amount;
        PrintEntityName = entityName;
        NotificationPlayer = player;
    }

    // Validation method
    bool IsValid()
    {
        if (PrintEntityType == string.Empty) return false;
        if (PrintTime <= 0) return false;
        if (PrintMaterial == string.Empty) return false;
        if (PrintAmount <= 0) return false;
        if (PrintEntityName == string.Empty) return false;
        //if (!NotificationPlayer) return false;
        return true;
    }
}
