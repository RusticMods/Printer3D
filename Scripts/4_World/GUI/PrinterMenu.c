class PrinterMenu extends UIScriptedMenu
{
//Vars
    private bool                 	m_Initialized;
    private bool                 	m_IsMenuOpen;

	bool							m_IsPrinting;

    protected Printer3D_Printer 	m_target;
	PlayerBase 						m_player;
	
	protected string 				m_SelectedPrintDN;
	protected string 				m_SelectedPrintN;
	protected int 					m_SelectedPrintT;
	protected int 					m_SelectedPrintFA;
	protected string 				m_SelectedPrintFT;
	
	ref Timer 						ProgressTimer;
	ref Timer						ProgressTimerLeft;
	float 							TotalPrintTime;
	float 							RemainingPrintTime;

	
	// Useless widgets
	private ScrollWidget 		 	m_Scroller;
    private GridSpacerWidget     	m_Grid;
    private TextWidget           	m_Title;
    private Widget               	m_pnl_bg;
	
	// Main functional widgets
	private ButtonWidget         	m_BtnStart;
	private ButtonWidget		 	m_BtnResume;
    private ButtonWidget         	m_BtnStop;
	private ButtonWidget		 	m_BtnClearQueue;
    private ButtonWidget         	m_BtnClose;
	
	//
	// Non grid related widgets
	//
			
	private TextWidget				m_NoUSBInstalled;
	private TextWidget 				m_SelectedPrint;
	private ProgressBarWidget		m_PrintingProgress; //deprecated for now
	private TextWidget 				m_PrintingTimeLeft;

	//
	// Item listing widgets
	//
	
	//Listing 0
	private Widget 					m_PrinterListing0;
	private ItemPreviewWidget 		m_ItemPreview0;
	private TextWidget				m_ItemName0;
	private TextWidget				m_FilamentLevel0;
	private TextWidget				m_FilamentAmount0;
	private TextWidget				m_TimeRequired0;
	private ButtonWidget			m_BtnSelect0;
	
	//Listing 1
	private Widget 					m_PrinterListing1;
	private ItemPreviewWidget 		m_ItemPreview1;
	private TextWidget				m_ItemName1;
	private TextWidget				m_FilamentLevel1;
	private TextWidget				m_FilamentAmount1;
	private TextWidget				m_TimeRequired1;
	private ButtonWidget			m_BtnSelect1;
	
	//Listing 2
	private Widget 					m_PrinterListing2;
	private ItemPreviewWidget 		m_ItemPreview2;
	private TextWidget				m_ItemName2;
	private TextWidget				m_FilamentLevel2;
	private TextWidget				m_FilamentAmount2;
	private TextWidget				m_TimeRequired2;
	private ButtonWidget			m_BtnSelect2;
	
	//Listing 3
	private Widget 					m_PrinterListing3;
	private ItemPreviewWidget 		m_ItemPreview3;
	private TextWidget				m_ItemName3;
	private TextWidget				m_FilamentLevel3;
	private TextWidget				m_FilamentAmount3;
	private TextWidget				m_TimeRequired3;
	private ButtonWidget			m_BtnSelect3;

	//Print Queue Menu
	private Widget 					m_PrintQueuePnl;
	private TextWidget				m_PrintQueueTitle;
	private TextListboxWidget		m_PrintQueue;
	private ButtonWidget			m_BtnRemoveFromQueue;
	ref array <ref PrintQueueItem>	m_PrintQueueItems;
	
	//GUI Init Check
	bool isUSB;
	Printer3D_FolderBase BP0Entity;
	Printer3D_FolderBase BP1Entity;
	Printer3D_FolderBase BP2Entity;
	Printer3D_FolderBase BP3Entity;
	
	EntityAI BP0EntityItem;
	EntityAI BP1EntityItem;
	EntityAI BP2EntityItem;	
	EntityAI BP3EntityItem;	

//Init
    void PrinterMenu()
    {
		m_PrintQueueItems = new array<ref PrintQueueItem>;
    	GetRPCManager().AddRPC("Printer3D", "SetMenuDataRPC", this, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("Printer3D", "SetAttVarsRPC", this, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("Printer3D", "UpdateQueueMenu", this, SingleplayerExecutionType.Both);
    }

    void ~PrinterMenu() 
    {
        PPEffects.SetBlurMenu( 0 );
        GetGame().GetUIManager().Back();
        g_Game.GetUIManager().ShowCursor(true);
        g_Game.GetUIManager().ShowUICursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetMission().GetHud().Show( true );

        if ( layoutRoot )
        	layoutRoot.Unlink();
		
		GetGame().ObjectDelete(BP0EntityItem);
		GetGame().ObjectDelete(BP1EntityItem);
		GetGame().ObjectDelete(BP2EntityItem);
		GetGame().ObjectDelete(BP3EntityItem);	
		
		if (ProgressTimer)
			delete ProgressTimer;
		if (ProgressTimerLeft)
			delete ProgressTimerLeft;
    }

    void SendRPC() {}

	void CheckState() {}

	void SetTarget(Printer3D_Printer targetObject) {
		m_target = targetObject;
	}

    override void OnShow()
    {
        super.OnShow();
        PPEffects.SetBlurMenu( 0.5 );

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetData, 1);
    }

	void GetData()
	{
		PlayerIdentity player = GetGame().GetPlayer().GetIdentity();
		GetRPCManager().SendRPC("Printer3D", "SendMenuDataRPC", new Param2<Printer3D_Printer, PlayerIdentity>(m_target, player), true, player); 
	}

    override void OnHide()
    {
        super.OnHide();
        PPEffects.SetBlurMenu( 0 );

        g_Game.GetUIManager().ShowCursor(true);
        g_Game.GetUIManager().ShowUICursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetMission().GetHud().Show( true );
    }

	void SetMenuDataRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
		Param5<bool, string, float, float, array<ref PrintQueueItem>> param;
        if(!ctx.Read(param)) return;

		if (param.param1) 
		{
			SetPrintingVars(param.param2, param.param3, param.param4);
			SetIsPrinting(true);
		} else {
			SetIsPrinting(false);
		}

		InitGUI();

		m_PrintQueueItems = param.param5;
		UpdatePrintQueue(m_PrintQueueItems);		
    }

	void UpdateQueueMenu(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<array<ref PrintQueueItem>> param;
		if(!ctx.Read(param)) return;

		m_PrintQueueItems = param.param1;
		UpdatePrintQueue(m_PrintQueueItems);
	}

	void UpdatePrintQueue(array<ref PrintQueueItem> printQueue)
	{
		string text;
		if (m_IsPrinting)
		{
			text = (new CF_Localiser("str_printermenu_queue_printing")).Format();
			text.Replace("{0}", printQueue.Count().ToString());
		}
		else if (printQueue.Count() > 0)
		{
			text = (new CF_Localiser("str_printermenu_queue_paused")).Format();
			text.Replace("{0}", printQueue.Count().ToString());
		}
		else
		{
			text = (new CF_Localiser("str_printermenu_queue_empty")).Format();
		}

	    m_PrintQueueTitle.SetText(text);
	    m_PrintQueue.ClearItems();

	    for (int i = 0; i < printQueue.Count(); i++)
	    {
			PrintQueueItem queueItem = printQueue.Get(i);
			if (queueItem && queueItem.IsValid())
			{
				string ItemName = queueItem.PrintEntityName;
				m_PrintQueue.AddItem(ItemName, queueItem, 0);
			}
	    }
	}

	void SetAttVarsRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param5<Printer3D_USB, Printer3D_FolderBase, Printer3D_FolderBase, Printer3D_FolderBase, Printer3D_FolderBase> param;
		if(!ctx.Read(param)) return;

		isUSB = param.param1 != null;
		SetVars(isUSB, param.param2, param.param3, param.param4, param.param5);
	}

    float GetPrinterMenuTextProportion()
	{
		string translated = (new CF_Localiser("str_printer_get_language")).Format();
		switch (translated)
        {
            case "2": // german
				return 0.5;
			case "3": // russian
				return 0.5;
			case "4": // polish
				return 0.75;
            case "5": // hungarian
				return 0.75;
            case "6": // italian
				return 0.75;
            case "7": // spanish
				return 0.75;
            case "8": // french
				return 0.75;
			case "9": // chinese
				return 0.75;
            case "10": // japanese
				return 0.5;
            case "11": // portuguese
                return 0.75;
			case "12": // chinesesimplified
				return 0.75;
            default:
                return 1.0;
        }
	
		// Default for all other languages
		return 1.0;
	}

    override Widget Init()
    {
        if (!m_Initialized)
        {
            layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Printer3D/Data/GUI/layouts/PrinterMenu.layout" );
            
			// Useless widgets
            m_Scroller 			= ScrollWidget.Cast( layoutRoot.FindAnyWidget( "Scroller" ) ); 
			m_Grid     			= GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "Grid" ) );
			m_Title	   			= TextWidget.Cast( layoutRoot.FindAnyWidget( "Title" ) );
			m_pnl_bg   			= layoutRoot.FindAnyWidget( "pnl_bg" );
			
			// Main functional widgets
			m_BtnStart 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnStart" ) );
			m_BtnResume			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnResumeQueue" ) );
			m_BtnStop  			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnStop" ) );
			m_BtnClearQueue		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnClearQueue" ) );
            m_BtnClose 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnClose" ) );
			
			//
			// Non grid related widgets
			//
			
			m_NoUSBInstalled 	= TextWidget.Cast( layoutRoot.FindAnyWidget( "NoUSBInstalled" ) );
			m_SelectedPrint		= TextWidget.Cast( layoutRoot.FindAnyWidget( "SelectedPrint" ) );
			m_PrintingProgress	= ProgressBarWidget.Cast( layoutRoot.FindAnyWidget( "PrintingProgress" ) );
			m_PrintingTimeLeft  = TextWidget.Cast( layoutRoot.FindAnyWidget( "PrintingTimeLeft" ) );
			m_PrintingTimeLeft.SetText("00:00:00"); 

			//
			// Item listing widgets
			//
			
			//Listing 0
			m_PrinterListing0	= layoutRoot.FindAnyWidget( "PrinterListing0" );
			m_ItemPreview0		= ItemPreviewWidget.Cast( layoutRoot.FindAnyWidget( "ItemPreview0" ) );
			m_ItemName0			= TextWidget.Cast( layoutRoot.FindAnyWidget( "ItemName0" ) );
			m_FilamentLevel0	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentLevel0" ) );
			m_FilamentAmount0	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentAmount0" ) );
			m_TimeRequired0		= TextWidget.Cast( layoutRoot.FindAnyWidget( "TimeRequired0" ) );
			m_BtnSelect0		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnSelect0" ) );

			//Listing 1
			m_PrinterListing1	= layoutRoot.FindAnyWidget( "PrinterListing1" );
			m_ItemPreview1		= ItemPreviewWidget.Cast( layoutRoot.FindAnyWidget( "ItemPreview1" ) );
			m_ItemName1			= TextWidget.Cast( layoutRoot.FindAnyWidget( "ItemName1" ) );
			m_FilamentLevel1	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentLevel1" ) );
			m_FilamentAmount1	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentAmount1" ) );
			m_TimeRequired1		= TextWidget.Cast( layoutRoot.FindAnyWidget( "TimeRequired1" ) );
			m_BtnSelect1		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnSelect1" ) );
			
			//Listing 2
			m_PrinterListing2	= layoutRoot.FindAnyWidget( "PrinterListing2" );
			m_ItemPreview2		= ItemPreviewWidget.Cast( layoutRoot.FindAnyWidget( "ItemPreview2" ) );
			m_ItemName2			= TextWidget.Cast( layoutRoot.FindAnyWidget( "ItemName2" ) );
			m_FilamentLevel2	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentLevel2" ) );
			m_FilamentAmount2	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentAmount2" ) );
			m_TimeRequired2		= TextWidget.Cast( layoutRoot.FindAnyWidget( "TimeRequired2" ) );
			m_BtnSelect2		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnSelect2" ) );
			
			//Listing 3
			m_PrinterListing3	= layoutRoot.FindAnyWidget( "PrinterListing3" );
			m_ItemPreview3		= ItemPreviewWidget.Cast( layoutRoot.FindAnyWidget( "ItemPreview3" ) );
			m_ItemName3			= TextWidget.Cast( layoutRoot.FindAnyWidget( "ItemName3" ) );
			m_FilamentLevel3	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentLevel3" ) );
			m_FilamentAmount3	= TextWidget.Cast( layoutRoot.FindAnyWidget( "FilamentAmount3" ) );
			m_TimeRequired3		= TextWidget.Cast( layoutRoot.FindAnyWidget( "TimeRequired3" ) );
			m_BtnSelect3		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnSelect3" ) );

			//Print Queue Menu
			m_PrintQueuePnl		= layoutRoot.FindAnyWidget( "print_queue_frame" );
			m_PrintQueueTitle	= TextWidget.Cast( layoutRoot.FindAnyWidget( "PrintQueueTitle" ) );
			m_PrintQueue		= TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "PrintQueue" ) );
			m_BtnRemoveFromQueue= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnRemoveFromQueue" ) );
			
            //SPINNNGING
            m_Initialized = true;
        }
		
		float textProportion = GetPrinterMenuTextProportion();
        m_Title.SetTextProportion(textProportion);
        m_BtnStart.SetTextProportion(textProportion);
        m_BtnResume.SetTextProportion(textProportion);
        m_BtnStop.SetTextProportion(textProportion);
        m_BtnClearQueue.SetTextProportion(textProportion);
        m_BtnClose.SetTextProportion(textProportion);
        m_NoUSBInstalled.SetTextProportion(textProportion);
        m_SelectedPrint.SetTextProportion(textProportion);
        m_ItemName0.SetTextProportion(textProportion);
        m_FilamentLevel0.SetTextProportion(textProportion);
        m_FilamentAmount0.SetTextProportion(textProportion);
        m_TimeRequired0.SetTextProportion(textProportion);
        m_BtnSelect0.SetTextProportion(textProportion);
        m_ItemName1.SetTextProportion(textProportion);
        m_FilamentLevel1.SetTextProportion(textProportion);
        m_FilamentAmount1.SetTextProportion(textProportion);
        m_TimeRequired1.SetTextProportion(textProportion);
        m_BtnSelect1.SetTextProportion(textProportion);
        m_ItemName2.SetTextProportion(textProportion);
        m_FilamentLevel2.SetTextProportion(textProportion);
        m_FilamentAmount2.SetTextProportion(textProportion);
        m_TimeRequired2.SetTextProportion(textProportion);
        m_BtnSelect2.SetTextProportion(textProportion);
        m_ItemName3.SetTextProportion(textProportion);
        m_FilamentLevel3.SetTextProportion(textProportion);
        m_FilamentAmount3.SetTextProportion(textProportion);
        m_TimeRequired3.SetTextProportion(textProportion);
        m_BtnSelect3.SetTextProportion(textProportion);
        m_PrintQueueTitle.SetTextProportion(textProportion);
        m_BtnRemoveFromQueue.SetTextProportion(textProportion);
		
        return layoutRoot;
    }
	
	
//Menu Functions
	

	void SetVars(bool USB, Printer3D_FolderBase BP0, Printer3D_FolderBase BP1, Printer3D_FolderBase BP2, Printer3D_FolderBase BP3)
	{
		isUSB = USB;
		BP0Entity = BP0;
		BP1Entity = BP1; 
		BP2Entity = BP2;
		BP3Entity = BP3;
	}
	
	void SetPrintingVars(string PrintName, float TimeRemaining, float TimeInitial)
	{
		if (PrintName == string.Empty) {this.Close()}
		m_BtnResume.Show(false);
		m_SelectedPrint.SetText(PrintName);
		
		ProgressTimer = new Timer(CALL_CATEGORY_GUI);
		ProgressTimer.Run(1, this, "SetProgress", null, true);
		ProgressTimerLeft = new Timer(CALL_CATEGORY_GUI);
		ProgressTimerLeft.Run(TimeRemaining, this, "SetProgress", null, false);
	}
	
	void SetProgress()
	{	
		if (ProgressTimerLeft.IsRunning())
		{
			m_PrintingTimeLeft.SetText(SecondsToTime(ProgressTimerLeft.GetRemaining()));
		}
	}
	
	void InitGUI()
	{
		if (!isUSB) 
		{ 
			m_NoUSBInstalled.Show(true); 
		} else 
		{ 
			m_NoUSBInstalled.Show(false);
		}
			
		if (BP0Entity != null) {BP0EntityItem = EntityAI.Cast(GetGame().CreateObject(BP0Entity.GetSelectedEntityName(), "0 0 0", true, false, true)); InitListing(BP0Entity, BP0EntityItem, m_PrinterListing0, m_ItemPreview0, m_ItemName0, m_FilamentLevel0, m_FilamentAmount0, m_TimeRequired0, m_BtnSelect0);} 
		if (BP1Entity != null) {BP1EntityItem = EntityAI.Cast(GetGame().CreateObject(BP1Entity.GetSelectedEntityName(), "0 0 0", true, false, true)); InitListing(BP1Entity, BP1EntityItem, m_PrinterListing1, m_ItemPreview1, m_ItemName1, m_FilamentLevel1, m_FilamentAmount1, m_TimeRequired1, m_BtnSelect1);} 
		if (BP2Entity != null) {BP2EntityItem = EntityAI.Cast(GetGame().CreateObject(BP2Entity.GetSelectedEntityName(), "0 0 0", true, false, true)); InitListing(BP2Entity, BP2EntityItem, m_PrinterListing2, m_ItemPreview2, m_ItemName2, m_FilamentLevel2, m_FilamentAmount2, m_TimeRequired2, m_BtnSelect2);} 
		if (BP3Entity != null) {BP3EntityItem = EntityAI.Cast(GetGame().CreateObject(BP3Entity.GetSelectedEntityName(), "0 0 0", true, false, true)); InitListing(BP3Entity, BP3EntityItem, m_PrinterListing3, m_ItemPreview3, m_ItemName3, m_FilamentLevel3, m_FilamentAmount3, m_TimeRequired3, m_BtnSelect3);} 

		if (BP0Entity == null && BP1Entity == null && BP2Entity == null && BP3Entity == null && isUSB)
		{
			m_NoUSBInstalled.Show(true);
			m_NoUSBInstalled.SetText("#str_printermenu_no_models");
		}
	}
	
	void InitListing(Printer3D_FolderBase EntityName, EntityAI item, Widget Panel, ItemPreviewWidget Preview, TextWidget Name, TextWidget Level, TextWidget Amount, TextWidget Time, ButtonWidget Button)
    {
		Panel.Show(true);	
		
		Preview.SetItem( EntityAI.Cast( item ));
		Preview.SetModelPosition(Vector(0.0, 0.0, 0.0)); 
		Preview.SetModelOrientation(Vector(0, 0, 0)); 
				
		Name.SetText( EntityName.GetSelectedEntityDisplayName() );
		Level.SetText( "#str_printermenu_filament_tier " + EntityName.GetSelectedEntityFilamentTier().ToString() );
		Amount.SetText( "#str_printermenu_filament_amount " + EntityName.GetSelectedEntityFilamentAmount().ToString() );
		
		Time.SetText( SecondsToTime(EntityName.GetSelectedEntityTimeToPrint()) );
	}

//Click Handler

    override bool OnClick(Widget w, int x, int y, int button)
    {
		PlayerIdentity m_playerIdentity = m_player.GetIdentity();
    	switch(w)
    	{
    		case m_BtnStart:
                GetRPCManager().SendRPC("Printer3D", "TryStartPrintingRPC", new Param6<string, int, string, int, string, Printer3D_Printer>(m_SelectedPrintN, m_SelectedPrintT, m_SelectedPrintFT, m_SelectedPrintFA, m_SelectedPrintDN, m_target), true);
    			this.Close();	
				break;
			case m_BtnResume:
				GetRPCManager().SendRPC("Printer3D", "ResumePrintQueue", new Param1<Printer3D_Printer>(m_target), true);
				this.Close();
				break;
			case m_BtnClearQueue:
				GetRPCManager().SendRPC("Printer3D", "ClearPrintQueue", new Param1<Printer3D_Printer>(m_target), true);
				GetRPCManager().SendRPC("Printer3D", "UpdateQueueMenu", new Param1<Printer3D_Printer>(m_target), true, m_playerIdentity);
				break;
    		case m_BtnStop:
                GetRPCManager().SendRPC("Printer3D", "TryStopPrintingRPC", new Param1<Printer3D_Printer>(m_target), true);
				this.Close();
            	break;
			case m_BtnRemoveFromQueue:
				int index = m_PrintQueue.GetSelectedRow();
				GetRPCManager().SendRPC("Printer3D", "RemoveFromQueue", new Param2<Printer3D_Printer, int>(m_target, index), true, m_playerIdentity);
				GetRPCManager().SendRPC("Printer3D", "UpdateQueueMenu", new Param1<Printer3D_Printer>(m_target), true, m_playerIdentity);
				break;
			case m_PrintQueue:
				for (int i = 0; i < m_PrintQueue.GetNumItems(); i++)
                {
                    m_PrintQueue.SetItemColor(i, 0, 0xFFFFFFFF);
                }

				m_PrintQueue.SetItemColor(m_PrintQueue.GetSelectedRow(), 0, 0xFFFF751A);
				break;
			
			case m_BtnSelect0:
				SetSelectedPrint(BP0Entity.GetSelectedEntityDisplayName(), BP0Entity.GetSelectedEntityName(), BP0Entity.GetSelectedEntityTimeToPrint(), BP0Entity.GetSelectedEntityFilamentAmount(), BP0Entity.GetSelectedEntityFilamentTier());
				break;
			case m_BtnSelect1:
				SetSelectedPrint(BP1Entity.GetSelectedEntityDisplayName(), BP1Entity.GetSelectedEntityName(), BP1Entity.GetSelectedEntityTimeToPrint(), BP1Entity.GetSelectedEntityFilamentAmount(), BP1Entity.GetSelectedEntityFilamentTier());
				break;
			case m_BtnSelect2:
				SetSelectedPrint(BP2Entity.GetSelectedEntityDisplayName(), BP2Entity.GetSelectedEntityName(), BP2Entity.GetSelectedEntityTimeToPrint(), BP2Entity.GetSelectedEntityFilamentAmount(), BP2Entity.GetSelectedEntityFilamentTier());
				break;
			case m_BtnSelect3:
				SetSelectedPrint(BP3Entity.GetSelectedEntityDisplayName(), BP3Entity.GetSelectedEntityName(), BP3Entity.GetSelectedEntityTimeToPrint(), BP3Entity.GetSelectedEntityFilamentAmount(), BP3Entity.GetSelectedEntityFilamentTier());
				break;
			
			
			case m_BtnClose:
                this.Close();
            	break;
    	}
        return super.OnClick(w, x, y, button);
    }

//Helpers
	
	void SetIsPrinting(bool IsPrinting) { m_IsPrinting = IsPrinting; }
	
	string SecondsToTime(int Seconds)
	{
		int Hours = Seconds / 3600;
		int Minutes = (Seconds % 3600) / 60;
		int remainingSeconds = Seconds % 60;
		
    	string formattedHours = Hours.ToString();
    	string formattedMinutes = Minutes.ToString();
    	string formattedSeconds = remainingSeconds.ToString();
	
    	if (Hours < 10) { formattedHours = "0" + formattedHours; }
    	if (Minutes < 10) { formattedMinutes = "0" + formattedMinutes; }
    	if (remainingSeconds < 10) { formattedSeconds = "0" + formattedSeconds; }

    	return formattedHours + ":" + formattedMinutes + ":" + formattedSeconds;
	}
	
	void SetSelectedPrint(string PrintDisplayName, string PrintName, int PrintTime, int FilamentAmount, int FilamentTier)
	{
		m_SelectedPrint.SetText(PrintDisplayName);
		m_SelectedPrintDN	= PrintDisplayName;
		m_SelectedPrintN 	= PrintName;
		m_SelectedPrintT 	= PrintTime;
		m_SelectedPrintFA 	= FilamentAmount;
		
		//Printer3D_Data will set the filament tiers in a modded class. This is just a placeholder.
		// Example of how it would be done in the modded class:
		//if (FilamentTier == 1) {m_SelectedPrintFT = "Printer3D_Filament_T1_Blue"}
		//if (FilamentTier == 2) {m_SelectedPrintFT = "Printer3D_Filament_T2_Red"}
		//if (FilamentTier == 3) {m_SelectedPrintFT = "Printer3D_Filament_T3_Gold"}
	}
	
	void SetPlayer(PlayerBase player)
	{
		m_player = player;
	}

    bool IsMenuOpen() 
    {
        return m_IsMenuOpen;
    }

    void SetMenuOpen(bool isMenuOpen) 
    {
        m_IsMenuOpen = isMenuOpen;
    }
	
	bool ShouldShowScrollBar()
    {
        float x, y;
        m_pnl_bg.GetScreenSize( x, y );
        return m_Scroller.GetContentHeight() > y;
    }
	
    override void Update(float timeslice)
    {
        super.Update(timeslice);

        if ( ShouldShowScrollBar() )
            m_Scroller.SetAlpha(1.0);
        else
            m_Scroller.SetAlpha(0.0);
    }
};