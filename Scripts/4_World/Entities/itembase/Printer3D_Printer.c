class Printer3D_Printer extends ItemBase 
{
	const string ATTACHMENT_SLOT_USB	 	        = "Printer3D_USB";
    const string ATTACHMENT_SLOT_FILAMENT			= "Filament";

	const string ATTACHMENT_SLOT_FOLDER1			= "Folder1";
	const string ATTACHMENT_SLOT_FOLDER2			= "Folder2";
	const string ATTACHMENT_SLOT_FOLDER3			= "Folder3";
	const string ATTACHMENT_SLOT_FOLDER4			= "Folder4";
	
	PlayerBase m_Player;
    
    bool 		m_IsPrinting 						= false;
	bool 		m_DisablePickup 					= false;
	string 		m_SyncedCurrentPrint;
    bool 		RGFNS = false;
	
	ref Timer PrintTimer;
	ref Timer PrintTimerFR;

	void SendPrinterNotification(PlayerIdentity NotificationPlayer, string NotificationMessage)
	{
		if (NotificationPlayer) 
		{
			NotificationSystem.SendNotificationToPlayerIdentityExtended(NotificationPlayer, 10.0, "#str_printer_notification_title", NotificationMessage);
		}
	};

    override bool IsTakeable()
    {
        if (this.GetCompEM().IsSwitchedOn() || this.GetCompEM().IsWorking()) { return false; }
		if (m_DisablePickup) { return false; }

        return true;
    };

    override bool IsElectricAppliance()
	{
		return true;
	};

    override bool CanPutIntoHands(EntityAI parent)
	{
        return IsTakeable();
	};

    override bool CanPutInCargo( EntityAI parent )
    {
        return IsTakeable();
    };
	
    void Printer3D_Printer() 
	{
		if (!GetGame().IsMultiplayer() || !GetGame().IsServer()) { return; }

        if (GetDayZGame().GetPrinterConfig().RGFNS()) {RGFNS = true;}   
	};

    bool RGFNS() {return RGFNS;}

    void ~Printer3D_Printer()
	{
		StopTimers();
		
		GetCompEM().UnplugAllDevices();
	};

	void StopTimers()
	{
	    if (PrintTimerFR) 
	    {
			if (PrintTimerFR.IsRunning()) { PrintTimerFR.Stop(); }
	        delete PrintTimerFR;
	    }

	    if (PrintTimer) 
	    {
			if (PrintTimer.IsRunning()) { PrintTimer.Stop(); }
	        delete PrintTimer;
	    }
	};
	
	void RemoveFilament(ItemBase FilamentItem, float FPS)
	{
		if (!FilamentItem) { return; }
		float currentQuantity = FilamentItem.GetQuantity();
		if (currentQuantity - FPS <= 0 || currentQuantity == 0) { FilamentItem.Delete(); return; }
		FilamentItem.SetQuantity(currentQuantity - FPS);
	};

	void SetPrintName(string printName)
	{
	    m_SyncedCurrentPrint = printName;
	    SetSynchDirty();
	};

	float GetRemaining()
	{
		if (!PrintTimer) { return 0.0; }
		return PrintTimer.GetRemaining();
	};

	float GetInitial()
	{
		if (!PrintTimer) { return 0.0; }
		return PrintTimer.GetDuration();
	};

	bool IsPrinting()
	{
		return m_IsPrinting;
	};

	string GetCurrentPrint()
	{
		return m_SyncedCurrentPrint;
	};

    override void SetActions()
	{
		super.SetActions();
		
        AddAction(ActionTogglePlaceObject);
        AddAction(ActionPlaceObject);
        AddAction(OpenPrinterMenu);

        AddAction(ActionPlugTargetIntoThis);
		AddAction(ActionUnplugThisByCord);
	};
}
