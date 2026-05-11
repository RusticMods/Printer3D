#ifdef SERVER
class PrinterRPCManager : PluginBase 
{
    void PrinterRPCManager() 
    {
        GetRPCManager().AddRPC("Printer3D", "TryStartPrintingRPC", this, SingleplayerExecutionType.Server);
		GetRPCManager().AddRPC("Printer3D", "TryStopPrintingRPC", this, SingleplayerExecutionType.Server);

        GetRPCManager().AddRPC("Printer3D", "ResumePrintQueue", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("Printer3D", "ClearPrintQueue", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("Printer3D", "RemoveFromQueue", this, SingleplayerExecutionType.Both);

		GetRPCManager().AddRPC("Printer3D", "SendMenuDataRPC", this, SingleplayerExecutionType.Both);

        GetRPCManager().AddRPC("Printer3D", "SendPrinterNotification", this, SingleplayerExecutionType.Both);

    };

    void TryStartPrintingRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
		Param6<string, int, string, int, string, Printer3D_Printer> param;
        if(!ctx.Read(param)) return;

        Printer3D_Printer printer = param.param6;

        if(type == CallType.Server)
        {
            PrintQueueItem item = new PrintQueueItem(param.param1, param.param2, param.param3, param.param4, param.param5, sender);
            printer.AddItemToQueue(item);
		}
    }
	
	void TryStopPrintingRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<Printer3D_Printer> param;
        if(!ctx.Read(param)) return;

        Printer3D_Printer printer = param.param1;

        if(type == CallType.Server)
        {
            printer.TryStopPrinting(sender);
		}
    }

    void ResumePrintQueue(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<Printer3D_Printer> param;
        if(!ctx.Read(param)) return;

        Printer3D_Printer printer = param.param1;

        if(type == CallType.Server)
        {
            printer.ResumePrintQueue();
        }
    }

    void ClearPrintQueue(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<Printer3D_Printer> param;
        if(!ctx.Read(param)) return;
        Printer3D_Printer printer = param.param1;

        if(type == CallType.Server)
        {
            printer.ClearQueue();
        }
    }

    void RemoveFromQueue(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param2<Printer3D_Printer, int> param;
        if(!ctx.Read(param)) return;

        Printer3D_Printer printer = param.param1;
        int index = param.param2;

        if(type == CallType.Server)
        {
            if (index == 0 && printer.IsPrinting())
            {
                printer.SendPrinterNotification(sender, "#str_printer_cannot_remove_current");
                return;
            }

            if (index >= printer.GetPrintQueue().Count())
            {
                printer.SendPrinterNotification(sender, "#str_printer_invalid_queue_index");
                return;
            }

            printer.RemoveItemFromQueue(index);

            GetRPCManager().SendRPC("Printer3D", "UpdateQueueMenu", new Param1<array<ref PrintQueueItem>>(printer.GetPrintQueue()), true, sender);
        }
    }

    void UpdateQueue(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<Printer3D_Printer> param;
        if(!ctx.Read(param)) return;

        Printer3D_Printer printer = param.param1;

        if(type == CallType.Server)
        {
            printer.GetPrintQueue();

        }
    }

	void SendMenuDataRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param2<Printer3D_Printer, PlayerIdentity> param;
        if(!ctx.Read(param)) return;

        Printer3D_Printer printer = param.param1;
        PlayerIdentity player = param.param2;

		Printer3D_USB USB = null;
		Printer3D_FolderBase slot1 = null;
		Printer3D_FolderBase slot2 = null;
		Printer3D_FolderBase slot3 = null;
		Printer3D_FolderBase slot4 = null;

		float PrintTimerRemaining = 0.0;
		float PrintTimerDuration = 0.0;

		PrintTimerRemaining = printer.GetRemaining();
        PrintTimerDuration = printer.GetInitial();


		USB = Printer3D_USB.Cast( printer.GetAttachmentByType(Printer3D_USB) );

		slot1 = Printer3D_FolderBase.Cast( USB.FindAttachmentBySlotName("Folder1") );
	 	slot2 = Printer3D_FolderBase.Cast( USB.FindAttachmentBySlotName("Folder2") );
		slot3 = Printer3D_FolderBase.Cast( USB.FindAttachmentBySlotName("Folder3") );
		slot4 = Printer3D_FolderBase.Cast( USB.FindAttachmentBySlotName("Folder4") );

		GetRPCManager().SendRPC("Printer3D", "SetAttVarsRPC", new Param5<Printer3D_USB, Printer3D_FolderBase, Printer3D_FolderBase, Printer3D_FolderBase, Printer3D_FolderBase>(USB, slot1, slot2, slot3, slot4), true, player);
        GetRPCManager().SendRPC("Printer3D", "SetMenuDataRPC", new Param5<bool, string, float, float, array<ref PrintQueueItem>>(printer.IsPrinting(), printer.GetCurrentPrint(), PrintTimerRemaining, PrintTimerDuration, printer.GetPrintQueue()), true, player);
	}

    void SendPrinterNotification(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param2<string, PlayerIdentity> param;
        if(!ctx.Read(param)) return;

        if (param.param2) 
		{
			NotificationSystem.SendNotificationToPlayerIdentityExtended(param.param2, 10.0, "#str_printer_notification_title", param.param1);
		}
    }
}

PrinterRPCManager GetPrinterRPCManager() {
    return PrinterRPCManager.Cast(GetPlugin(PrinterRPCManager));
}
#endif
