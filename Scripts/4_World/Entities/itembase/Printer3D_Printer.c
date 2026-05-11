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

#ifdef SERVER
//Vars & Init
	string 			currentPrintEntityType;
	int 			currentPrintTime;
	string 			currentPrintMaterial;
	int 			currentPrintAmount;
	string 			currentPrintEntityName;
	PlayerIdentity 	currentPrintNotificationPlayer;
	float 			startedFilamentAmount;
	float 			endedFilamentAmount;

	private ref array<ref PrintQueueItem> PrintQueue;
	private ref Timer CleanupTimer;
#endif

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
#ifdef SERVER
		PrintQueue = new array<ref PrintQueueItem>();
		startedFilamentAmount = -1;
		endedFilamentAmount = -1;

        if (GetGame().IsServer() || GetGame().IsMultiplayer())
        {
            SetSynchDirty();
            RegisterNetSyncVariableBool("m_IsPrinting");
			RegisterNetSyncVariableBool("m_DisablePickup");

			m_DisablePickup = GetDayZGame().GetPrinterConfig().DisablePrinterPickup();
			SetSynchDirty();

			// Set up periodic cleanup every 30 seconds
			CleanupTimer = new Timer(CALL_CATEGORY_SYSTEM);
			CleanupTimer.Run(30.0, this, "PeriodicCleanup", null, true);
		}
#endif

		if (!GetGame().IsMultiplayer() || !GetGame().IsServer()) { return; }

        if (GetDayZGame().GetPrinterConfig().RGFNS()) {RGFNS = true;}   
	};

    bool RGFNS() {return RGFNS;}

    void ~Printer3D_Printer()
	{
#ifdef SERVER
		if (CleanupTimer)
		{
			CleanupTimer.Stop();
			delete CleanupTimer;
		}
#endif

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

#ifdef SERVER
//Print Queue Functions
	void AddItemToQueue(PrintQueueItem item)
    {
		if (!IsQueueItemValid(item)) { 
			if (GetDayZGame().GetPrinterConfig().DebugMode())
			{
				GetRusticModsCoreLogger().Log("Printer3D", "SERVER", "Cannot add invalid item to queue");
			}
			return; 
		}

        PrintQueue.Insert(item);

        if (!m_IsPrinting)
        {
			CleanupInvalidQueueItems();
			if (PrintQueue.Count() > 0)
			{
				PrintQueueItem NextItem = PrintQueue[0];
				if (IsQueueItemValid(NextItem))
				{
					TryStartPrinting(NextItem.PrintEntityType, NextItem.PrintTime, NextItem.PrintMaterial, NextItem.PrintAmount, NextItem.PrintEntityName, NextItem.NotificationPlayer);
				}
				else
				{
					PrintQueue.Remove(0);
					ResumePrintQueue();
				}
			}
        } else {			
			GetRPCManager().SendRPC("Printer3D", "SendTranslatedReplacedString", new Param3<string, string, PlayerIdentity>("str_printer_queue_added", item.PrintEntityName, item.NotificationPlayer), true, item.NotificationPlayer);
        }
    }

	void RemoveItemFromQueue(int position)
    {
        if (position >= 0 && position < PrintQueue.Count())
        {
			PrintQueueItem queueItem = PrintQueue[position];
			if (!IsQueueItemValid(queueItem))
			{
				if (GetDayZGame().GetPrinterConfig().DebugMode())
				{
					GetRusticModsCoreLogger().Log("Printer3D", "SERVER", "Removing invalid queue item at position: " + position.ToString());
				}
				PrintQueue.Remove(position);
				return;
			}

            string queueItemName = queueItem.PrintEntityName;
			PlayerIdentity queueItemNotificationPlayer = queueItem.NotificationPlayer;

			GetRPCManager().SendRPC("Printer3D", "SendTranslatedReplacedString", new Param3<string, string, PlayerIdentity>("str_printer_queue_removed", queueItemName, queueItemNotificationPlayer), true, queueItemNotificationPlayer);

            PrintQueue.Remove(position);
        }
    }

    void ClearQueue()
    {
        if (PrintQueue.Count() > 1)
        {
            for (int i = PrintQueue.Count() - 1; i > 0; i--)
            {
                PrintQueue.Remove(i);
            }

			if (!m_IsPrinting)
			{
				PrintQueue.Remove(0);
			}
        }

        SendPrinterNotification(currentPrintNotificationPlayer, "#str_printer_queue_cleared");
    }

	void ResumePrintQueue()
	{
		CleanupInvalidQueueItems();
		if (PrintQueue.Count() > 0)
		{
			PrintQueueItem NextItem = PrintQueue[0];
			if (IsQueueItemValid(NextItem))
			{
				TryStartPrinting(NextItem.PrintEntityType, NextItem.PrintTime, NextItem.PrintMaterial, NextItem.PrintAmount, NextItem.PrintEntityName, NextItem.NotificationPlayer);
			}
			else
			{
				PrintQueue.Remove(0);
				ResumePrintQueue();
			}
		}
	}

	bool IsQueueItemValid(PrintQueueItem item)
	{
		if (!item) return false;
		return item.IsValid();
	}

	void CleanupInvalidQueueItems()
	{
		for (int i = PrintQueue.Count() - 1; i >= 0; i--)
		{
			if (!IsQueueItemValid(PrintQueue[i]))
			{
				if (GetDayZGame().GetPrinterConfig().DebugMode())
				{
					GetRusticModsCoreLogger().Log("Printer3D", "SERVER", "Removing invalid queue item at position: " + i.ToString());
				}
				PrintQueue.Remove(i);
			}
		}
	}

	void PeriodicCleanup()
	{
		CleanupInvalidQueueItems();
	}

	array<ref PrintQueueItem> GetPrintQueue()
    {
		CleanupInvalidQueueItems();
        return PrintQueue;
    }

//Printing Functions

	bool TryStartPrinting(string PrintEntityType, int PrintTime, string PrintMaterial, int PrintAmount, string PrintEntityName, PlayerIdentity NotificationPlayer)
	{
		if (!GetGame().IsMultiplayer() || !GetGame().IsServer()) { return false; }

		bool CanWork = false;

		//---------------------------------------//
		//-----------------Power-----------------//
		//---------------------------------------//

		bool requireGenerator = false;
		bool hasPower = this.HasEnergyManager() && this.GetCompEM().CanWork() && this.GetCompEM().CanSwitchOn();
		bool IsStatic = false;

		if (this.IsInherited(Printer3D_Printer_static)) {IsStatic = true;}
		
		if (IsStatic) 
		{
			requireGenerator = GetDayZGame().GetPrinterConfig().RGFS();
		}
		
		if (!IsStatic) 
		{
			requireGenerator = GetDayZGame().GetPrinterConfig().RGFNS();
		}

		if (requireGenerator)
		{
		    CanWork = hasPower;
		}
		else
		{
		    CanWork = true;
		}

		if (!CanWork) 
		{
			if (GetDayZGame().GetPrinterConfig().DebugMode())
			{
				GetRusticModsCoreLogger().Log("Printer3D", "CLIENT", "There was an error starting the print job. Printer has no power.");
			}
			SendPrinterNotification(NotificationPlayer, "#str_printer_no_power");
			return false; 
		}

		//---------------------------------------//
		//------------------USB------------------//
		//---------------------------------------//
		
		//ADD PRINTER SLOTS HERE
		
		EntityAI USB = FindAttachmentBySlotName(ATTACHMENT_SLOT_USB);

		if (!USB) 
		{
			SendPrinterNotification(NotificationPlayer, "#str_printer_no_usb");
			CanWork = false;
		}

		array<string> folderSlots = new array<string>;
		folderSlots = {
		    ATTACHMENT_SLOT_FOLDER1,
		    ATTACHMENT_SLOT_FOLDER2,
		    ATTACHMENT_SLOT_FOLDER3,
		    ATTACHMENT_SLOT_FOLDER4
		};

		Printer3D_FolderBase FolderItem;
		int totalFolderQuantity = 0;
		for (int i = 0; i < folderSlots.Count(); i++)
		{
			Printer3D_FolderBase currentFolder = Printer3D_FolderBase.Cast(USB.FindAttachmentBySlotName(folderSlots[i]));
			if (currentFolder && currentFolder.GetSelectedEntityName() == PrintEntityType && currentFolder.GetSelectedEntityDisplayName() == PrintEntityName && currentFolder.GetSelectedEntityTimeToPrint() == PrintTime)
			{
				int folderQty = 1;
				if (currentFolder.HasQuantity())
				{
					folderQty = currentFolder.GetQuantity();
				}
				totalFolderQuantity += folderQty;
				if (!FolderItem && folderQty > 0)
				{
					FolderItem = currentFolder;
				}
			}
		}
		
		if (!FolderItem || totalFolderQuantity < 1)
		{
			if (GetDayZGame().GetPrinterConfig().DebugMode())
			{
				GetRusticModsCoreLogger().Log("Printer3D", "CLIENT", "Folder item not found or empty in USB. Cannot start print job."); 
			}
			SendPrinterNotification(NotificationPlayer, "#str_printer_folder_empty");
			CanWork = false;
		}

		if (!CanWork) 
		{
			if (GetDayZGame().GetPrinterConfig().DebugMode())
			{
				GetRusticModsCoreLogger().Log("Printer3D", "CLIENT", "There was an error starting the print job. Printer has no USB."); 
			}
			return false; 
		}

		//---------------------------------------//
		//---------------Filament----------------//
		//---------------------------------------//

		EntityAI FilamentInstalled = FindAttachmentBySlotName(ATTACHMENT_SLOT_FILAMENT);
		ItemBase FilamentItem = ItemBase.Cast(FindAttachmentBySlotName(ATTACHMENT_SLOT_FILAMENT));
		float InitialFilamentAmount = FilamentItem.GetQuantity();

		if (!FilamentInstalled) 
		{
			SendPrinterNotification(NotificationPlayer, "#str_printer_no_filament");
			CanWork = false;
		}

		if (FilamentItem.GetQuantity() < PrintAmount && FilamentInstalled) 
		{
			SendPrinterNotification(NotificationPlayer, "#str_printer_not_enough_filament");
			CanWork = false;
		}

		if (FilamentItem.GetType() != PrintMaterial && FilamentInstalled) 
		{
			SendPrinterNotification(NotificationPlayer, "#str_printer_wrong_filament");
			CanWork = false;
		}

		if (!CanWork) 
		{
			if (GetDayZGame().GetPrinterConfig().DebugMode())
			{
				GetRusticModsCoreLogger().Log("Printer3D", "CLIENT", "There was an error starting the print job. Printer has no filament."); 
			}
			return false; 
		}

		//---------------------------------------//
		//---------------Item Check--------------//
		//---------------------------------------//
		bool WillDrop = false;

		if (!PrintEntityType)
		{
			SendPrinterNotification(NotificationPlayer, "#str_printer_no_model");
			CanWork = false;
		} else {
			EntityAI ItemToAdd = EntityAI.Cast(GetGame().CreateObject(PrintEntityType, "0 0 0", false, false, false)); 

			if (this.GetInventory().CanAddEntityInCargo(ItemToAdd, false) == false)
			{
				if (GetDayZGame().GetPrinterConfig().DropItemsIfNoSpace())
				{
					SendPrinterNotification(NotificationPlayer, "#str_printer_no_space_drop");
					WillDrop = true;
				} else {
					SendPrinterNotification(NotificationPlayer, "#str_printer_no_space");
					CanWork = false;
				}
			}
		}

		GetGame().ObjectDelete(ItemToAdd);

		if (!CanWork) 
		{
			if (GetDayZGame().GetPrinterConfig().DebugMode())
			{
				GetRusticModsCoreLogger().Log("Printer3D", "CLIENT", "There was an error starting the print job. Printer has no filament.");
			}
			return false; 
		}

		//---------------------------------------//
		//---------------Start Print-------------//
		//---------------------------------------//

		PrintTimer = new Timer( CALL_CATEGORY_SYSTEM );
		// PrintTimerFR = new Timer( CALL_CATEGORY_SYSTEM );

			if (FilamentItem && startedFilamentAmount <= 0 && endedFilamentAmount <= 0)
			{
				startedFilamentAmount = FilamentItem.GetQuantity();
				endedFilamentAmount = startedFilamentAmount - PrintAmount;
			}
		
		PrintTimer.Run(PrintTime, this, "PrintItem", new Param8<string, int, ItemBase, Printer3D_FolderBase, float, string, PlayerIdentity, bool>(PrintEntityType, PrintAmount, FilamentItem, FolderItem, InitialFilamentAmount, PrintEntityName, NotificationPlayer, WillDrop), false);
		
		// float FPS = PrintAmount / PrintTime;
		// PrintTimerFR.Run(1, this, "RemoveFilament", new Param2<ItemBase, float>(FilamentItem, FPS), true);

		ChangeSlotLocks(true);

		this.GetCompEM().SwitchOn();

		SetCurrentPrintJob(true, PrintEntityType, PrintTime, PrintMaterial, PrintAmount, PrintEntityName, NotificationPlayer);

		GetRPCManager().SendRPC("Printer3D", "SendTranslatedReplacedString", new Param3<string, string, PlayerIdentity>("str_printer_printing_item", PrintEntityName, NotificationPlayer), true, NotificationPlayer);

		SetSynchDirty();

		return true;
	};

	void SetCurrentPrintJob(bool isPrinting, string PrintEntityType = string.Empty, int PrintTime = 0, string PrintMaterial = string.Empty, int PrintAmount = 0, string PrintEntityName = string.Empty, PlayerIdentity NotificationPlayer = null) 
	{
		m_IsPrinting					= isPrinting;

		currentPrintEntityType 			= PrintEntityType;
		currentPrintTime 				= PrintTime;
		currentPrintMaterial 			= PrintMaterial;
		currentPrintAmount 				= PrintAmount;
		currentPrintEntityName 			= PrintEntityName;
		currentPrintNotificationPlayer 	= NotificationPlayer;
		SetPrintName(PrintEntityName);
		SetSynchDirty();
	};
	
	void PrintItem(string Item, int FilamentUsage, ItemBase FilamentItem, Printer3D_FolderBase FolderItem, float InitialFilamentAmount, string ItemDN, PlayerIdentity NotificationPlayer, bool Drop)
	{
		if (!GetGame().IsMultiplayer() || !GetGame().IsServer()) { return; }

		StopPrinter("#str_printer_printing_finished", NotificationPlayer, ItemDN, "str_printer_printing_finished");

		// if (PrintTimerFR) 
		// {
		// 	PrintTimerFR.Stop();
		// }
		
		if (FilamentItem)
		{
			FilamentItem.SetQuantity(InitialFilamentAmount - FilamentUsage);

			endedFilamentAmount = startedFilamentAmount - FilamentUsage;
			if (FilamentItem.GetQuantity() != endedFilamentAmount)
			{
				FilamentItem.SetQuantity(endedFilamentAmount);
			}

			if (FilamentItem.GetQuantity() <= 0)
			{
				FilamentItem.DeleteSafe();
			}
		}
		
		startedFilamentAmount = -1;
		endedFilamentAmount = -1;
		
		if (FolderItem.HasQuantity() && FolderItem)
		{
			FolderItem.SetQuantity(FolderItem.GetQuantity() - 1);
		}
		
		EntityAI ItemToAdd = EntityAI.Cast(GetGame().CreateObject(Item, "0 0 0", false, false, false)); 
		int PrintQuantity = FolderItem.GetSelectedEntityPrintQuantity();

		for (int i = 0; i < PrintQuantity; i++)
		{
			if (!Drop && this.GetInventory().CanAddEntityInCargo(ItemToAdd, false))
			{
				this.GetInventory().CreateInInventory(Item);
			} else {
				GetGame().CreateObject(Item, this.GetPosition(), false, true); 
			}
		}

		ItemToAdd.Delete();

		PrintQueue.Remove(0);
		CleanupInvalidQueueItems();
		if (PrintQueue.Count() > 0)
		{
			PrintQueueItem NextItem = PrintQueue.Get(0);
			if (IsQueueItemValid(NextItem))
			{
				TryStartPrinting(NextItem.PrintEntityType, NextItem.PrintTime, NextItem.PrintMaterial, NextItem.PrintAmount, NextItem.PrintEntityName, NextItem.NotificationPlayer);
			}
			else
			{
				PrintQueue.Remove(0);
				ResumePrintQueue();
			}
		} else {
			SendPrinterNotification(NotificationPlayer, "#str_printer_queue_empty");
		}
		 
		SetSynchDirty();
	}
 
	void TryStopPrinting(PlayerIdentity NotificationPlayer = null)
    {
		if (!GetGame().IsMultiplayer() || !GetGame().IsServer()) { return; }

		// if (PrintTimerFR)
		// {
		// 	PrintTimerFR.Stop();
		// 	delete PrintTimerFR;
		// }

		if (PrintTimer)
		{
			PrintTimer.Stop();
			delete PrintTimer;
		}

		StopPrinter("#str_printer_cancelled", NotificationPlayer);

		if (PrintQueue.Count() > 0)
		{
			PrintQueue.Remove(0);
			if (GetDayZGame().GetPrinterConfig().AutoResumeQueueAfterStop())
			{
				CleanupInvalidQueueItems();
				if (PrintQueue.Count() > 0)
				{
					PrintQueueItem NextItem = PrintQueue.Get(0);
					if (IsQueueItemValid(NextItem))
					{
						TryStartPrinting(NextItem.PrintEntityType, NextItem.PrintTime, NextItem.PrintMaterial, NextItem.PrintAmount, NextItem.PrintEntityName, NextItem.NotificationPlayer);
					}
					else
					{
						PrintQueue.Remove(0);
						ResumePrintQueue();
					}
				}
			}
		}

        SetSynchDirty();
    }

	void StopPrinter(string StopNotification, PlayerIdentity NotificationPlayer, string ReplaceString = string.Empty, string Key = string.Empty)
	{
		if (!GetGame().IsMultiplayer() || !GetGame().IsServer()) { return; }

		ChangeSlotLocks(false);

		SetCurrentPrintJob(false);

		this.GetCompEM().SwitchOff();

		if (ReplaceString != string.Empty)
		{
			GetRPCManager().SendRPC("Printer3D", "SendTranslatedReplacedString", new Param3<string, string, PlayerIdentity>(Key, ReplaceString, NotificationPlayer), true, NotificationPlayer);
		} else {
			SendPrinterNotification(NotificationPlayer, StopNotification);
		}
	}

//Helpers
	void ChangeSlotLocks(bool Lock)
	{
		GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString(ATTACHMENT_SLOT_FILAMENT), Lock);
		GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString(ATTACHMENT_SLOT_USB), Lock);
		if (Lock) {GetInventory().LockInventory(LOCK_FROM_SCRIPT);} else {GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);}
	};

	override void OnStoreSave(ParamsWriteContext ctx)
	{
	    super.OnStoreSave(ctx);

	    ctx.Write(m_IsPrinting);
	    ctx.Write(m_SyncedCurrentPrint);

	    ctx.Write(currentPrintEntityType);
	    ctx.Write(currentPrintTime);
	    ctx.Write(currentPrintMaterial);
	    ctx.Write(currentPrintAmount);
	    ctx.Write(currentPrintEntityName);
		ctx.Write(startedFilamentAmount);
		ctx.Write(endedFilamentAmount);
		
		ctx.Write(PrintQueue.Count());
		for (int i = 0; i < PrintQueue.Count(); i++)
		{
			ctx.Write(PrintQueue[i]);
		}
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
	    if (!super.OnStoreLoad(ctx, version))
	        return false;

	    if (!ctx.Read(m_IsPrinting))
	        return false;

	    if (!ctx.Read(m_SyncedCurrentPrint))
	        return false;

	    if (!ctx.Read(currentPrintEntityType))
	        return false;

	    if (!ctx.Read(currentPrintTime))
	        return false;

	    if (!ctx.Read(currentPrintMaterial))
	        return false;

	    if (!ctx.Read(currentPrintAmount))
	        return false;

	    if (!ctx.Read(currentPrintEntityName))
	        return false;

		int PrintQueueCount;
		if (!ctx.Read(PrintQueueCount))
			return false;

		if (!ctx.Read(startedFilamentAmount))
			return false;

		if (!ctx.Read(endedFilamentAmount))
			return false;

		PrintQueue.Clear();
		for (int i = 0; i < PrintQueueCount; i++)
		{
			PrintQueueItem NewQueueItem = new PrintQueueItem();
			if (!ctx.Read(NewQueueItem))
				return false;

			if (IsQueueItemValid(NewQueueItem))
			{
				PrintQueue.Insert(NewQueueItem);
			}
			else if (GetDayZGame().GetPrinterConfig().DebugMode())
			{
				GetRusticModsCoreLogger().Log("Printer3D", "SERVER", "Skipped loading invalid queue item during restoration");
			}
		}

	    if (m_IsPrinting)
	    {
	        TryStartPrinting(currentPrintEntityType, currentPrintTime, currentPrintMaterial, currentPrintAmount, currentPrintEntityName, currentPrintNotificationPlayer);
	    }
		else
		{
			CleanupInvalidQueueItems();
		}

	    return true;
	}
#endif

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
